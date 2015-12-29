#include "sdl2_window.hpp"
#include "gles.hpp"
#include <iostream>

#pragma comment( lib, "opengl32.lib" )

namespace ee {

sdl2_window::sdl2_window( int w, int h ) {
    open( w, h );
}

sdl2_window::~sdl2_window() {
    if( sdlGlContext )
    SDL_GL_DeleteContext(sdlGlContext);
    if( sdlRenderer )
    SDL_DestroyRenderer(sdlRenderer);
    if( sdlWindow )
    SDL_DestroyWindow(sdlWindow);
}

bool sdl2_window::open(int w, int h) {
    //SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &sdlWindow, &sdlRenderer);
    //SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL, &sdlWindow, &sdlRenderer);
    SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_OPENGL, &sdlWindow, &sdlRenderer);
    //SDL_CreateWindow()
    sdlGlContext = SDL_GL_CreateContext(sdlWindow);

    {
        SDL_GetWindowSize(sdlWindow, &w, &h);
        this->w = w;
        this->h = h;
    }

    initGL(w, h);
    return true;
}

int sdl2_window::width() {
    return w;
}

int sdl2_window::height() {
    return h;
}

}

