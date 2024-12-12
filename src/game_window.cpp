#include "game_window.hpp"
#include "SDL_video.h"
#include "gl_context.hpp"
#include <string_view>

game_window::game_window(std::string_view title, int width, int height) : width(width), height(height)
{
    window.reset(SDL_CreateWindow(title.data(),
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  width,
                                  height,
                                  SDL_WINDOW_OPENGL),
                 SDL_DestroyWindow);
    context = gl_context(window.get());
}
