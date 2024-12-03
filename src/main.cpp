#include <GL/glew.h>
#include <GL/glu.h>
#include <cmath>
#include <string_view>
#include <strings.h>
#include <vector>

#include "SDL.h"
#include "SDL_events.h"
#include "SDL_image.h"
#include "SDL_keyboard.h"
#include "SDL_mouse.h"
#include "SDL_scancode.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "asset_registry.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "input_manager.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "stddefs.hpp"
#include "texture.hpp"

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window   *window  = SDL_CreateWindow("Testing window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          1280,
                                          720,
                                          SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    glewInit();
    glEnable(GL_DEPTH_TEST);

    AssetRegistry &registry = AssetRegistry::get_instance();
    registry.add_shader("default", {"shaders/vertex.vert.glsl", "shaders/fragment.frag.glsl"});
    registry.add_texture("test", {"imgs/test.png"});
    registry.add_mesh("sphere", {"models/sphere.obj"});
    registry.add_mesh("cube", {"models/cube.obj"});

    const Shader  &shader          = registry.get_shader("default");
    const Texture &texture         = registry.get_texture("test");
    const Mesh    &mesh            = registry.get_mesh("sphere");

    // Camera
    glm::vec3      camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3      camera_front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3      camera_up       = glm::vec3(0.0f, 1.0f, 0.0f);

    // Position and rotation
    int            width, height;
    SDL_GetWindowSize(window, &width, &height);
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view  = glm::mat4(1.0);
    glm::mat4 proj  = glm::mat4(1.0);
    model           = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0f, 0.0f));
    view            = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
    proj = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

    glViewport(0, 0, width, height);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::vector<SDL_Scancode> keys = {SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D};
    InputManager              input_manager(keys);
    float                     yaw = 0.0f, pitch = 0.0f;
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
                    yaw += relative.x;
                    pitch += relative.y;
                    if (pitch > 89.0f) pitch = 89.0f;
                    if (pitch < -89.0f) pitch = 89.0f;

                    glm::vec3 direction;
                    direction.x  = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
                    direction.y  = std::sin(glm::radians(pitch));
                    direction.z  = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
                    camera_front = glm::normalize(direction);
                }
            }

            const float camera_speed = 2.5f * elapsed / 100;
            if (input_manager.is_held(SDL_SCANCODE_W)) camera_position += camera_speed * camera_front;
            if (input_manager.is_held(SDL_SCANCODE_S)) camera_position -= camera_speed * camera_front;
            if (input_manager.is_held(SDL_SCANCODE_A))
                camera_position -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
            if (input_manager.is_held(SDL_SCANCODE_D))
                camera_position += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;

            model = glm::rotate(model, static_cast<float>(elapsed / 100), glm::vec3(0.5, 1.0, 0.0));
            view  = glm::lookAt(camera_position, camera_position + camera_front, camera_up);

            elapsed -= 1.0;
        }

        glClearColor(0.2f, 0.3f, 0.3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.set("model", model);
        shader.set("view", view);
        shader.set("projection", proj);
        texture.use(0);
        mesh.render();

        SDL_GL_SwapWindow(window);
        SDL_Delay(1);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
