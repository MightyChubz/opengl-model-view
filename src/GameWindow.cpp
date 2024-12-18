#include <string_view>

#include "GLContext.hpp"
#include "GameWindow.hpp"
#include "SDL_mouse.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"

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

void GameWindow::SetRelativeMouseMode(bool value) const
{
    SDL_SetRelativeMouseMode(value ? SDL_TRUE : SDL_FALSE);
}

void GameWindow::SwapBuffer() const
{
    SDL_GL_SwapWindow(m_window.get());
}
