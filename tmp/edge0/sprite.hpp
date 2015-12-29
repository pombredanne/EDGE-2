#ifndef EE_SPRITE_HPP
#define EE_SPRITE_HPP

#include "texture.hpp"
#include "renderer.hpp"

#include "vec2.hpp"
#include "rect.hpp"

struct Sprite {
    TextureRef texture;
    vec2f position;
    vec2f size;
    vec2f center;
    float angle;
    int frame;
    bool has_frames;

    Sprite(TextureRef texture);
    void render(ee::renderer& rend);

    void translate_by(const vec2f& adjust_position);
    void rotate_by(float angle);

    Recti get_frame_rect(int frame);

    Rectf bounds() const {
        return Rectf(position - center, size);
    }
};

template<typename T>
inline void render( ee::renderer &renderer, const T &renderable ) {
    renderable.render( renderer );
}

template<typename T>
inline void render_serial( ee::renderer &renderer, const T &collection ) {
    for( auto &item : collection ) {
        item.render( renderer );
    }
}

template<typename T>
inline void render_assoc( ee::renderer &renderer, const T &collection ) {
    for( auto &item : collection ) {
        item.second.render( renderer );
    }
}

#endif
