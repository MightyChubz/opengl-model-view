#include <algorithm>
#include <cmath>
#include <cstddef>
#include <list>
#include <map>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <strings.h>

#include "AssetRegistry.hpp"
#include "Camera.hpp"
#include "GameWindow.hpp"
#include "InputManager.hpp"
#include "MaterialSystem.hpp"
#include "Math.hpp"
#include "Mesh.hpp"
#include "MeshLoader.hpp"
#include "Model.hpp"
#include "ModelFactory.hpp"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_scancode.h"
#include "SDL_timer.h"
#include "SdlSubsystem.hpp"
#include "Shader.hpp"
#include "StdDefs.hpp"
#include "Texture.hpp"

int main(int argc, char **argv)
{
    SdlSubsystem subsystem;
    if (!subsystem.IsInitialized()) return 0;

    GameWindow window("Testing window", 1280, 720);
    window.SetRelativeMouseMode(true);

    std::shared_ptr<MaterialSystem> matRenderContext;
    MaterialSystem::GetContext(matRenderContext);
    matRenderContext->EnableDepthTest();
    matRenderContext->ConfigureErrorHandling();

    AssetRegistry &registry = AssetRegistry::GetInstance();
    registry.AddShader("default", {"vertex.vert.glsl", "fragment.frag.glsl"});
    registry.AddTexture("test", {"test.png"});
    registry.AddMesh("sphere", {MeshLoader::LoadObj("sphere.obj")});
    registry.AddMesh("cube", {MeshLoader::LoadObj("cube.obj")});

    // Objects
    Camera camera(window.GetWidth(), window.GetHeight());
    camera.GetPosition() = Vec3(0.0F, 0.0F, 3.0F);
    std::list<Model> models;
    models.push_back(ModelFactory::CreateModel("cube", "test", "default"));
    models.push_back(ModelFactory::CreateModel("sphere", "test", "default"));
    models.push_back(ModelFactory::CreateModel("cube", "test", "default"));
    models.front().GetTransform().GetPosition().x += 3.0F;
    models.back().GetTransform().GetPosition().x -= 3.0F;

    matRenderContext->SetViewport(0, 0, window.GetWidth(), window.GetHeight());

    auto         inputMap = {SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_G};
    InputManager inputManager(inputMap);
    u64          then      = SDL_GetTicks64();
    f64          elapsed   = 0;
    f64          msPerTick = 1000.0 / 60.0;
    bool         running   = true;
    SDL_Event    event;
    while (running) {
        u64 now = SDL_GetTicks64();
        elapsed += static_cast<f32>(now - then) / msPerTick;
        then = now;

        while (elapsed >= 1.0) {
            inputManager.UpdateStates();
            while (SDL_PollEvent(&event) != 0) {
                bool shouldClose = event.type == SDL_QUIT ||
                                   (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE);
                if (shouldClose) {
                    running = false;
                }

                inputManager.Update(event);

                Vec2 relative = inputManager.MouseRelative();
                if (inputManager.IsMouseMoving()) {
                    Vec3 &rotation = camera.GetRotation();
                    rotation.x += relative.x;
                    rotation.y += relative.y;
                    rotation.y = std::min(rotation.y, 89.0F);
                    rotation.y = std::max(rotation.y, -89.0F);

                    Vec3 direction;
                    direction.x    = std::cos(Radians(rotation.x)) * std::cos(Radians(rotation.y));
                    direction.y    = std::sin(Radians(rotation.y));
                    direction.z    = std::sin(Radians(rotation.x)) * std::cos(Radians(rotation.y));
                    camera.Front() = Normalize(direction);
                }
            }

            const float cameraSpeed = 2.5F * static_cast<float>(elapsed) / 164.0F;
            if (inputManager.IsHeld(SDL_SCANCODE_W)) camera.GetPosition() += cameraSpeed * camera.Front();
            if (inputManager.IsHeld(SDL_SCANCODE_S)) camera.GetPosition() -= cameraSpeed * camera.Front();
            if (inputManager.IsHeld(SDL_SCANCODE_A))
                camera.GetPosition() -= Normalize(Cross(camera.Front(), UP_AXIS)) * cameraSpeed;
            if (inputManager.IsHeld(SDL_SCANCODE_D))
                camera.GetPosition() += Normalize(Cross(camera.Front(), UP_AXIS)) * cameraSpeed;
            if (inputManager.IsPressed(SDL_SCANCODE_G)) matRenderContext->ToggleDebugWireframe();

            for (auto &model : models) {
                model.GetTransform().GetRotation().y += static_cast<float>(elapsed);
            }
            camera.Update();

            elapsed -= 1.0;
        }

        matRenderContext->ClearWithColor(0.2F, 0.3F, 0.3F);
        std::map<u32, std::list<Matrix4>> instanceMap;
        for (const auto &model : models) {
            std::list<Matrix4> &instanceMatrixList = instanceMap[model.GetId()];
            instanceMatrixList.push_back(model.GetTransform().GetModel());
        }

        for (const auto &[id, transformList] : instanceMap) {
            namespace ranges             = std::ranges;
            namespace rview              = ranges::views;
            const auto    &model         = *ranges::find_if(models, [&](const auto &e) { return e.GetId() == id; });
            const Shader  &modelShader   = model.GetShader();
            const Texture &modelTexture  = model.GetTexture();
            const Mesh    &modelMesh     = model.GetMesh();
            const size_t   instanceCount = transformList.size();
            modelShader.Use();

            for (const auto &[index, mat] : rview::enumerate(transformList)) {
                const std::string varName = "models[" + std::to_string(index) + "]";
                modelShader.Set(varName, mat);
            }

            modelShader.Set("view", camera.CameraView());
            modelShader.Set("projection", camera.ProjectionMat());
            modelTexture.Use(0);

            matRenderContext->BindVertexArray(modelMesh.GetBuffers().m_vao);
            matRenderContext->BindBuffer(BufferType::ELEMENT_ARRAY, modelMesh.GetBuffers().m_ebo);
            matRenderContext->SetTargetIndiceSize(modelMesh.GetIndiceSize());
            matRenderContext->DrawElementsInstanced(instanceCount);
            matRenderContext->UnbindVertexArray();
            matRenderContext->UnbindBuffer();
        }

        window.SwapBuffer();
        SDL_Delay(1);
    }

    return 0;
}
