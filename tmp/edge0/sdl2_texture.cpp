#include "sdl2_texture.hpp"

#ifndef STBI_INCLUDE_STB_IMAGE_H
#include <stb_image.h>
#endif

SDL_Surface *SDL_LoadIMG(const char* filename)
{
    SDL_Surface *temp = NULL;
    unsigned char *data = NULL;
    int w, h, bpp;

    unsigned char *ptr = stbi_load(filename, &w, &h, &bpp, 4);
    if( ptr ) {
        temp = SDL_CreateRGBSurfaceFrom((void*)ptr, w, h, 32, 4 * w,
                                        0x000000ff,
                                        0x0000ff00,
                                        0x00ff0000,
                                        0xff000000);
        if (temp) {
            SDL_Surface *t2 = SDL_ConvertSurface(temp, temp->format, 0);
            SDL_FreeSurface(temp);
            temp = t2;
        }
        stbi_image_free(ptr);
    }

    return temp;
}

sdl2_texture::sdl2_texture(sdl2_renderer& rend, const std::string& filename) : m_texture(0) {
    SDL_Surface *surface = SDL_LoadIMG(filename.c_str());
    if (surface) {
        m_texture = SDL_CreateTextureFromSurface(rend.m_rend, surface);
        if (m_texture) {
            m_size.x = surface->w;
            m_size.y = surface->h;
        }
        SDL_FreeSurface(surface);
    }
}

sdl2_texture::~sdl2_texture() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}