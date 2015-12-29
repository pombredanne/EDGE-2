#ifndef EE_SDL2_RENDERER_HPP
#define EE_SDL2_RENDERER_HPP

#include "renderer.hpp"
#include "vec2.hpp"

#include <SDL.h>
#include <map>

struct sdl2_renderer : public ee::renderer {
protected:
    friend class sdl2_texture;

    SDL_Window* m_win;
    SDL_Renderer* m_rend;
    std::map<std::string, TextureRef> m_textureCache;
public:
    sdl2_renderer(SDL_Window* win, SDL_Renderer* rend);
    ~sdl2_renderer();

    void set_logical_size(vec2i size, bool keepAspect = true);

    void set_swap_interval(int swap);
    void update_window_size();
    void clear(const Color& c = Color());
    void draw_rect_fill(const Recti& r, const Color& c);
    void draw_rect(short x, short y, short w, short h, int color);

    void present();

    TextureRef load_texture(const std::string& name);
    void draw_texture(TextureRef texture, const Recti& src, const Recti& dst, float angle = 0.0f);
private:
    void set_draw_color(const Color& c);
};

#endif
