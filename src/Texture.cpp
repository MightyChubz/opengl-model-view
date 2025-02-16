#include <cstddef>
#include <memory>
#include <string_view>

#include "MaterialSystem.hpp"
#include "SDL_image.h"
#include "SDL_log.h"
#include "SDL_surface.h"
#include "StdDefs.hpp"
#include "Texture.hpp"

void Texture::DestructTexture(const TEXTURE_HANDLE *handle)
{
    if (handle == nullptr) return;
    std::shared_ptr<MaterialSystem> matRenderContext;
    MaterialSystem::GetContext(matRenderContext);
    matRenderContext->DeleteTexture(*handle);
    delete handle;
}

void Texture::FlipSurface(SDL_Surface *surface, const bool horizontal, const bool vertical)
{
    if (surface == nullptr) return;

    SDL_LockSurface(surface);

    i32   pitch  = surface->pitch;
    i32   height = surface->h;
    i32   width  = surface->w;
    auto *pixels = static_cast<Uint8 *>(surface->pixels);

    for (i32 y = 0; y < height >> 1; ++y) {
        for (i32 x = 0; x < width; ++x) {
            i32 targetY = vertical ? height - 1 - y : y;
            i32 targetX = horizontal ? width - 1 - x : x;

            Uint8 *pixel1 = pixels + (y * pitch) + (x * surface->format->BytesPerPixel);
            Uint8 *pixel2 = pixels + (targetY * pitch) + (targetX * surface->format->BytesPerPixel);
            for (i32 i = 0; i < surface->format->BytesPerPixel; ++i) {
                std::swap(*pixel1++, *pixel2++);
            }
        }
    }

    SDL_UnlockSurface(surface);
}

Texture::Texture(const std::string_view path)
{
    MaterialSystem::GetContext(m_matRenderContext);
    TEXTURE_HANDLE texture = m_matRenderContext->GenerateTexture();
    m_matRenderContext->BindTexture(texture);
    m_matRenderContext->SetTextureParameter(TextureParameters::WRAP_S, TextureSetValues::WRAP_REPEAT);
    m_matRenderContext->SetTextureParameter(TextureParameters::WRAP_T, TextureSetValues::WRAP_REPEAT);
    m_matRenderContext->SetTextureParameter(TextureParameters::MIN_FILTER,
                                            TextureSetValues::FILTER_LINEAR_MIPMAP_LINEAR);
    m_matRenderContext->SetTextureParameter(TextureParameters::MAG_FILTER, TextureSetValues::FILTER_LINEAR);
    m_matRenderContext->SetTextureParameter(TextureParameters::MAX_ANISOTROPHY, TextureSetValues::ANISOTROPHY_X8);

    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> image{IMG_Load(path.data()), &SDL_FreeSurface};
    if (image) {
        FlipSurface(image.get(), false, true);
        SDL_Log("Loaded image: %s", path.data());
        m_matRenderContext->WriteTextureToActive(image->w, image->h, image->pixels);
        m_matRenderContext->GenerateMipmaps();
    }

    this->m_texture.reset(new TEXTURE_HANDLE(texture), Texture::DestructTexture);
}

void Texture::Use(size_t slot) const
{
    m_matRenderContext->SetActiveTextureSlot(slot);
    m_matRenderContext->BindTexture(*m_texture);
}
