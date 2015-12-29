// r-lyeh, boost licensed

#pragma once
#include <map>
#include <functional>

enum EVENT_TYPE { // <-- enumeration examples
    EVENT_NONE = 0,
    EVENT_DEBUG = 1,
    EVENT_GAMEPLAY = 2,
    EVENT_GEOMETRY = 4,
    EVENT_AUDIO = 8,
    EVENT_PARTICLES = 16,
    EVENT_MOTION = 32,
    EVENT_UI = 64,
    EVENT_PLAYER = 128,
    EVENT_SERVER = 256,
};

struct event {
    unsigned types;             // possible types of callback this event applies to
    std::function<void()> fn;   // callback
};

// keyframe -> events 
struct events : public std::map< unsigned, std::vector< event > > {

    void execute( unsigned keyframe, unsigned mask = ~0 ) const {
        auto found = this->find( keyframe );
        if( found != this->end() ) {
            for( auto &event : found->second ) {
                if( (event.types & mask) && (event.fn) ) {
                    event.fn();
                }
            }
        }
    }
};
