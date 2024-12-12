#include <GL/glew.h>

#include "SDL_video.h"
#include "gl_context.hpp"

gl_context::gl_context(SDL_Window *window)
{
    context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    glewInit();
}

gl_context::~gl_context()
{
    SDL_GL_DeleteContext(context);
}

const SDL_GLContext &gl_context::get()
{
    return context;
}
