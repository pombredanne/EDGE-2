#include <eve/gl.hpp>
#include <eve/eve.hpp>
#include <string>
#include <stdlib.h>

namespace debugdraw
{
    class reticle
    {
        //info {
        struct info
        {
            // ~~ black sabbath / heaven and hell

            info()
            {}
            info( const std::string &_name, const float &_aspect_ratio, const bool &_safe_guide = false )
                : name(_name), aspect_ratio(_aspect_ratio), safe_guide(_safe_guide)
            {}

            void resize( const float &_w, const float &_h )
            {
                //std::cout << eve::echo( name, _w, _h );

                float xoff, yoff, w, h;
                float viewport_aspect_ratio = _w / _h;

                // if current viewport aspect_ratio is smaller than reticle aspect ratio
                if( viewport_aspect_ratio < aspect_ratio )
                {
                    //current viewport: 640 x 480 -> 1.33
                    //reticle: 1920 x 1080 style -> 1.78
                    w = _w;
                    h = _w / aspect_ratio;
                    xoff = 0;
                    yoff = ( _h - h ) * 0.5;
                }
                // else if current viewport aspect_ratio is bigger than reticle aspect ratio
                else
                if( viewport_aspect_ratio > aspect_ratio )
                {
                    //current viewport: 1920 x 1080 -> 1.78
                    //reticle: 640 x 480 style -> 1.33
                    w = _h * aspect_ratio;
                    h = _h;
                    xoff = ( _w - w ) * 0.5;
                    yoff = 0;
                }
                // else equal.
                else
                {
                    // be sure to make it visible
                    xoff = 1, yoff = 1, w = _w - 1, h = _h - 1;
                }

                //std::cout << eve::echo( name, xoff, yoff, w, h );

                if( safe_guide )
                {
                    // safe area = 15% smaller area
                    // make every side to pad 7.5%

                    geo::vec2 top_left( w * 0.075, h * 0.075 );
                    geo::vec2 bottom_right( w * 0.925, h * 0.925 );

                    guide.clear();
                    guide.push_back( geo::vec3( top_left.x, top_left.y, 0 ) );
                    guide.push_back( geo::vec3( bottom_right.x, top_left.y, 0 ) );
                    guide.push_back( geo::vec3( bottom_right.x, bottom_right.y, 0 ) );
                    guide.push_back( geo::vec3( top_left.x, bottom_right.y, 0 ) );
                    guide.push_back( geo::vec3( top_left.x, top_left.y, 0 ) );
                }
                else
                {
                    geo::vec2 top_left( xoff, yoff );
                    geo::vec2 bottom_right( xoff+w-1, yoff+h-1 );

                    guide.clear();
                    guide.push_back( geo::vec3( top_left.x, top_left.y, 0 ) );
                    guide.push_back( geo::vec3( bottom_right.x, top_left.y, 0 ) );
                    guide.push_back( geo::vec3( bottom_right.x, bottom_right.y, 0 ) );
                    guide.push_back( geo::vec3( top_left.x, bottom_right.y, 0 ) );
                    guide.push_back( geo::vec3( top_left.x, top_left.y, 0 ) );
                }
            }

            bool safe_guide;
            std::string name;
            float aspect_ratio;
            std::vector< geo::vec3 > guide;
        };
        std::vector< info > infos;
        std::vector< geo::vec3 > crosshairv, crosshairh;
        float dt;
        // }

        GLdouble projection_matrix_ro[16];                     // Where The 16 Doubles Of The Projection Matrix Are To Be Stored
        GLdouble modelview_matrix_ro[16];                      // Where The 16 Doubles Of The Modelview Matrix Are To Be Stored

        public:

        reticle( int w = 640, int h = 480 ) : dt(0)
        {
            // http://en.wikipedia.org/wiki/Aspect_ratio_(image)

            infos.push_back( info(      "current", float(w) / float(h) ) );
            infos.push_back( info( "current safe", float(w) / float(h), true ) );
            infos.push_back( info(       "tv 4:3", 1.33f ) );
            infos.push_back( info(    "hdtv 16:9", 1.78f ) );
            infos.push_back( info(   "theatrical", 1.85f ) );
            infos.push_back( info(   "anamorphic", 2.35f ) ); //cinemascope

            resize(w, h);
        }

        const GLdouble* get_projection_matrix()
        {
            glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix_ro);     // Retrieve The Projection Matrix
            return projection_matrix_ro;
        }

        const GLdouble* get_modelview_matrix()
        {
            glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_ro);        // Retrieve The Modelview Matrix
            return modelview_matrix_ro;
        }

        void resize( int w, int h)
        {
            // update "current" and "current safe" reticle aspect_ratios
            infos[ 0 ].aspect_ratio = float(w) / float(h);
            infos[ 1 ].aspect_ratio = float(w) / float(h);

            // update all reticles with new given dimensions
            for( size_t i = 0; i < infos.size(); ++i )
                infos[ i ].resize( w, h );

            // update crosshair
            float segment = std::sqrtf( w*w + h*h ) * 0.05; // 10% of hypotenuse
            crosshairh.clear();
            crosshairh.push_back( geo::vec3( w/2 - segment, h/2, 0 ) );
            crosshairh.push_back( geo::vec3( w/2 + segment, h/2, 0 ) );
            crosshairv.clear();
            crosshairv.push_back( geo::vec3( w/2, h/2 - segment, 0 ) );
            crosshairv.push_back( geo::vec3( w/2, h/2 + segment, 0 ) );
        }

        void update( float dt ) {
            this->dt += dt;
            if( this->dt > 1 ) {
                dt = 0;
            }
        }

        void render()
        {
            int frame = int( dt * 100.0 );

            eve::matrix::ortho flat;

            for( size_t i = 0; i < infos.size(); ++i )
            {
                if( infos[i].safe_guide )
                {
                    eve::style::dashed line_style;
                    eve::style::green line_color;
                    eve::geometry::lines( infos[i].guide );
                }
                else
                {
                    eve::style::red line_color;
                    eve::geometry::lines( infos[i].guide );
                }

                eve::style::yellow text_color;
                eve::geometry::text2d( infos[i].guide[3].x + 5, infos[i].guide[3].y - 20, infos[i].name );
            }

            char buffer[32];
            sprintf(buffer, "frame %02d", frame);

            eve::style::white crosshair_color;
            eve::geometry::lines h( crosshairh );
            eve::geometry::lines v( crosshairv );
            eve::geometry::text2d( crosshairh[0].x, crosshairh[0].y, buffer );
        }
    };
}
