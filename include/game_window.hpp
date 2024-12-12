#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include "SDL_video.h"
#include "gl_context.hpp"
#include <memory>
#include <string_view>
class game_window
{
    std::shared_ptr<SDL_Window> window{nullptr};
    gl_context                  context{};
    int                         width{0};
    int                         height{0};

  public:
    game_window(std::string_view title, int width, int height);

    [[nodiscard]] constexpr SDL_Window *get()
    {
        return window.get();
    }

    [[nodiscard]] constexpr const SDL_GLContext &get_context()
    {
        return context.get();
    }

    [[nodiscard]] constexpr int get_width()
    {
        return width;
    }

    [[nodiscard]] constexpr int get_height()
    {
        return height;
    }
};

#endif
