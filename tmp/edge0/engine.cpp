#include "engine.hpp"

#include "renderer.hpp"
#include "window.hpp"
#include "input.hpp"
#include "script.hpp"
#include "sprite.hpp"

#include <SDL.h>
#include "sdl2_renderer.hpp"
#include "sdl2_window.hpp"
#include "sdl2_logger.hpp"
#include "sdl2_timer.hpp"
#include "sdl2_input.hpp"
#include "sdl2_filesystem.hpp"

#include <lua.hpp>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef _WIN32
#include <direct.h>
#define chdir _chdir
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#include "git.hpp"
#endif
#include "macros.hpp"
#define EE_URL      "https://github.com/r-lyeh/EDGE" 
#define EE_VERSION  "1.0.0"
#define EE_DATE     __DATE__
#define EE_BUILD    $debug("DEBUG-") $release("RELEASE-") $bits32("32") $bits64("64") 
#define EE_GIT      $windows( "git-" GIT_BRANCH "-r" GIT_REVISION )
#define EE_COMPILER $msvc("MSC") $clang("clang") $gnuc("gcc")
#define EE_TEXT     "Edge v" EE_VERSION " (" EE_BUILD ") (" EE_GIT ") (" EE_DATE ") (" EE_COMPILER ")"

#include <profit/profit.hpp>

namespace ee {

window *wnd;
renderer *gfx;
input *hid;
script *env;
logger *log;
timer *tme;
filesystem *fs;

int argc;
const char **argv;

bool init( int argc, const char **argv ) {
    ee::argc = argc;
    ee::argv = argv;

    // initialise SDL
    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) ) {
        //cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
        return 0;
    }

    log = new sdl2_logger();

    /*
    auto *wnd2 = new sdl2_window( 640, 480 );
    wnd = wnd2;

    gfx = new sdl2_renderer( wnd2->sdlWindow, wnd2->sdlRenderer );
    */
    wnd = 0;
    gfx = 0;
    hid = new sdl2_input();
    env = new ee::script();
    tme = new sdl2_timer();
    fs  = new sdl2_filesystem();

    #define def(name, ...) { name, [](lua_State *L) -> int __VA_ARGS__ },

    std::map<std::string, lua_CFunction> graphicsMap { 
        def("test", {
            return 0;
        })
        { "openScreen", [](lua_State *L) -> int {
            int x = lua_tonumber(L, 1);
            int y = lua_tonumber(L, 2);

            if(gfx) {
                delete gfx, gfx = 0;
            }
            if(wnd) {
                delete wnd, wnd = 0;
            }
            auto *wnd2 = new sdl2_window( 640, 480 );
            wnd = wnd2;
            gfx = new sdl2_renderer( wnd2->sdlWindow, wnd2->sdlRenderer );

//          wnd->open(x, y);
            return 0;
        }}, 
        { "drawRect", [](lua_State *L) -> int {
            int x;
            int y;
            int w;
            int h;
            int colour;
            x = lua_tointeger(L, 1);
            y = lua_tointeger(L, 2);
            w = lua_tointeger(L, 3);
            h = lua_tointeger(L, 4);
            colour = lua_tointeger(L, 5);
            gfx->draw_rect(x, y, w, h, colour);
            return 0;
        }}, 
        { "drawSprite", [](lua_State *L) -> int {
            int x;
            int y;
            int w;
            int h;
            const char *name;
            x = lua_tointeger(L, 1);
            y = lua_tointeger(L, 2);
            w = lua_tointeger(L, 3);
            h = lua_tointeger(L, 4);
            name = lua_tolstring(L, 5, 0);

            Sprite spr( gfx->load_texture(name) );
            spr.position.x = x;
            spr.position.y = y;
            spr.render(*gfx);

            return 0;
        }}, 
        { "width", [](lua_State *L) -> int {
            lua_pushnumber(L, wnd->width());
            return 1;
        }}, 
        { "height", [](lua_State *L) -> int {
            lua_pushnumber(L, wnd->height());
            return 1;
        }},
    };

    std::map<std::string, lua_CFunction> inputMap = { 
        { "mouseX", [](lua_State *L) -> int {
            lua_pushinteger(L, hid->mouse_x());
            return 1;
        }}, 
        { "mouseY", [](lua_State *L) -> int {
            lua_pushinteger(L, hid->mouse_y());
            return 1;
        }}, 
        { "mouseButton", [](lua_State *L) -> int {
            lua_pushinteger(L, hid->mouse_left());
            return 1;
        }},
        { "keyboard", [](lua_State *L) -> int {
            int kc = lua_tointeger(L, 1);
            int ret = hid->keyboard(kc);

            lua_pushinteger(L, ret);
            return 1;
        }}, 
    };

    std::map<std::string, lua_CFunction> fsMap = { 
        { "mount", [](lua_State *L) -> int {
            fs->mount(lua_tolstring(L, 1, 0));
            return 0;
        }}, 
        { "scan", [](lua_State *L) -> int {
            fs->scan();
            return 0;
        }}, 
        { "size", [](lua_State *L) -> int {
            const char *str = lua_tolstring(L, 1, 0);
            int sz( fs->size( str ) );
            lua_pushinteger(L, sz);
            return 1;
        }}, 
        { "find", [](lua_State *L) -> int {
            const char *str = lua_tolstring(L, 1, 0);
            str = fs->find( str );
            lua_pushstring(L, str);
            return 1;
        }}, 
    };

    env->bind_class("window", graphicsMap);
    env->bind_class("input", inputMap);
    env->bind_class("fs", fsMap);

    auto lua_log = [](lua_State *L) -> int { 
        const char *txt = lua_tolstring(L, 1, 0);
        ee::log->info(txt);
        return 0;
    };
    env->bind_function("log", lua_log );

    {
        char *base = SDL_GetBasePath();
        if( !base ) {
            base = SDL_strdup("./");
        }
        std::string rodir(base);
        chdir(base);
        SDL_free(base);
    }

    {
        char *base = SDL_GetPrefPath("My Company", "My Awesome SDL 2 Game");
        if (!base) {
            base = SDL_strdup("./");
        }
        std::string rwdir(base);
        SDL_free(base);
    }

    ee::log->dev("%s - begin {", EE_TEXT);

    if( !env->load_file( argc > 1 ? argv[1] : "main.lua") ) {
        return false;
    }

    return true;
}

bool quit() {
    profit::report(std::cout);
    if( ee::log ) {
        ee::log->dev("} end // %s", EE_TEXT);
    }
    delete fs, fs = 0;
    delete tme, tme = 0;
    delete env, env = 0;
    delete hid, hid = 0;
    delete gfx, gfx = 0;
    delete wnd, wnd = 0;
    delete log, log = 0;
    SDL_Quit();
    return true;
}

bool restart() {
    if( quit() ) {
        if( init( argc, argv ) ) {
            return true;
        }
    }
    return false;
}

}

