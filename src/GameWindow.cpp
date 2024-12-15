#include "game_window.hpp"
#include "SDL_video.h"
#include "gl_context.hpp"
#include <string_view>

GameWindow::GameWindow(std::string_view title, int width, int height) : m_width(width), m_height(height)
{
    m_window.reset(SDL_CreateWindow(title.data(),
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    width,
                                    height,
                                    SDL_WINDOW_OPENGL),
                   SDL_DestroyWindow);
    m_context = GlContext(m_window.get());
}
