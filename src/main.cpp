#include <GL/glew.h>
#include <GL/glu.h>
#include <cmath>
#include <string_view>
#include <strings.h>
#include <vector>

#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_mouse.h"
#include "SDL_scancode.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "asset_registry.hpp"
#include "camera.hpp"
#include "game_window.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "input_manager.hpp"
#include "mesh.hpp"
#include "mesh_loader.hpp"
#include "sdl_subsystem.hpp"
#include "shader.hpp"
#include "stddefs.hpp"
#include "texture.hpp"

int main(int argc, char **argv)
{
    sdl_subsystem subsystem;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    game_window window("Testing window", 1280, 720);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    glEnable(GL_DEPTH_TEST);

    MeshLoader     mesh_loader;
    AssetRegistry &registry = AssetRegistry::get_instance();
    registry.add_shader("default", {"vertex.vert.glsl", "fragment.frag.glsl"});
    registry.add_texture("test", {"test.png"});
    registry.add_mesh("sphere", {mesh_loader.load_obj("sphere.obj")});
    registry.add_mesh("cube", {mesh_loader.load_obj("cube.obj")});

    const Shader  &shader  = registry.get_shader("default");
    const Texture &texture = registry.get_texture("test");
    const Mesh    &mesh    = registry.get_mesh("sphere");

    // Position and rotation
    int width, height;
    SDL_GetWindowSize(window.get(), &width, &height);
    Camera    camera(width, height);
    glm::mat4 model = glm::mat4(1.0);
    model           = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0f, 0.0f));

    glViewport(0, 0, width, height);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::vector<SDL_Scancode> keys = {SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D};
    InputManager              input_manager(keys);
    u64                       then      = SDL_GetTicks64();
    f64                       elapsed   = 0;
    f64                       msPerTick = 1000.0 / 60.0;
    bool                      running   = true;
    SDL_Event                 event;
    while (running) {
        u64 now = SDL_GetTicks64();
        elapsed += static_cast<f32>(now - then) / msPerTick;
        then = now;

        while (elapsed >= 1.0) {
            input_manager.update_states();
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT ||
                    (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
                    running = false;
                }

                input_manager.update(event);

                glm::vec2 relative = input_manager.mouse_relative();
                if (input_manager.is_mouse_moving()) {
                    glm::vec2 &rotation = camera.angle();
                    rotation.x += relative.x;
                    rotation.y += relative.y;
                    if (rotation.y > 89.0f) rotation.y = 89.0f;
                    if (rotation.y < -89.0f) rotation.y = 89.0f;

                    glm::vec3 direction;
                    direction.x    = std::cos(glm::radians(rotation.x)) * std::cos(glm::radians(rotation.y));
                    direction.y    = std::sin(glm::radians(rotation.y));
                    direction.z    = std::sin(glm::radians(rotation.x)) * std::cos(glm::radians(rotation.y));
                    camera.front() = glm::normalize(direction);
                }
            }

            const float camera_speed = 2.5f * elapsed / 100;
            if (input_manager.is_held(SDL_SCANCODE_W)) camera.position() += camera_speed * camera.front();
            if (input_manager.is_held(SDL_SCANCODE_S)) camera.position() -= camera_speed * camera.front();
            if (input_manager.is_held(SDL_SCANCODE_A))
                camera.position() -= glm::normalize(glm::cross(camera.front(), camera.up())) * camera_speed;
            if (input_manager.is_held(SDL_SCANCODE_D))
                camera.position() += glm::normalize(glm::cross(camera.front(), camera.up())) * camera_speed;

            model = glm::rotate(model, static_cast<float>(elapsed / 100), glm::vec3(0.5, 1.0, 0.0));
            camera.update();

            elapsed -= 1.0;
        }

        glClearColor(0.2f, 0.3f, 0.3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.set("model", model);
        shader.set("view", camera.camera_view());
        shader.set("projection", camera.projection_mat());
        texture.use(0);
        mesh.render();

        SDL_GL_SwapWindow(window.get());
        SDL_Delay(1);
    }

    return 0;
}
