#include <iostream>
#include "frodo.hpp"

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


int main() {

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

    // app starts here
    std::string dummy;
    std::cout << "Press any key to continue... (try aborting or killing app too)" << std::endl;
    std::getline( std::cin, dummy );

    // shutdown
    return frodo::quit() ? 0 : -1;
}