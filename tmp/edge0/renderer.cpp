#include "renderer.hpp"

void ee::renderer::draw_texture(TextureRef texture, const vec2i &dst_pos, float angle)
{
    const vec2i& size = texture->size();
    Recti src(size);
    Recti dst(dst_pos, size);

    draw_texture(texture, src, dst, angle);
}