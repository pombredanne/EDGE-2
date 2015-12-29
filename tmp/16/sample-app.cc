#include "eve.hpp"

int main( int argc, char **argv )
{
    eve::window app( "ƎVE ɘve ΞVΞ - window sample", 0.75f );
    eve::camera camera;

    glClearColor( 0.2f, 0.2f, 0.2f, 1.f);
    glDisable(GL_BLEND);

    camera.position = eve::vec3( 50, 50, 50 );

    while( app.is_open() )
    {
        // update
        camera.orbit( 0.001f, 0.001f );
        camera.lookat( eve::vec3(0,0,0) );
        camera.resize( app.w, app.h );
        camera.update();
        camera.apply();

        // render
        glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

        //
        eve::style::red color;
        eve::geometry::cube rnd;

        // flush
        app.flush( 1/60.f );
    }

    return 0;
}
