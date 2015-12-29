#include <eve/gl.hpp>
#include <eve/gl/nocopy.hpp>
#include <eve/gl/disable.hpp>
#include <eve/geometry.hpp>
#include <nanosvg.h>

namespace eve
{
namespace geometry
{
        svg::svg( const std::string &pathFile )
        {
            // Load
            NSVGimage *image;
            image = nsvgParseFromFile( pathFile.c_str(), "px", 96 ); //"fg.svg"); //bg.svg"); tiger.svg");

            if( !image ) {
                return;
            }

            /*
            glColor3fv(color);
            glPolygonMode(MaterialFace.Front, PolygonMode.Fill);
            glBegin(BeginMode.Polygon);
            for (int j = 0; j < sides; j++)
            {
            glVertex2f(xPoints[j], yPoints[j]);
            }
            glEnd();
            glFinish();
            */

            eve::disable::texturing no_texture;

            $glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

            // Use...
            for (NSVGshape* shape = image->shapes; shape; shape = shape->next)
            {
                for (NSVGpath* path = shape->paths; path; path = path->next)
                {
                    if( path->npts ) {
                        $glBegin( path->closed ? GL_LINE_LOOP : GL_LINE_STRIP);
                        for( int i = 0; i < path->npts - 1; ++i) {
                            $glVertex3f(path->pts[i*2], path->pts[i*2+1], 160.0f );
                        }
                        $glEnd();
                    }
                }
            }

            // Delete
            nsvgDelete(image);
        }
}
}
