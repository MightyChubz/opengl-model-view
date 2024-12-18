#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include <memory>
#include <string_view>

#include "GLContext.hpp"
#include "SDL_video.h"

class GameWindow
{
    std::shared_ptr<SDL_Window> m_window{nullptr};
    GlContext                   m_context;
    int                         m_width{0};
    int                         m_height{0};

  public:
    GameWindow(std::string_view title, int width, int height);

    [[nodiscard]] constexpr SDL_Window *Get()
    {
        return m_window.get();
    }

    [[nodiscard]] constexpr const SDL_GLContext &GetContext()
    {
        return m_context.Get();
    }

    [[nodiscard]] constexpr int GetWidth() const
    {
        return m_width;
    }

    [[nodiscard]] constexpr int GetHeight() const
    {
        return m_height;
    }

    void SetRelativeMouseMode(bool value) const;

    void SwapBuffer() const;
};

#endif
