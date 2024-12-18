#include "InputManager.hpp"
#include "SDL_events.h"
#include "glm/ext/vector_float2.hpp"

glm::vec2 InputManager::MouseRelative() const
{
    return {m_mouseX, m_mouseY};
}

void InputManager::ChangeSensitivity(const f32 value)
{
    m_sensitivity = value;
}

void InputManager::UpdateStates()
{
    for (const auto &[code, state] : m_inputMap) {
        if (state == InputState::RELEASED) m_inputMap[code] = InputState::NONE;
        if (state == InputState::PRESSED) m_inputMap[code] = InputState::HELD;
    }

    m_mouseX = 0.0F;
    m_mouseY = 0.0F;
}

void InputManager::Update(const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN && m_inputMap[event.key.keysym.scancode] == InputState::NONE)
        m_inputMap[event.key.keysym.scancode] = InputState::PRESSED;

    if (event.type == SDL_KEYUP && m_inputMap[event.key.keysym.scancode] == InputState::HELD)
        m_inputMap[event.key.keysym.scancode] = InputState::RELEASED;

    if (event.type == SDL_MOUSEMOTION) {
        m_mouseX = static_cast<float>(event.motion.xrel);
        m_mouseY = static_cast<float>(-event.motion.yrel);
        m_mouseX *= m_sensitivity;
        m_mouseY *= m_sensitivity;
    }
}

bool InputManager::IsPressed(SDL_Scancode key) const
{
    return m_inputMap.at(key) == InputState::PRESSED;
}

bool InputManager::IsHeld(SDL_Scancode key) const
{
    return m_inputMap.at(key) == InputState::HELD;
}

bool InputManager::IsReleased(SDL_Scancode key) const
{
    return m_inputMap.at(key) == InputState::RELEASED;
}

bool InputManager::IsMouseMoving() const
{
    return m_mouseX < 0 || m_mouseX > 0 || m_mouseY < 0 || m_mouseY > 0;
}
