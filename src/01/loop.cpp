#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

// directives

// Check windows
#if _WIN32 || _WIN64
#   if _WIN64
#       define EE_BITS 64
#   else
#       define EE_BITS 32
#   endif
#endif

// Check GCC
#if __GNUC__
#   if __x86_64__ || __ppc64__
#       define EE_BITS 64
#   else
#       define EE_BITS 32
#   endif
#endif

static_assert( EE_BITS >= 64, "You should be compiling on x64 arch");

#define EE_VERSION "0.0.0" /* (2015/12/05) Initial commit */

#define EE_STR2(x) #x
#define EE_STR(x) EE_STR2(x)

#include "../../prj/git.hpp"

#define EE_BUILD "DEBUG"
#define EE_URL "https://github.com/r-lyeh/EDGE"
#define EE_TEXT "EDGE " EE_VERSION " (" EE_STR(EE_BITS) "-bit " EE_BUILD ") (git:" GIT_BRANCH " r" GIT_REVISION ")"

#if defined(DEBUGOPT)
#undef  EE_BUILD
#define EE_BUILD "DEBUGOPT"
#endif

#if defined(RELEASE) || defined(NDEBUG) || defined(_NDEBUG)
#undef  EE_BUILD
#define EE_BUILD "RELEASE"
#endif

#if defined(SHIPPING)
#undef  EE_BUILD
#define EE_BUILD "SHIPPING"
#endif

// core interface

int eeInit();
int eeUpdate();
int eeRender();
int eeQuit();

int eeLoadModule();
int eeDropModule();

#include "../3rd.hpp"
#include "../api.hpp"
#include <iostream>

namespace memory {
    bool init() {
        std::cout << "[ OK ] setup: memory" << std::endl;
        return true;
    }
    bool quit() {
        std::cout << "[ OK ] teardown: mem" << std::endl;
        return true;
    }
}

namespace logger {
    bool init() {
        std::cout << "[ OK ] setup: logger" << std::endl;
        return true;
    }
    bool quit() {
        std::cout << "[ OK ] teardown: logger" << std::endl;
        return true;
    }
}

namespace console {
    bool init() {
        std::cout << "[ OK ] setup: console" << std::endl;
        return true;
    }
    bool quit() {
        std::cout << "[ OK ] teardown: console" << std::endl;
        return true;
    }
}

namespace framelocker {
    bool init() {
        std::cout << "[ OK ] setup: framerate locker" << std::endl;
        hertz::lock( 60, []{}, []{} );
        return true;
    }
    bool quit() {
        std::cout << "[ OK ] teardown: framerate locker" << std::endl;
        hertz::unlock();
        return true;
    }
}

namespace vfs {
    bool init();
    bool quit();
}


int eeGameInit() {
    printf("%s\n", EE_TEXT);

    // app-defined levels
    // 00 memory and hooks
    frodo::ring(  0, { "memory", memory::init, memory::quit } );
    // 10 subsystems
    frodo::ring( 13, { "logger", logger::init, logger::quit } );
    frodo::ring( 14, { "console", console::init, console::quit } );
    frodo::ring( 15, { "framerate-locker", framelocker::init, framelocker::quit } );
    // 20 devices
    frodo::ring( 20, { "filesystem", vfs::init, vfs::quit } );
    //frodo::ring( 20, { "audio", audio::init, audio::quit } );
    //frodo::ring( 25, { "data", data::init, data::quit } );
    //frodo::ring( 27, { "input", input::init, input::quit } );
    // 30 opengl
    //frodo::ring( 34, { "ui", ui::init, ui::quit } );
    //frodo::ring( 30, { "opengl", gl::init, gl::quit } );
    //frodo::ring( 31, { "monitor", monitor::init, monitor::quit } );
    //frodo::ring( 35, { "font", font::init, font::quit } );
    // 40 game
    //frodo::ring( 40, { "model", model::init, model::quit } );
    //frodo::ring( 45, { "world", world::init, world::quit } );
    // 50 ui
    //frodo::ring( 59, { "help", help::init, help::quit } );

    return frodo::init() ? 0 : -1;
}

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "user32.lib")
#endif

#include <algorithm>
#include <iostream>
#include <time.h>
#include <stdio.h>

int eeGameLoop() {
    unsigned HZ = 60, updates = 0, frames = 0, fps = 0;

    auto tick = [&]{
        updates++;
#ifdef _WIN32
        static auto &once = std::cout << "FPS keys: up, down, escape, space" << std::endl;
        if( GetAsyncKeyState(VK_UP) & 0x8000 )  HZ+=(std::min)((std::max)(int(HZ*0.01), 1), 5);
        if( GetAsyncKeyState(VK_DOWN) & 0x8000 ) if(HZ > (std::max)(int(HZ*0.01), 1)) HZ-=(std::max)(int(HZ*0.01), 1);
        if( GetAsyncKeyState(VK_ESCAPE) & 0x8000 ) exit(0);
        if( GetAsyncKeyState(VK_SPACE) & 0x8000 ) Sleep( rand() % 80 );
#endif
    };

    auto render = [&]{
        frames++;
        char bar[] = "\\|/-";
        auto current_time = time(NULL);
        auto sec = localtime(&current_time)->tm_sec;

        printf( "(%d/%d) [%c] updates %02ds [%c] frames \r",
            fps, HZ, bar[updates%4], sec, bar[frames%4] );
    };

    for(;;) {
        fps = hertz::lock( HZ, tick, render );
    }

    puts("");
    return 0;
}


int eeGameQuit() {
    // shutdown
    return frodo::quit() ? 0 : -1;
}

int main( int argc, const char **argv ) {
    if( eeGameInit() >= 0 ) {
        eeGameLoop();
        eeGameQuit();
    }
}
