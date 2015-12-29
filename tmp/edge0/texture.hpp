#ifndef EE_TEXTURE_HPP
#define EE_TEXTURE_HPP

#include "vec2.hpp"

class Texture;
typedef Texture* TextureRef;

class Texture {
protected:
    vec2i m_size;

    Texture() {}
public:
    virtual ~Texture() {}

    int width() { return m_size.x; }
    int height() { return m_size.y; }
    const vec2i& size() { return m_size; }

    static TextureRef NullTexture;
};

#endif
