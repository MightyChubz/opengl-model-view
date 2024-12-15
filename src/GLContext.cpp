#include <GL/glew.h>

#include "GLContext.hpp"
#include "SDL_video.h"

GlContext::GlContext(SDL_Window *window)
{
    m_context.reset(new SDL_GLContext(SDL_GL_CreateContext(window)), DeleteContext);
    SDL_GL_MakeCurrent(window, m_context.get());
    glewInit();
}
