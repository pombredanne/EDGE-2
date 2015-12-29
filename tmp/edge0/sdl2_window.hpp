#ifndef EE_SDL2_WINDOW_HPP
#define EE_SDL2_WINDOW_HPP

#include "window.hpp"
#include <SDL.h>

namespace ee {

struct sdl2_window : window {

    SDL_Window *sdlWindow;
    SDL_Renderer *sdlRenderer;
    SDL_GLContext sdlGlContext;

    sdl2_window();
    sdl2_window( int w, int h );
    ~sdl2_window();

    bool open(int w, int h);
    int width();
    int height();

    bool is_minimized() { return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_MINIMIZED ) > 0; }
    bool is_maximized() { return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_MAXIMIZED ) > 0; }
    bool is_fullscreen() { return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_FULLSCREEN ) > 0; }
    bool is_fullscreen_desktop() { return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP ) > 0; }
    bool is_visible() { return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_SHOWN ) > 0; }
    bool is_resizable() { return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_RESIZABLE ) > 0; }
    bool is_borderless() { return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_BORDERLESS ) > 0; }
    bool has_input_focus() { return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_INPUT_FOCUS ) > 0; }
    bool has_input_grabbed() { return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_INPUT_GRABBED ) > 0; }
    bool has_mouse_focus() { return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_MOUSE_FOCUS ) > 0; }
    bool has_mouse_captured() { return 0; } // return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_MOUSE_CAPTURED ) > 0; }

};

}

#endif