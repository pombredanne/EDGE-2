#include "engine.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "script.hpp"
#include "window.hpp"
#include "logger.hpp"
#include "sprite.hpp"
#include "rect.hpp"
#include "timer.hpp"

#include <3rd/hertz/hertz.hpp>

struct module
{
    void update( float dt ) {}
    void render( float dt ) {}
};

void control_loop(module *game) {
    using namespace ee;
    if( hid->restarted() ) {
        restart();
    }
    if( hid->debugged() ) {
        hid->debug(0);
        env->debug();
    }
}

void update_loop(module *game) {
    using namespace ee;
    static auto last_time = tme->now_ms();
    auto cur_time = tme->now_ms();
    float delta(cur_time - last_time);
    last_time = cur_time;

    static auto frames = 0;
    frames++;

    static auto elapsed = 0;
    elapsed += delta;
    if( elapsed > 5000 ) {
        ee::log->dev( "%d fps", frames / 5 );
        elapsed -= 5000;
        frames = 0;
    }

    hid->process( wnd ? wnd->has_input_focus() : 1 );
    if( game ) {
        game->update(delta);
    }
    env->update();
}

void render_loop(module *game) {
    using namespace ee;
    if( gfx ) {
        gfx->clear();
    }
    if( game ) {
        game->render(0);
    }
    env->render();
    if( gfx ) {
        gfx->present();
    }
}

void main_loop(module *game) {
    control_loop(game);
    update_loop(game);
    render_loop(game);
}

int appmain(int argc, const char *argv[])
{
    if( ee::init(argc, (const char **)argv ) ) {
        module* game = new module; //Game(*ee::gfx);

        if( ee::gfx ) {
            ee::gfx->set_swap_interval(1);
            ee::gfx->set_logical_size(vec2i(640, 400));
        }

#       ifdef EMSCRIPTEN
            emscripten_set_main_loop_arg((em_arg_callback_func)main_loop, game, 0, 1);
#       else
            while( !ee::hid->terminated() ) {
                hertz::lock( 60, [&]{ control_loop(game); update_loop(game); }, [&]{ render_loop(game); } );
            }
#       endif

        delete game;
    }

    ee::quit();
    return 0;
}
