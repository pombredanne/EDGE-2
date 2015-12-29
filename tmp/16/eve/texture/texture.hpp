#pragma once

#include <GL/glew.h>

#include <spot/spot.hpp>

namespace eve
{
    class texture : public spot::texture
    {
        public:

        texture() : spot::texture()
        {}

        template<typename T>
        texture( const T &t ) : spot::texture(t)
        {}

        texture( size_t w, size_t h ) : spot::texture(w,h)
        {}

        ~texture() {
        	if( destroy_on_exit ) destroy();
        }

        bool destroy_on_exit = false;
        //size_t iw = 0, ih = 0; // image width, height
        float u0 = 0, v0 = 0, u1 = 1, v1 = 1;

        void create();
        void destroy();

        void bind( int unit = 0 ) const;      // apply()
        void unbind() const;                  // clear()

        void filtering( int filt_min = GL_LINEAR_MIPMAP_NEAREST, int filt_mag = GL_NEAREST, int wrap_s = GL_REPEAT, int wrap_t = GL_REPEAT, int wrap_r = GL_REPEAT );
        void commit( int format = GL_RGBA, bool build_mip_maps = true );

        void upload();
        void ready(); // to deprecate

        void sprite( float scale = 1.f ) const;
    };

    template< typename TYPE >
    class texturemap : public std::map< TYPE, texture >
    {
        typename std::map< TYPE, texture >::iterator cursor;

        public:

        texturemap() {
            cursor = this->end();
        }

        inline bool find( const TYPE &refkey ) {
            cursor = this->std::map< TYPE, texture >::find( refkey );
            return cursor != this->end();
        }

        inline const texture &found() const {
            return cursor->second;
        }

        inline texture &found() {
            return cursor->second;
        }

        inline texture &insert( const TYPE &refkey ) {
            return ( this->operator[]( refkey ) = this->operator[]( refkey ) );
        }

        inline texture &insert( const TYPE &refkey, const texture &t ) {
            return ( ( this->operator[]( refkey ) = this->operator[]( refkey ) ) = t );
        }

        void delegate() {
            for( auto &it : *this ) {
                it.delegate();
            }
        }
    };
}

namespace eve {
    using pixel = spot::pixel;
    using image = spot::image;
    using color = spot::color;

    extern pixel (&rgba)(float r, float g, float b, float a );
    extern color (&hsla)(float r, float g, float b, float a );
}
