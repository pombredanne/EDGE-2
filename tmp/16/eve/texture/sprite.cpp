// @todo: triparticle, biparticle, particle
// - rlyeh ~~ listening to Toundra - Requiem

//#include "eve.hpp"
//#include "render.hpp"
//#include "image.hpp"
#include <iostream>
#include <eve/gl/enable.hpp>
#include <eve/style.hpp>
#include <eve/matrix.hpp>
#include "texture.hpp"
#include "sprite.hpp"
#include <geo/plane.hpp>
#include <eve/geometry.hpp>

namespace eve
{
    void render( sprite_ &spr )
    {
        //xyz
    //        eve::matrix::translate t( spr.x.position / scale, spr.y.position / scale, 0 );
        //xzy

            eve::matrix::translate t( spr.position.x, 0, spr.position.y );

            eve::disable::culling both_faces;
            eve::enable::alpha hard_alpha;

            eve::enable::billboard bb;
            eve::matrix::rotate_x rx(90);

            eve::matrix::scale sc( ( spr.mirror.x ? -1 : +1 ) * spr.scale.x, 1.f, ( spr.mirror.y ? -1 : +1 ) * spr.scale.y );

            eve::style::texture tx( spr.diffuse.id );
            eve::geometry::plane floor( 1.f );
    }

    void render2d( sprite_ &spr, float gscale )
    {
        eve::disable::culling both_faces;
        eve::enable::alpha hard_alpha;

        {
            eve::style::texture tx( spr.diffuse.id );

            // @todo: optimize this {

            float px = spr.position.x;
            float py = spr.position.y;
            float iw = spr.diffuse.iw;
            float ih = spr.diffuse.ih;
            float scx = spr.scale.x;
            float scy = spr.scale.y;
            float ax = spr.anchor.x;
            float ay = spr.anchor.y;
            float mx = spr.mirror.x;
            float my = spr.mirror.y;

            float sscx = gscale * scx * iw;
            float sscy = gscale * scy * ih;

            // translate to position plus anchors
            eve::matrix::translate position( ( px / iw + ( ax - 1 ) / iw) * sscx, ( py / ih + ( ay - 1 ) / ih ) * sscy, 0 );
            // scale to image size (!= texture size)
            eve::matrix::scale image( mx ? -sscx : sscx, my ? -sscy : sscy, 1 );

            // }

            glBegin(GL_QUADS);
                glTexCoord2f( spr.diffuse.u0, spr.diffuse.v0 );
                glVertex2f(0, 0.f);
                glTexCoord2f( spr.diffuse.u0, spr.diffuse.v1 );
                glVertex2f(0.f, 1.f);
                glTexCoord2f( spr.diffuse.u1, spr.diffuse.v1 );
                glVertex2f(1.f, 1.f);
                glTexCoord2f( spr.diffuse.u1, spr.diffuse.v0 );
                glVertex2f(1.f, 0.f);
            glEnd();
        }
    }
}

namespace eve
{
    static eve::texture error_texture;

    bool is_error_texture( const eve::texture *t )
    {
        return t->id == error_texture.id;
    }

    eve::texture &get_error_texture()
    {
        if( !error_texture.w )
        {
            error_texture = eve::texture( 32, 32 );

            for( int y = 0; y < 32; y++ )
            for( int x = (y < 16 ? 0 : 16), end = (y < 16 ? 16 : 32); x < end; x++ )
                error_texture.at( x, y ) = eve::rgba( 255, 255, 0, 255 );

            error_texture.ready();
        }

        return error_texture;
    }

    eve::texture &get_texture( const std::string &texture_name )
    {
        // @fixme GIF texture loading way too slow

        static eve::texturemap<std::string> map;

        if( map.find(texture_name) )
            return map.found();

        eve::texture &tx = map.insert( texture_name, eve::texture( texture_name ) );

        if( tx.w > 0 )
        {
            std::cout << "Texture '" << texture_name << "' submitted!" << std::endl;
            tx.ready();
        }
        else
        {
            tx.id = get_error_texture().id;
        }

        return tx;
    }

#if 0
    // point sprites, issues:
    // setting sprite size
    // sprites get culled as soon as center of sprite is out of viewport
    // wont rotate (to verify)
    void sprite( const std::string &texture_name, float texture_scale = 1.f, float unused = 1.f )
    {
        eve::texture &texture = get_texture( texture_name );
        texture.bind();

        eve::enable::alpha hard_alpha;

            float quadratic[] =  { 0.0f, 0.0f, 0.01f };
            glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
            float maxSize = 0.0f;
            glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );
            glPointSize( maxSize );
            glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, maxSize );
            glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
            glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
            glEnable( GL_POINT_SPRITE_ARB );
            glBegin( GL_POINTS );
                    //glColor4f(VecParticle[i].Color.x,VecParticle[i].Color.y,VecParticle[i].Color.z,1.0f);
                    //glVertex3f(VecParticle[i].Position.x,VecParticle[i].Position.y,VecParticle[i].Position.z);
                    glColor4f(1,1,1,1);
                    glVertex3f(0,0,0);
            glEnd();
            glDisable( GL_POINT_SPRITE_ARB );

        texture.unbind();
    }
#else

    void texturedquad( bool mirror_w, bool mirror_h, float scale_w, float scale_h, float texture_tiling )
    {
            // opengl < 1.5
            eve::enable::billboard bd;
            eve::matrix::rotate_x rx(90.f);
//            eve::matrix::scale sc( texture_scale );

            eve::disable::culling both_faces;

            float tw = scale_w, itw = ( mirror_w ? -1.f : +1.f ) / tw;
            float th = scale_h, ith = ( mirror_h ? -1.f : +1.f ) / th;

            glScalef( tw, 1, th );
            eve::geometry::plane floor( texture_tiling );
            glScalef( itw, 1, ith );
    }

    void spritebox( const eve::texture &texture, bool mirror_w, bool mirror_h, float texture_scale_w, float texture_scale_h, float texture_tiling )
    {
#if 1
        //eve::enable::alpha hard_alpha;

        texture.bind();

        texturedquad( mirror_w, mirror_h, texture_scale_w, texture_scale_h, texture_tiling );

        texture.unbind();
#else
        eve::texture &tx = get_texture( texture_name );

        eve::disable::lighting nolights;
        eve::disable::depth nodepth;

        eve::matrix::scale sc( tx.w * 0.01f, tx.h * 0.01f, 0.1f );

        eve::style::red color;
        eve::style::wireframe wf;

        eve::cube cb;
#endif
    }

    void sprite( const eve::texture &texture, bool mirror_w, bool mirror_h, float texture_scale_w, float texture_scale_h, float texture_tiling )
    {
        eve::enable::alpha hard_alpha;

        texture.bind();

        texturedquad( mirror_w, mirror_h, texture_scale_w, texture_scale_h, texture_tiling );

        texture.unbind();
    }

    void sprite( const std::string &texture_name, bool mirror_w, bool mirror_h, float texture_scale_w, float texture_scale_h, float texture_tiling )
    {
        sprite( get_texture( texture_name ), mirror_w, mirror_h, texture_scale_w, texture_scale_h, texture_tiling );
    }

    void spritebox( const std::string &texture_name, bool mirror_w, bool mirror_h, float texture_scale_w, float texture_scale_h, float texture_tiling )
    {
        spritebox( get_texture( texture_name ), mirror_w, mirror_h, texture_scale_w, texture_scale_h, texture_tiling );
    }

#endif
}

