#include "texture.hpp"

class NullTexture : public Texture {
public:
    NullTexture() {}
};

static NullTexture nullTexture;

TextureRef Texture::NullTexture = &nullTexture;
