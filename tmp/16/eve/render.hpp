// (Not so) simple RAII render, wip
// -rlyeh

// prio #1
// use pushattrib y pushmatrix, etc para restaurar donde sea posible
// un push/pop viewport tb

// prio #2
// todo!! split! ie, eve::2d::raster and eve::3d::translate
// split visually whats 2d and 3d commands. i spent too much time trying to do an matrix::ortho and render::cube!
// ortho is only for 2d bitmap operations, and cube is a 3d thing. argh. its not clear enough

// todo: this file should be an interface (w/ pImpl maybe) and fully agnostic {

// proposal 2d coord axis: -1,-1 <-> 1,1 safe margins; >1 || <-1 visibility not guaranteed
// -1,-1   |
//         |0,0
// --------+----------
//         |
//         |       1,1

//www.eng.cam.ac.uk/help/tpl/languages/C++/namespaces.html
//http://winterdom.com/dev/cpp/nspaces
//shivavg (opengl), maestrovg(sw)
//http://sourceforge.net/projects/shivavg/
//http://code.google.com/p/maestrovg/

// to check:
// http://altdevblogaday.org/2011/06/23/improving-opengl-error-messages/
// https://github.com/harthur/color



#pragma once

#include "gl/gl.hpp"
//#include <GL/freeglut.h>

#include <vector>
#include <deque>
#include <map>
#include <string>
#include <cassert>
#include <iostream>
#include <mutex>

//#include <eve/os/mutex.hpp>
//#include <eve/string/format.hpp>

#include "gl/shader.hpp"
//#include "texture/texture.hpp"

#include <geo/vec.hpp>
#include <geo/spline.hpp>

//#include "texture/texture.hpp"

#include "matrix.hpp"
#include "gl/invert.hpp"
#include "gl/disable.hpp"
#include "gl/enable.hpp"
#include "style.hpp"
#include "geometry.hpp"
#include "lists.hpp"
#include "gl/nocopy.hpp"




namespace eve
{
    class capturer;

    class surface
    {
        // @todo: fix top horiz line glitch & scale (half?) bug

        std::vector<unsigned char> colorBits;
        const int w, h; //size of texture
        const bool capture_depth, sharp; //sharp/blocky or blurry

        unsigned int id;

        int viewport[4];

        public:

        surface( int _w = 256, int _h = 256 ) : w( _w ), h( _h ), capture_depth( false ), sharp( true ), id( 0 )
        {
            //new array
            colorBits.resize( w * h * 3 );

            //texture creation..
            glGenTextures( 1, &id );
            glBindTexture( GL_TEXTURE_2D, id );

            if(!capture_depth)
                glTexImage2D( GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, colorBits.data() );
            else
                glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0 );

            //you can set other texture parameters if you want
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        }

        ~surface()
        {
            if( id )
            {
                GLuint uid = id;
                glDeleteTextures( 1, &uid );
            }
        }

        void capture_begin() //capture @rect(x,y)!! //void predraw() || operator bool?
        {
            //save viewport and set up new one
            glGetIntegerv( GL_VIEWPORT, (int*)viewport );
            glViewport( 0, 0, w, h );
        }

        void capture_end() //void postdraw()
        {
            //save data to texture using glCopyTexImage2D
            glBindTexture( GL_TEXTURE_2D, id );

            if( !capture_depth )
                glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 0,0, w, h, 0 );
            else
                glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, w, h );

            //restore viewport
            glViewport( viewport[0], viewport[1], viewport[2], viewport[3] );
        }

        void display( const eve::vec2 &pos = eve::vec2(0,0), float scale = 1.f ) //x,y
        {
            eve::disable::lighting dl;

            // locate (0,0 is top-left if using default viewport)
            eve::matrix::ortho flat;
            eve::matrix::translate tr( eve::vec3(pos.x,pos.y,0) );
            eve::matrix::scale sc( eve::vec3( w * scale, h * scale, scale ) );

            // draw center of quad at given position
            eve::style::texture tx( id );
            eve::geometry::quad sprite( 1.f );
        }
    };

    class capturer : nocopy
    {
        surface &sf;

        public:

        explicit capturer( surface &_surface ) : sf(_surface)
        {
            sf.capture_begin();
        }

        ~capturer()
        {
            sf.capture_end();
        }
    };

    // DEPTH TO TEXTURE
    /*

    int shadowMapWidth = 512;
    int shadowMapHeight = 512;
    glGenTextures(1, &m_depthTexture);

    glBindTexture(GL_TEXTURE_2D, m_depthTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 512,512);

    */
}

#include "render-vertex-array.inl"
