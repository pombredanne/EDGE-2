#pragma once

#include "texture.hpp"

namespace eve
{
    bool is_error_texture( const eve::texture *t );

    eve::texture &get_error_texture();

    eve::texture &get_texture( const std::string &texture_name );

    void texturedquad( bool mirror_w, bool mirror_h, float scale_w = 1.f, float scale_h = 1.f, float texture_tiling = 1.f );

    void sprite( const eve::texture &texture, bool mirror_w = false, bool mirror_h = false, float texture_scale_w = 1.f, float texture_scale_h = 1.f, float texture_tiling = 1.f );
    void spritebox( const eve::texture &texture, bool mirror_w = false, bool mirror_h = false, float texture_scale_w = 1.f, float texture_scale_h = 1.f, float texture_tiling = 1.f );
/*
    void sprite( const std::string &texture_name, bool mirror_w = false, bool mirror_h = false, float texture_scale_w = 1.f, float texture_scale_h = 1.f, float texture_tiling = 1.f );
    void spritebox( const std::string &texture_name, bool mirror_w = false, bool mirror_h = false, float texture_scale_w = 1.f, float texture_scale_h = 1.f, float texture_tiling = 1.f );
    */
}

//#include <eve/play/scene.hpp>
//tree<sprite> spritetree;

namespace eve
{
    struct sprite_ // sprite!
    {
        // material, bucket
        struct texture
        {
            int id = 0;
            int unit = 0;
            float u0 = 0, v0 = 0;
            float u1 = 1, v1 = 1;
            float  w = 1,  h = 1;
            float iw = 1, ih = 1;
            bool enabled = 0;
        } diffuse, normal, opacity, specular, glow;

        struct {   float x = 0, y = 0; } position;
        struct {   float x = 1, y = 1; } face;
        struct {   float x = 1, y = 1; } scale;
        struct {   float x = 1, y = 1; } tile;
        struct {    bool x = 0, y = 0; } mirror;
        struct {   float x = 0, y = 0; } anchor; // defaults to center (-1,1: left-top; 0,0 = center-middle; 1,-1 = right-bottom)
        struct {   float x = 0, y = 0, angle = 0; } pivot;

        struct {   float x = 1, y = 1; } size;

        // transform, transformation
        // link, unlink
        // sphere, aabb, obb
    };

    void render( sprite_ &spr );
    void render2d( sprite_ &spr, float scale = 1 );

    // sprite3d { dims }
}

