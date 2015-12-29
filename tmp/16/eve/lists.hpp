#pragma once

#include <cassert>

#include <iostream>
#include <string>
#include <map>

#include "gl/gl.hpp"
#include "gl/nocopy.hpp"

namespace eve
{
    namespace lists
    {
        #define EVE_COMPILE_LIST eve::compile __comp__ = eve::compile(__FILE__,__LINE__)

        class compile
        {
            /* usage:

                if( eve::compile comp = eve::compile("sphere1") )
                {
                    eve::matrix::position p( vec3(1, 10, 0) );
                    eve::sphere sph;
                }

                or

                if( eve::compile comp = eve::compile(__FILE__,__LINE__) )
                {
                    eve::matrix::position p( vec3(1, 10, 0) );
                    eve::sphere sph;
                }

                or

                if( EVE_COMPILE_LIST )
                {
                    ...
                }

            */

            const char *file;
            int line;
            bool compiled;
            void go( bool construct )
            {
                typedef std::pair<std::string,int> key_t;
                typedef std::map<key_t, int> map_t;

                static map_t map;
                key_t id = key_t( file, line );
                map_t::iterator it = map.find( id );

                if( construct )
                {
                    compiled = ( it != map.end() );
                    if( !compiled )
                    {
                        static int list = 0;
                        list++;
                        map.insert( std::pair<key_t,int>( id, list) );
                        glNewList( list, GL_COMPILE );
                    }
                }
                else
                {
                    int list = it->second;
                    if( !compiled )
                    {
                        glEndList();
                        //std::cout << "compiled list #" << list << std::endl;
                    }
                    //std::cout << "calling list #" << list << std::endl;
                    glCallList( list );
                }
            }

            public:

            explicit
            compile( const char *_id ) : file(_id), line(0), compiled(false)
            {
                go( true );
            }

            explicit
            compile( const char *_file, int _line ) : file(_file), line(_line), compiled(false)
            {
                go( true );
            }

            ~compile()
            {
                go( false );
            }

            operator const bool() const
            { return !compiled; }
        };

        struct check : nocopy
        {
            check( const char *_file, int _line ) : file(_file), line(_line)
            {
                if( glGetError() != GL_NO_ERROR )
                {
                    std::cerr << gluErrorString( glGetError() ) << std::endl;
                    assert( !"OpenGL Error" );
                }
            }

            ~check()
            {
                if( glGetError() != GL_NO_ERROR )
                {
                    std::cerr << gluErrorString( glGetError() ) << std::endl;
                    assert( !"OpenGL Error" );
                }
            }

            private:

            const char *file;
            int line;
        };
    }
}

namespace eve
{
    using namespace lists;
}
