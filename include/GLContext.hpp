#ifndef GL_CONTEXT_HPP
#define GL_CONTEXT_HPP

#include <memory>

#include "SDL_video.h"

class GlContext final
{
    std::shared_ptr<SDL_GLContext> m_context{nullptr};

    static void DeleteContext(SDL_GLContext *context);

  public:
    GlContext() = default;
    GlContext(SDL_Window *window);

    [[nodiscard]] constexpr const SDL_GLContext &Get() const
    {
        return *m_context;
    }
};

#endif
