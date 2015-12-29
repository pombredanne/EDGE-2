//
//  ViewController.m
//  FontstashiOS
//
//  Created by Karim Naaji on 07/10/2014.
//  Copyright (c) 2014 Mapzen. All rights reserved.
//

#include "reticle.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <eve/geometry.hpp>
#include <eve/window.hpp>
#include <hertz/hertz.hpp>

#include <stdio.h>
#include <string.h>

#include "debugdraw2.h"
#include <windows.h>

#ifdef  $live
#undef  $live
#endif

#define $live(x) (x)

void setup(int width, int height) {

    glViewport(0, 0, width, height);
    glClearColor(0/255.f, 40/255.f, 40/255.f, 1.0f);

}

int key = 0;

void logic() {
    static unsigned int c = 0;
    if( (!c++) || GetAsyncKeyState(VK_SPACE) & 0x8000 ) {
        key = 1;
    } else {
        key = 0;
    }

}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    if( key ) {
        DrawSolidPolygon( floats {10,10, 30,30, 10,60}, 2 );
        DrawPolygon( floats {10,10, 30,30, 10,60} );
        DrawCircle( {200,200}, 10, 1 );
        DrawSolidCircle( {220,220}, 10, 1.25 );
        DrawSegment( {240,240}, {240,260}, 1.50 );
        DrawPoint( {260,260}, 20, 1.75 );
        DrawString( {280,280}, "hello world", 2.0 );
        DrawAABB( {300,300}, {320,320}, 2.25 );
        //DrawTransform( b2Transform {}, 1.0 );

        eve::geometry::cube cb;
    }
    {
        DebugDrawAll();
    }
}

void shutdown() {
}

int main() {
    eve::window app("debugdraw");
    setup(app.w, app.h);
    debugdraw::reticle r( app.w, app.h );
    while( app.is_open() ) {
        static int c = 0;
        hertz::lock( 60, [&]{
            app.title = std::string() + "debugdraw " + "\\|/-"[(c++)%4];
            logic();
            r.resize( app.w, app.h );
            r.update( 1/60.f );
        }, [&]{
            draw();
            r.render();
            app.flush( 1/60.f );
        } );
    }
    hertz::unlock();
    shutdown();
}
