#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <cstddef>
#include <unordered_map>

#include "SDL_events.h"
#include "SDL_scancode.h"
#include "StdDefs.hpp"
#include "glm/ext/vector_float2.hpp"

enum class InputState : u8 { PRESSED = 0, HELD = 1, RELEASED = 2, NONE = 3 };

class InputManager
{
    std::unordered_map<SDL_Scancode, InputState> m_inputMap;
    f32                                          m_mouseX{0.0F};
    f32                                          m_mouseY{0.0F};
    f32                                          m_sensitivity{0.1F};

  public:
    template <size_t T>
    InputManager(const std::array<SDL_Scancode, T> &input_array)
    {
        for (const auto &key : input_array) {
            m_inputMap[key] = InputState::NONE;
        }
    }

    glm::vec2 MouseRelative() const;

    void ChangeSensitivity(const f32 value);

    void UpdateStates();
    void Update(const SDL_Event &event);
    bool IsPressed(SDL_Scancode key) const;
    bool IsHeld(SDL_Scancode key) const;
    bool IsReleased(SDL_Scancode key) const;
    bool IsMouseMoving() const;
};

#endif
