#include <GL/glew.h>
#include <GL/glu.h>
#include <cmath>
#include <cstddef>
#include <functional>
#include <string_view>
#include <strings.h>
#include <unordered_map>
#include <vector>

#include "SDL.h"
#include "SDL_events.h"
#include "SDL_image.h"
#include "SDL_keyboard.h"
#include "SDL_log.h"
#include "SDL_mouse.h"
#include "SDL_scancode.h"
#include "SDL_stdinc.h"
#include "SDL_surface.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "rapidobj.hpp"
#include "shader.hpp"
#include "stddefs.hpp"
#include "texture.hpp"

struct Vertex {
    glm::vec3 vertice;
    glm::vec2 texcoord;

    Vertex() = default;
    Vertex(glm::vec3 vertice, glm::vec2 texcoord) : vertice(vertice), texcoord(texcoord)
    {
    }

    bool operator==(const Vertex &v) const
    {
        return vertice.x == v.vertice.x && vertice.y == v.vertice.y && vertice.z == v.vertice.z &&
               texcoord.x == v.texcoord.x && texcoord.y == v.texcoord.y;
    }
};

template <> struct std::hash<Vertex> {
    size_t operator()(const Vertex &v) const noexcept
    {
        size_t h1 = std::hash<float>{}(v.vertice.x);
        size_t h2 = std::hash<float>{}(v.vertice.y);
        size_t h3 = std::hash<float>{}(v.vertice.z);
        size_t h4 = std::hash<float>{}(v.texcoord.x);
        size_t h5 = std::hash<float>{}(v.texcoord.y);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4);
    }
};

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

    Shader           shader("shaders/vertex.vert.glsl", "shaders/fragment.frag.glsl");

    rapidobj::Result data = rapidobj::ParseFile("models/sphere.obj");

    SDL_Log("Loaded model: %s", "models/sphere.obj");
    SDL_Log("Total indices: %zu", data.shapes.front().mesh.indices.size());
    SDL_Log("Total positions: %zu", data.attributes.positions.size());
    SDL_Log("Total texcoords: %zu", data.attributes.texcoords.size());

    i32                             index_offset = 0;
    std::vector<u32>                indices;
    std::vector<Vertex>             vertices;
    std::unordered_map<Vertex, u32> verts;
    for (const auto &faces : data.shapes.front().mesh.num_face_vertices) {
        for (int i = 0; i < faces; ++i) {
            rapidobj::Index idx = data.shapes.front().mesh.indices[index_offset + i];
            Vertex          vertex;
            vertex.vertice.x  = data.attributes.positions[idx.position_index * 3];
            vertex.vertice.y  = data.attributes.positions[idx.position_index * 3 + 1];
            vertex.vertice.z  = data.attributes.positions[idx.position_index * 3 + 2];
            vertex.texcoord.x = data.attributes.texcoords[idx.texcoord_index * 2];
            vertex.texcoord.y = data.attributes.texcoords[idx.texcoord_index * 2 + 1];

            if (!verts.count(vertex)) {
                vertices.push_back(vertex);
                verts[vertex] = vertices.size() - 1;
            }

            indices.push_back(verts[vertex]);
        }

        index_offset += faces;
    }

    SDL_Log("Map size: %zu", verts.size());
    SDL_Log("Vertices: %zu", vertices.size());
    SDL_Log("Indices: %zu", indices.size());

    u32 vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, texcoord)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Texture
    Texture   texture("imgs/test.png");

    // Camera
    glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camera_up       = glm::vec3(0.0f, 1.0f, 0.0f);

    // Position and rotation
    int       width, height;
    SDL_GetWindowSize(window, &width, &height);
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view  = glm::mat4(1.0);
    glm::mat4 proj  = glm::mat4(1.0);
    model           = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0f, 0.0f));
    view            = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
    proj = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

    glViewport(0, 0, width, height);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float     yaw = 0.0f, pitch = 0.0f;
    u64       then      = SDL_GetTicks64();
    f64       elapsed   = 0;
    f64       msPerTick = 1000.0 / 60.0;
    bool      running   = true;
    SDL_Event event;
    while (running) {
        u64 now = SDL_GetTicks64();
        elapsed += static_cast<f32>(now - then) / msPerTick;
        then = now;

        while (elapsed >= 1.0) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT ||
                    (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
                    running = false;
                }

                switch (event.type) {
                case SDL_MOUSEMOTION: {
                    float       mouse_x     = event.motion.xrel;
                    float       mouse_y     = -event.motion.yrel;
                    const float sensitivity = 0.1f;
                    mouse_x *= sensitivity;
                    mouse_y *= sensitivity;

                    yaw += mouse_x;
                    pitch += mouse_y;
                    if (pitch > 89.0f) pitch = 89.0f;
                    if (pitch < -89.0f) pitch = 89.0f;

                    glm::vec3 direction;
                    direction.x  = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
                    direction.y  = std::sin(glm::radians(pitch));
                    direction.z  = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
                    camera_front = glm::normalize(direction);
                }
                }
            }

            const float  camera_speed = 2.5f * elapsed / 100;
            const Uint8 *keystate     = SDL_GetKeyboardState(nullptr);
            if (keystate[SDL_SCANCODE_W]) camera_position += camera_speed * camera_front;
            if (keystate[SDL_SCANCODE_S]) camera_position -= camera_speed * camera_front;
            if (keystate[SDL_SCANCODE_A])
                camera_position -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
            if (keystate[SDL_SCANCODE_D])
                camera_position += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;

            model = glm::rotate(model, static_cast<float>(elapsed / 100), glm::vec3(0.5, 1.0, 0.0));
            view  = glm::lookAt(camera_position, camera_position + camera_front, camera_up);

            elapsed -= 1.0;
        }

        glClearColor(0.2f, 0.3f, 0.3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        u32 model_location = glGetUniformLocation(*shader.id, "model");
        u32 view_location  = glGetUniformLocation(*shader.id, "view");
        u32 proj_location  = glGetUniformLocation(*shader.id, "projection");
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj));

        texture.use(0);
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        SDL_GL_SwapWindow(window);
        SDL_Delay(1);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
