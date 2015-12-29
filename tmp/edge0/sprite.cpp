#include "sprite.hpp"
#include "renderer.hpp"

Sprite::Sprite(TextureRef texture)
    : texture(texture),
    size(texture->size().as<float>()),
    angle(0.0f),
    frame(0),
    has_frames(false)
{}

void Sprite::render(ee::renderer& rend) {
    bool has_frames = (this->size != size);
    if (has_frames) {
        Recti src = get_frame_rect(frame);
        Recti dst = Rectf((position - center), size).as<int>();
        rend.draw_texture(texture, src, dst, angle);
    } else {
        rend.draw_texture(texture, (position - center).as<int>(), angle);
    }
}

Recti Sprite::get_frame_rect(int frame) {
    int pos_x, pos_y, frame_y;
    pos_x = frame * size.x;
    frame_y = pos_x / texture->width();
    pos_x %= texture->width();
    pos_y = frame_y * size.y;
    if ((pos_y + size.y) > texture->height()) {
        return Recti(size.as<int>());
    }
    return Recti(pos_x, pos_y, size.x, size.y);
}

void Sprite::translate_by(const vec2f &adjust_position) {
    position += adjust_position;
}

void Sprite::rotate_by(float angle) { 
    this->angle += angle;
}
