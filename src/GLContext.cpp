#include <GL/glew.h>

#include "GLContext.hpp"
#include "SDL_video.h"

void GlContext::DeleteContext(SDL_GLContext *context)
{
    if (context == nullptr) return;
    SDL_GL_DeleteContext(*context);
    delete context;
}

GlContext::GlContext(SDL_Window *window)
{
    m_context.reset(new SDL_GLContext(SDL_GL_CreateContext(window)), DeleteContext);
    SDL_GL_MakeCurrent(window, m_context.get());
    glewInit();
}
