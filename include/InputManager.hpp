#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <ranges>
#include <unordered_map>

#include "Math.hpp"
#include "SDL_events.h"
#include "SDL_scancode.h"
#include "StdDefs.hpp"

enum class InputState : u8 { PRESSED = 0, HELD = 1, RELEASED = 2, NONE = 3 };

class InputManager final
{
    std::unordered_map<SDL_Scancode, InputState> m_inputMap;
    f32                                          m_mouseX{0.0F};
    f32                                          m_mouseY{0.0F};
    f32                                          m_sensitivity{0.1F};

  public:
    template <std::ranges::sized_range RANGE>
    InputManager(RANGE input_map_array)
    {
        for (const auto &key : input_map_array) {
            m_inputMap[key] = InputState::NONE;
        }
    }

    Vec2 MouseRelative() const;

    void ChangeSensitivity(const f32 value);

    void UpdateStates();
    void Update(const SDL_Event &event);
    bool IsPressed(SDL_Scancode key) const;
    bool IsHeld(SDL_Scancode key) const;
    bool IsReleased(SDL_Scancode key) const;
    bool IsMouseMoving() const;
};

#endif
