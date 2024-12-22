#include <algorithm>
#include <cmath>
#include <memory>
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

    AssetRegistry &registry = AssetRegistry::GetInstance();
    registry.AddShader("default", {"vertex.vert.glsl", "fragment.frag.glsl"});
    registry.AddTexture("test", {"test.png"});
    registry.AddMesh("sphere", {MeshLoader::LoadObj("sphere.obj")});
    registry.AddMesh("cube", {MeshLoader::LoadObj("cube.obj")});

    // Objects
    Camera camera(window.GetWidth(), window.GetHeight());
    Model  model = ModelFactory::CreateModel("cube", "test", "default");
    camera.GetPosition() = Vec3(0.0F, 0.0F, 3.0F);

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

                glm::vec2 relative = inputManager.MouseRelative();
                if (inputManager.IsMouseMoving()) {
                    glm::vec3 &rotation = camera.GetRotation();
                    rotation.x += relative.x;
                    rotation.y += relative.y;
                    rotation.y = std::min(rotation.y, 89.0F);
                    rotation.y = std::max(rotation.y, -89.0F);

                    glm::vec3 direction;
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

            model.Rotate(static_cast<float>(elapsed), UP_AXIS);
            camera.Update();

            elapsed -= 1.0;
        }

        matRenderContext->ClearWithColor(0.2F, 0.3F, 0.3F);
        model.Render(camera);
        window.SwapBuffer();
        SDL_Delay(1);
    }

    return 0;
}
