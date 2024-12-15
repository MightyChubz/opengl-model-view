#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstddef>
#include <memory>
#include <string_view>

#include "MaterialSystem.hpp"
#include "SDL_surface.h"
#include "StdDefs.hpp"

class Texture final
{
    std::shared_ptr<MaterialSystem> m_matRenderContext{nullptr};
    std::shared_ptr<TEXTURE_HANDLE> m_texture{nullptr};

    static void DestructTexture(TEXTURE_HANDLE *handle)
    {
        if (handle == nullptr) return;
        std::shared_ptr<MaterialSystem> matRenderContext;
        MaterialSystem::GetContext(matRenderContext);
        matRenderContext->DeleteTexture(*handle);
        delete handle;
    }

    void flipSurface(SDL_Surface *surface, bool horizontal, bool vertical);

  public:
    Texture() = default;
    Texture(const std::string_view path);

    void Use(size_t slot) const;
};

#endif
