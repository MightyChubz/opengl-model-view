#include <GL/glew.h>

#include "SDL_video.h"
#include "gl_context.hpp"

GlContext::GlContext(SDL_Window *window)
{
    m_context.reset(new SDL_GLContext(SDL_GL_CreateContext(window)), DeleteContext);
    SDL_GL_MakeCurrent(window, m_context.get());
    glewInit();
}
