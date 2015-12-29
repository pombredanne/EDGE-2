#ifndef EE_SDL2_TIMER_HPP
#define EE_SDL2_TIMER_HPP

#include "timer.hpp"
#include <SDL.h>

namespace ee {

struct sdl2_timer : timer {
    virtual uint64_t now_s() {
        return SDL_GetTicks() / 1000.f;
    }
    virtual uint64_t now_ms() {
        return SDL_GetTicks();
    }
};

}

#endif
