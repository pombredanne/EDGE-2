#include "texture.hpp"
#include <GL/glu.h>
#include <cassert>

#if 0

#include "gl/gl.hpp"

void setup_filtering( bool high_quality )
{
#if 0

    // Nearest Filtered Texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(...)

    // Linear Filtered Texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(...)

    // MipMapped Texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(...)

#endif

    /* blurry/blocky

    ref: http://gregs-blog.com/2008/01/17/opengl-texture-filter-parameters-explained/
    MAG_FILTER/MIN_FILTER                 Bilinear Filtering          Mipmapping
                                            Near   Far
    GL_NEAREST / GL_NEAREST_MIPMAP_NEAREST  Off    Off                Standard
    GL_NEAREST / GL_LINEAR_MIPMAP_NEAREST   Off    On                 Standard (* chars)
    GL_NEAREST / GL_NEAREST_MIPMAP_LINEAR   Off    Off                Use trilinear filtering
    GL_NEAREST / GL_LINEAR_MIPMAP_LINEAR    Off    On                 Use trilinear filtering (* chars)
    GL_NEAREST / GL_NEAREST                 Off    Off                None
    GL_NEAREST / GL_LINEAR                  Off    On                 None (* scene)
    GL_LINEAR / GL_NEAREST_MIPMAP_NEAREST   On     Off                Standard
    GL_LINEAR / GL_LINEAR_MIPMAP_NEAREST    On     On                 Standard
    GL_LINEAR / GL_NEAREST_MIPMAP_LINEAR    On     Off                Use trilinear filtering
    GL_LINEAR / GL_LINEAR_MIPMAP_LINEAR     On     On                 Use trilinear filtering
    GL_LINEAR / GL_NEAREST                  On     Off                None
    GL_LINEAR / GL_LINEAR                   On     On                 None

    (*) intended for prjx

    @todo
    enum { near_blurry, near_blocky }
    enum { far_blurry, far_blocky }
    enum { mipmap_none, mipmap_std, mipmap_trilinear };
    mask -> near_blocky | far_blurry | mipmap_std

    */

    if( high_quality )
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

#endif

namespace eve {

void texture::create()
{
    if( id == 0 )
    {
        GLuint uid;
        glGenTextures( 1, &uid );
        id = (unsigned)(uid);
    }
}

void texture::destroy()
{
    // if texture is valid, try to delete it
    if( id )
    {
        GLuint uid = id;
        // if we own texture, delete it
        glDeleteTextures( 1, &uid );
        id = 0;
    }
}

void texture::bind( int unit ) const
{
    assert( id );
    if( id )
    {
        GLuint uid = id;
        glEnable( GL_TEXTURE_2D );
        switch( unit )
        {
            case 0: glActiveTexture( GL_TEXTURE0 ); break;
            case 1: glActiveTexture( GL_TEXTURE1 ); break;
            case 2: glActiveTexture( GL_TEXTURE2 ); break;
            case 3: glActiveTexture( GL_TEXTURE3 ); break;
            case 4: glActiveTexture( GL_TEXTURE4 ); break;
            case 5: glActiveTexture( GL_TEXTURE5 ); break;
            case 6: glActiveTexture( GL_TEXTURE6 ); break;
            case 7: glActiveTexture( GL_TEXTURE7 ); break;
            default: break;
        }
        glBindTexture( GL_TEXTURE_2D, uid );
    }
}

void texture::unbind() const
{
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable( GL_TEXTURE_2D );
}

#if 0
bool texture::scale( const unsigned int *pic, unsigned width, unsigned height, bool mirror_w, bool mirror_h )
{
    if( width == 0 && height == 0 ) {
        SAVE_PROPS();
        *this = texture( width, height );
        LOAD_PROPS();
        return true;
    }

    if( !(pic && width && height) ) {
        return false;
    }

    /*
    // @todo :(
    image p = pic;
    if( mirror_w ) {
        p = p.flip_w();
    }
    if( mirror_h ) {
        p = p.flip_h();
    }
    */

    // reset
    w = width, h = height;
    bool is_power_of_two_w = w && !(w & (w - 1));
    bool is_power_of_two_h = h && !(h & (h - 1));

    if( is_power_of_two_w && is_power_of_two_h )
    {
        SAVE_PROPS();
        *this = texture( width, height ); // slow!, try resize(w*h) && setup member vars instead
        LOAD_PROPS();

        memcpy( this->data(), pic, width * height * 4 );
    }
    else
    {
        size_t nw = 1, nh = 1, atx, aty;
        while( nw < w ) nw <<= 1;
        while( nh < h ) nh <<= 1;

        // squared as well, cos we want them texel perfect
        if( nh > nw ) nw = nh; else nh = nw;

        atx = (nw - w) / 2;
        aty = (nh - h) / 2;

        size_t pw = w, ph = h;

        SAVE_PROPS();
        *this = texture( nw, nh, texel::rgba() ); // slow!, try resize(w*h) && setup member vars instead
        LOAD_PROPS();

        this->u0 = float(atx) / nw;
        this->v0 = float(aty) / nh;
        this->u1 = float(atx+pw) / nw;
        this->v1 = float(aty+ph) / nh;
        this->iw = pw;
        this->ih = ph;

        for( size_t y = 0; y < ph; ++y ) {
            for( size_t x = 0; x < pw; ++x ) {
                this->at( atx + x, aty + y ).pack = pic[ x + y * pw ]; // *pic++
            }
        }
    }

    return true;
}
#endif

void texture::filtering( int min, int mag, int wrap_s, int wrap_t, int wrap_r )
{
    if( min )
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min );

    if( mag )
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag );

    if( wrap_s )
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s );

    if( wrap_t )
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t );

    if( wrap_r )
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrap_r );
}

void texture::commit( int format, bool build_mip_maps )
{
    std::vector<unsigned char> texels = ( format == GL_RGBA /* || format == GL_BGRA */ ? rgba() : rgb() );

    // Set unpack alignment to one byte

    GLint   UnpackAlignment;
    glGetIntegerv( GL_UNPACK_ALIGNMENT, &UnpackAlignment );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    // Submit decoded data to texture
    GLint ret;
    if( build_mip_maps ) {
        ret = gluBuild2DMipmaps(GL_TEXTURE_2D,
            format,
            w,
            h,
            format,
            GL_UNSIGNED_BYTE,
            texels.data() );
    }
    else {
        glTexImage2D(GL_TEXTURE_2D,
        0 /*LOD*/,
        format,
        w,
        h,
        0 /*border*/,
        format,
        GL_UNSIGNED_BYTE,
        texels.data() );
    }

    // Restore old unpack alignment
    glPixelStorei( GL_UNPACK_ALIGNMENT, UnpackAlignment );
}

void texture::upload() {
    if( !id ) {
        create();
        bind();
        filtering();
        commit();
    }
}

void texture::ready() {
    upload();
}

#if 0
void texture::sprite( /*const eve::vec2 &pos,*/ float scale ) const
{
    eve::disable::lighting dl;
    eve::matrix::ortho flat; // 0,0 is top-left

    //eve::matrix::translate t( pos );
    //glRasterPos2f

    eve::style::texture tx( id );
    eve::matrix::scale sc( eve::vec3( w * scale, h * scale, 1 ) );
    eve::quad sprite(1.f);
}
#endif

pixel (&rgba)(float r, float g, float b, float a = 255) = spot::rgba;
color (&hsla)(float r, float g, float b, float a = 1.f) = spot::hsla;

}
