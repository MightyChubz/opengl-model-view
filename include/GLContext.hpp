#ifndef GL_CONTEXT_HPP
#define GL_CONTEXT_HPP

#include <memory>

#include "SDL_video.h"

class GlContext
{
    std::shared_ptr<SDL_GLContext> m_context{nullptr};

    static void DeleteContext(SDL_GLContext *context)
    {
        if (context == nullptr) return;
        SDL_GL_DeleteContext(*context);
        delete context;
    }

  public:
    GlContext() = default;
    GlContext(SDL_Window *window);

    constexpr const SDL_GLContext &Get()
    {
        return *m_context;
    }
};

#endif
