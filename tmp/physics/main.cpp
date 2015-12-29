#include <iostream>

#define HAS_TOKAMAK 0
#define HAS_BULLET  1
#define HAS_INPUT   1

#include <eve/eve.hpp>
#include <eve/render.hpp>
#include <eve/window.hpp>
#include <geo/camera.hpp>

#define dot dot_

#if HAS_TOKAMAK
#include "tk.hpp"
#endif

#if HAS_BULLET
#include "bt.hpp"
#endif

#undef dot

#include "randworld.hpp"

#if HAS_INPUT
eve::mouse mouse(0);
eve::keyboard keyboard(0);
#endif


class window : public eve::window
{
    public:

    eve::camera camera;

#if HAS_TOKAMAK
    tk::world tkWorld;
    tk::object tkGround;
    std::vector<tk::object> tkFall;
#endif

#if HAS_BULLET
    bt::world btWorld;
    bt::object btGround;
    std::vector<bt::object> btFall;
#endif

    window() : eve::window("physics sample", 0.85f)
    {
        setup();
    }

    ~window()
    {
        clear();
    }

    void clear()
    {
#if HAS_TOKAMAK
        // tokamak
        tkWorld.clear();
        tkFall.clear();
#endif

#if HAS_BULLET
        // bullet
        btWorld.clear();
        btFall.clear();
#endif
    }

    void setup()
    {
#if HAS_TOKAMAK
        // tokamak
        tkWorld.setup( eve::vec3(0,0,-10) );
        for( int i = 0; i < 16; ++i )
            tkFall.push_back( tkWorld.add<tk::tkRandom>( 1, eve::vec3(i * 2,0,50)) );
        tkGround = tkWorld.add<tk::tkPlane>( 0.f );
#endif

#if HAS_BULLET
        // bullet
        btWorld.setup( eve::vec3(0,-10,0) );
        for( int i = 0; i < 16; ++i )
            btFall.push_back( btWorld.add<bt::btRandom>( 1, eve::vec3(0.5f * i,50 + i * 4,0) ) );
        btGround = btWorld.add<bt::btPlane>( 0.f, eve::vec3(0,-1,0) );
#endif
    }

    void restart()
    {
        clear();
        setup();
    }

    void update_camera( eve::camera &camera )
    {
        static bool once = true; if( once ) { once = false;
        camera.position = eve::vec3(50,50,50);
        };

#if HAS_INPUT
        keyboard.update();
        mouse.update();

        bool dragged_a = mouse.left.hold() && ( mouse.client.newest().xdt > 0 );
        bool dragged_d = mouse.left.hold() && ( mouse.client.newest().xdt < 0 );
        bool dragged_e = mouse.left.hold() && ( mouse.client.newest().ydt > 0 );
        bool dragged_c = mouse.left.hold() && ( mouse.client.newest().ydt < 0 );

        camera.
        wsadec( keyboard.w.hold(),              keyboard.s.hold(),
                keyboard.a.hold() || dragged_a, keyboard.d.hold() || dragged_d,
                keyboard.e.hold() || dragged_e, keyboard.c.hold() || dragged_c );

        camera.wheel( mouse.wheel.newest().y );
#endif

        camera.resize( this->w, this->h );
        camera.lookat( eve::vec3(0,0,0) );
        camera.update();
        camera.apply();
    }

    void update( double t, float dt )
    {
        update_camera( camera );

        static bool paused = false;
#if HAS_INPUT
        if( keyboard.space.trigger() )
            paused ^= true;

        if( keyboard.f5.trigger() )
            restart();

        if( mouse.left.hold() )
        {
#if HAS_TOKAMAK
            // tokamak
            tkFall.front().push( eve::vec3(0,0,0.5f) );
#endif
#if HAS_BULLET
            // bullet
            btFall.front().push( eve::vec3(0,0.5f,0) );
#endif
        }
#endif

        if( !paused )
        {
#if HAS_TOKAMAK
            // tokamak
            tkWorld.run( dt );
#endif

#if HAS_BULLET
            // bullet
            btWorld.run( dt );
#endif
        }
    }

    void render( double t, float dt )
    {
        glClearColor(0.32f,0.32f,0.32f, 1.f);
        glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
        glColor4f(1.f,1.f,1.f,1.f);
        glDisable(GL_BLEND);

        camera.apply();

        if( EVE_COMPILE_LIST )
            render_random_world( 150, 50 );

#if HAS_TOKAMAK
        // tokamak
        for( size_t i = 0; i < tkFall.size(); ++i )
            tkFall[i].render();
#endif

#if HAS_BULLET
        camera.apply90();
        // bullet
        for( size_t i = 0; i < btFall.size(); ++i )
            btFall[i].render();
        btWorld.render();
#endif

    }
};

int main( int argc, char **argv )
{
    window w;

    while( w.is_open() ) {
        w.update( w.t, w.dt );
        w.render( w.t, w.dt );
        w.flush( 1/60.f );
    }

    return 0;
}
