#include <GL/glew.h>

#include "SDL_video.h"
#include "gl_context.hpp"

gl_context::gl_context(SDL_Window *window)
{
    context.reset(new SDL_GLContext(SDL_GL_CreateContext(window)), delete_context);
    SDL_GL_MakeCurrent(window, context.get());
    glewInit();
}
