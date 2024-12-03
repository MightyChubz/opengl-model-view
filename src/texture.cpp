#include <array>
#include <cstddef>
#include <memory>
#include <string_view>

#include "SDL_image.h"
#include "SDL_log.h"
#include "SDL_surface.h"
#include "shader.hpp"
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
            int    target_y = vertical ? height - 1 - y : y;
            int    target_x = horizontal ? width - 1 - x : x;

            Uint8 *pixel1   = pixels + y * pitch + x * surface->format->BytesPerPixel;
            Uint8 *pixel2   = pixels + target_y * pitch + target_x * surface->format->BytesPerPixel;
            for (int i = 0; i < surface->format->BytesPerPixel; ++i) {
                std::swap(*pixel1++, *pixel2++);
            }
        }
    }
}

Texture::Texture(const std::string_view path)
{
    u32 texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> image{IMG_Load(path.data()), &SDL_FreeSurface};
    if (image) {
        flip_surface(image.get(), false, true);
        SDL_Log("Loaded image: %s", path.data());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    this->texture.reset(new u32(texture), Texture::destruct_texture);
}

void Texture::use(size_t slot)
{
    std::array texture_slots = {
        GL_TEXTURE0,  GL_TEXTURE1,  GL_TEXTURE2,  GL_TEXTURE3,  GL_TEXTURE4,  GL_TEXTURE5,  GL_TEXTURE6,  GL_TEXTURE7,
        GL_TEXTURE8,  GL_TEXTURE9,  GL_TEXTURE10, GL_TEXTURE11, GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14, GL_TEXTURE15,
        GL_TEXTURE16, GL_TEXTURE17, GL_TEXTURE18, GL_TEXTURE19, GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23,
        GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29, GL_TEXTURE30, GL_TEXTURE31,
    };
    glActiveTexture(texture_slots[slot]);
    glBindTexture(GL_TEXTURE_2D, *texture);
}
