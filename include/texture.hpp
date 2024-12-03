#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <memory>
#include <string_view>

#include "SDL_surface.h"
#include "shader.hpp"

class Texture final
{
    std::shared_ptr<u32> texture{nullptr};
    static void          destruct_texture(u32 *texture)
    {
        glDeleteTextures(1, texture);
    }

    void flip_surface(SDL_Surface *surface, bool horizontal, bool vertical);

  public:
    Texture() = delete;
    Texture(const std::string_view path);
    ~Texture() = default;

    void use(GLenum texture_slot);
};

#endif
