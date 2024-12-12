#ifndef GL_CONTEXT_HPP
#define GL_CONTEXT_HPP

#include "SDL_video.h"

class gl_context
{
    SDL_GLContext context;

  public:
    gl_context(SDL_Window *window);
    ~gl_context();

    constexpr const SDL_GLContext &get()
    {
        return context;
    }
};

#endif
