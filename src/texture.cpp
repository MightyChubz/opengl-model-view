#include <array>
#include <cstddef>
#include <memory>
#include <string_view>

#include "SDL_image.h"
#include "SDL_log.h"
#include "SDL_surface.h"
#include "material_system.hpp"
#include "stddefs.hpp"
#include "texture.hpp"

void Texture::flip_surface(SDL_Surface *surface, bool horizontal, bool vertical)
{
    if (!surface) return;

    SDL_LockSurface(surface);

    int    pitch  = surface->pitch;
    int    height = surface->h;
    int    width  = surface->w;
    Uint8 *pixels = static_cast<Uint8 *>(surface->pixels);

    for (int y = 0; y < height >> 1; ++y) {
        for (int x = 0; x < width; ++x) {
            int target_y = vertical ? height - 1 - y : y;
            int target_x = horizontal ? width - 1 - x : x;

            Uint8 *pixel1 = pixels + y * pitch + x * surface->format->BytesPerPixel;
            Uint8 *pixel2 = pixels + target_y * pitch + target_x * surface->format->BytesPerPixel;
            for (int i = 0; i < surface->format->BytesPerPixel; ++i) {
                std::swap(*pixel1++, *pixel2++);
            }
        }
    }
}

Texture::Texture(const std::string_view path)
{
    MaterialSystem::get_context(mat_render_context);
    TEXTURE_HANDLE texture = mat_render_context->generate_texture();
    mat_render_context->bind_texture(texture);
    mat_render_context->set_texture_parameter(TextureParameters::WRAP_S, TextureSetValues::REPEAT);
    mat_render_context->set_texture_parameter(TextureParameters::WRAP_T, TextureSetValues::REPEAT);
    mat_render_context->set_texture_parameter(TextureParameters::MIN_FILTER, TextureSetValues::LINEAR_MIPMAP);
    mat_render_context->set_texture_parameter(TextureParameters::MAG_FILTER, TextureSetValues::LINEAR);

    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> image{IMG_Load(path.data()), &SDL_FreeSurface};
    if (image) {
        flip_surface(image.get(), false, true);
        SDL_Log("Loaded image: %s", path.data());
        mat_render_context->write_texture_to_active(image->w, image->h, image->pixels);
        mat_render_context->generate_mipmaps();
    }

    this->texture.reset(new u32(texture), Texture::destruct_texture);
}

void Texture::use(size_t slot) const
{
    mat_render_context->set_active(slot);
    mat_render_context->bind_texture(*texture);
}
