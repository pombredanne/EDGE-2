#include <eve/eve.hpp>

int main( int argc, char **argv )
{
    eve::window app( "svg", 0.75f);
    geo::camera camera;

    camera.position = geo::vec3( 50, 50, 50 );

    // xzy defaults to zxy basis,  CW, apply(),   zxy normals
    // xyz defaults to xyz basis, CCW, apply90(), xyz normals

    bool use_xzy = true;

    while( app.is_open() )
    {
        camera.wasdec( 0, 0.1f, 0, 0, 0, 0 );
        camera.lookat( eve::vec3(0,0,0) );
        camera.resize( app.w, app.h );
        camera.update();

        if( use_xzy )
        camera.apply();
        else
        camera.apply90();

        glClearColor( 0.2f, 0.2f, 0.2f, 1.f);
        glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
        glDisable(GL_BLEND);

        {
            eve::matrix::scale sc(10,10,10);
            eve::geometry::axes axes( 1.0 );
        }

        {
            eve::matrix::translate p(0,0,0);
        eve::style::red color;
//        eve::invert::normals inv;
        eve::matrix::scale sc(1.0/20,1.0/20,1.0/20);
        eve::geometry::svg svg("star_icon.svg");
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_DEPTH_TEST);
        glFrontFace( use_xzy ? GL_CW : GL_CCW );

        app.flush(1/60.f);
    }

    return 0;
}
