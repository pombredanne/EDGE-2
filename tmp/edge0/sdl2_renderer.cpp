#include "sdl2_renderer.hpp"
#include "sdl2_texture.hpp"
#include "vec2.hpp"

sdl2_renderer::sdl2_renderer(SDL_Window* win, SDL_Renderer* rend) : m_win(win), m_rend(rend) {
    update_window_size();
}

sdl2_renderer::~sdl2_renderer() {
    for (auto it = m_textureCache.begin(); it != m_textureCache.end(); ++it) {
        delete it->second;
    }
    m_textureCache.clear();
}

void sdl2_renderer::update_window_size() {
    SDL_GetWindowSize(m_win, &m_size.x, &m_size.y);
    if (!m_logical_set) {
        m_logical_size = m_size;
    }
}

void sdl2_renderer::set_logical_size(vec2i size, bool keepAspect) {
    m_logical_set = true;
    m_logical_size = size;
    SDL_RenderSetLogicalSize(m_rend, size.x, size.y);
}

void sdl2_renderer::set_swap_interval(int swap) {
#ifndef EMSCRIPTEN
    int ret = SDL_GL_SetSwapInterval(swap);
    if (swap == -1 && ret != 0) {
        SDL_GL_SetSwapInterval(1);
    }
#endif
}

TextureRef sdl2_renderer::load_texture(const std::string &name) {
    auto it = m_textureCache.find(name);
    if (it != m_textureCache.end()) {
        return it->second;
    }
    std::string asset_file = name; //AssetManager::DefaultManager().find_resource(name, "png");
    if (!asset_file.empty()) {
        TextureRef tex = new sdl2_texture(*this, asset_file);
        m_textureCache.insert(std::make_pair(name, tex));
        return tex;
    }
    return Texture::NullTexture;
}

void sdl2_renderer::draw_texture(TextureRef texture, const Recti& src, const Recti& dst, float angle) {
    sdl2_texture* tex = dynamic_cast<sdl2_texture*>(texture);
    if (tex) {
        SDL_Texture *stex = tex->m_texture;
        SDL_RenderCopyEx(m_rend, stex, (const SDL_Rect*)&src, (const SDL_Rect*)&dst, angle, NULL, SDL_FLIP_NONE);
    }
}

void sdl2_renderer::clear(const Color& color) {
    set_draw_color(color);
    SDL_RenderClear(m_rend);
}

void sdl2_renderer::draw_rect_fill(const Recti& r, const Color& c) {
    set_draw_color(c);
    SDL_RenderFillRect(m_rend, (const SDL_Rect*)&r);
}

void sdl2_renderer::draw_rect(short x, short y, short w, short h, int color) {
    set_draw_color(color);
    SDL_Rect rect = { (Sint16)x, (Sint16)y, (Uint16)w, (Uint16)h };
    SDL_RenderFillRect(m_rend, &rect);
}

void sdl2_renderer::present() {
    SDL_RenderPresent(m_rend);
}

void sdl2_renderer::set_draw_color(const Color& c) {
    SDL_SetRenderDrawColor(m_rend, c.r, c.g, c.b, c.a);
}
