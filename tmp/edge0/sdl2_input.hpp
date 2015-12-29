#ifndef EE_SDL2_INPUT_H
#define EE_SDL2_INPUT_H

#include <stdint.h>
#include "input.hpp"

struct sdl2_input : ee::input {
    void process( bool has_focus );

    void terminate( bool enabled );
    bool terminated();

    void restart( bool enabled );
    bool restarted();

    void debug( bool enabled );
    bool debugged();

    int mouse_x();
    int mouse_y();
    int mouse_left();
    int keyboard( int keycode );

    private: int x = 0, y = 0, left = 0;
    private: const uint8_t *keystates = 0;
    private: bool is_terminated = 0, is_restarted = 0, is_debugged = 0;
};

#endif
