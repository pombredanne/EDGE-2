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

#include "../prj/git.hpp"

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
int eeQuitModule();

#include <3rd/frodo/frodo.hpp>
#include <3rd/hertz/hertz.hpp>
#include <iostream>

namespace memory {
    bool init() {
        return true;
    }
    bool quit() {
        return true;
    }
}

namespace logger {
    bool init() {
        std::cout << "logger subsystem initialized" << std::endl;
        return true;
    }
    bool quit() {
        return true;
    }
}

namespace console {
    bool init() {
        std::cout << "console subsystem initialized" << std::endl;
        return true;
    }
    bool quit() {
        std::cout << "bye bye console" << std::endl;
        return true;
    }
}

int main( int argc, const char **argv ) {
    printf("%s\n", EE_TEXT);

    // app-defined levels
    // 00 memory and hooks
    frodo::ring(  0, { "memory", memory::init, memory::quit } );
    // 10 subsystems
    frodo::ring( 13, { "logger", logger::init, logger::quit } );
    frodo::ring( 14, { "console", console::init, console::quit } );
    // 20 devices 
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

    if( !frodo::init() ) {
        return -1;
    }

    auto tick = [] {

    };

    auto render = [] {
        static unsigned c = 0;
        std::cout << "[ OK ] Running... " << "\\|/-"[(c++)%4] << "\r";
    };

    // app starts here
    for( int i = 0; i < 60 * 10; ++i ) {
        double fps = hertz::lock( 60, tick, render ); 
    }

    // deinit
    hertz::lock( -1, []{}, []{} ); 

    // shutdown
    return frodo::quit() ? 0 : -1;
}
