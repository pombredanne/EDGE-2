#ifndef EE_SDL2_TEXTURE_HPP
#define EE_SDL2_TEXTURE_HPP

#include "sdl2_renderer.hpp"

#include <SDL_render.h>

class sdl2_texture : public Texture {
    friend class sdl2_renderer;
    SDL_Texture *m_texture;
public:
    sdl2_texture(sdl2_renderer& rend, const std::string& filename);
    ~sdl2_texture();
};

#endif
