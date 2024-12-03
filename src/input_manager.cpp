#include <vector>

#include "SDL_events.h"
#include "glm/ext/vector_float2.hpp"
#include "input_manager.hpp"

InputManager::InputManager(const std::vector<SDL_Scancode> &input_array)
{
    for (const auto &key : input_array) {
        input_map[key] = InputState::NONE;
    }
}

glm::vec2 InputManager::mouse_relative()
{
    return {mouse_x, mouse_y};
}

void InputManager::change_sensitivity(const f32 value)
{
    sensitivity = value;
}

void InputManager::update_states()
{
    for (const auto &[code, state] : input_map) {
        if (state == InputState::RELEASED) input_map[code] = InputState::NONE;
        if (state == InputState::PRESSED) input_map[code] = InputState::HELD;
    }

    mouse_x = 0.0f;
    mouse_y = 0.0f;
}

void InputManager::update(const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN && input_map[event.key.keysym.scancode] == InputState::NONE)
        input_map[event.key.keysym.scancode] = InputState::PRESSED;

    if (event.type == SDL_KEYUP && input_map[event.key.keysym.scancode] == InputState::HELD)
        input_map[event.key.keysym.scancode] = InputState::RELEASED;

    if (event.type == SDL_MOUSEMOTION) {
        mouse_x = event.motion.xrel;
        mouse_y = -event.motion.yrel;
        mouse_x *= sensitivity;
        mouse_y *= sensitivity;
    }
}

bool InputManager::is_pressed(SDL_Scancode key) const
{
    return input_map.at(key) == InputState::PRESSED;
}

bool InputManager::is_held(SDL_Scancode key) const
{
    return input_map.at(key) == InputState::HELD;
}

bool InputManager::is_released(SDL_Scancode key) const
{
    return input_map.at(key) == InputState::RELEASED;
}

bool InputManager::is_mouse_moving() const
{
    return mouse_x < 0 || mouse_x > 0 || mouse_y < 0 || mouse_y > 0;
}
