#ifndef GL_CONTEXT_HPP
#define GL_CONTEXT_HPP

#include "SDL_video.h"
#include <memory>

class gl_context
{
    std::shared_ptr<SDL_GLContext> context{nullptr};

    static void delete_context(SDL_GLContext *context)
    {
        if (context == nullptr) return;
        SDL_GL_DeleteContext(*context);
        delete context;
    }

  public:
    gl_context() = default;
    gl_context(SDL_Window *window);

    constexpr const SDL_GLContext &get()
    {
        return *context;
    }
};

#endif
