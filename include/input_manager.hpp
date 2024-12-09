#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <unordered_map>
#include <vector>

#include "SDL_events.h"
#include "SDL_scancode.h"
#include "glm/ext/vector_float2.hpp"
#include "stddefs.hpp"

enum class InputState : u8 { PRESSED = 0, HELD = 1, RELEASED = 2, NONE = 3 };

class InputManager
{
    std::unordered_map<SDL_Scancode, InputState> input_map;
    f32                                          mouse_x{0.0f};
    f32                                          mouse_y{0.0f};
    f32                                          sensitivity{0.1f};

  public:
    InputManager(const std::vector<SDL_Scancode> &input_array);

    glm::vec2 mouse_relative();

    void change_sensitivity(const f32 value);

    void update_states();
    void update(const SDL_Event &event);
    bool is_pressed(SDL_Scancode key) const;
    bool is_held(SDL_Scancode key) const;
    bool is_released(SDL_Scancode key) const;
    bool is_mouse_moving() const;
};

#endif
