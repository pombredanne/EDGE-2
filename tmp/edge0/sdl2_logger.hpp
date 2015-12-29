#ifndef EE_SDL2_LOGGER_HPP
#define EE_SDL2_LOGGER_HPP

#include "logger.hpp"
#include "macros.hpp"
#include <stdarg.h>
#include <SDL.h>

struct sdl2_logger : public ee::logger {
    sdl2_logger() {
/*
        $debug(
        SDL_LogSetPriority( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG );
        )
        $release(
        SDL_LogSetPriority( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR );
        )
*/
        SDL_LogSetPriority( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG );
    }
    void dev( const char *fmt, ... ) {
        va_list args;
        va_start(args, fmt);
        SDL_LogMessageV( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, fmt, args );
        va_end(args);
    }
    void info( const char *fmt, ... ) {
        va_list args;
        va_start(args, fmt);
        SDL_LogMessageV( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, args );
        va_end(args);
    }
    void warning( const char *fmt, ... ) {
        va_list args;
        va_start(args, fmt);
        SDL_LogMessageV( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, fmt, args );
        va_end(args);
    }
    void error( const char *fmt, ... ) {
        va_list args;
        va_start(args, fmt);
        SDL_LogMessageV( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, fmt, args );
        va_end(args);
    }
};


#endif
