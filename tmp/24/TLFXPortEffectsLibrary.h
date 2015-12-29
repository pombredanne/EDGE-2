#ifdef _MSC_VER
#pragma once
#endif

/*
 * TLFX Port IwGx for rendering
 * PugiXML for parsing data
 */

#ifndef _TLFXPORTEFFECTSLIBRARY_H
#define _TLFXPORTEFFECTSLIBRARY_H

#include <stdint.h>
#include "TLFXEffectsLibrary.h"
#include "TLFXParticleManager.h"
#include "TLFXAnimImage.h"

class CIwTexture;
class XMLLoader;

union CIwColour {
    struct { uint8_t r,g,b,a; };
    uint32_t rgba;
};

class TLFXPortEffectsLibrary : public TLFX::EffectsLibrary
{
public:
    virtual TLFX::XMLLoader* CreateLoader() const;
    virtual TLFX::AnimImage* CreateImage() const;
};

class TLFXPortParticleManager : public TLFX::ParticleManager
{
public:
    TLFXPortParticleManager(int particles = TLFX::ParticleManager::particleLimit, int layers = 1);
    void Flush();
protected:
    virtual void DrawSprite(TLFX::AnimImage* sprite, float px, float py, float frame, float x, float y, float rotation,
                            float scaleX, float scaleY, unsigned char r, unsigned char g, unsigned char b, float a, bool additive);

    // batching
    struct Batch
    {
        float px, py;
        float frame;
        float x, y;
        float rotation;
        float scaleX, scaleY;
        CIwColour color;
    };
    std::list<Batch> _batch;
    TLFX::AnimImage *_lastSprite;
    bool             _lastAdditive;
};

class TLFXPortImage : public TLFX::AnimImage
{
public:
    TLFXPortImage();
    ~TLFXPortImage();

    bool Load(const char *filename);
    CIwTexture* GetTexture() const;

protected:
    CIwTexture *_texture;
};

#endif // _TLFXPORTEFFECTSLIBRARY_H
