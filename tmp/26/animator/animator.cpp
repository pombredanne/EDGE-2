// - rlyeh // listening to amplifier - black rainbow

#include "playlist.hpp"
#include <sand/sand.hpp>
#include <windows.h>


struct player {
    std::string name;
#if USE_ANIMATION_BLENDING
    blend_controller c;
#else
    controller c;
#endif
    float x = 0, y = 0, z = 0;
    bool on_ground = 1;
    bool dead = 0;
    int health = 100;

    /*
    unity's mecanim animations

    1  "walk-loop"
    2  "run-intro"
    3  "run-loop"
    4  "run-outro"
    5  "roar"
    6  "swipe-attack"
    7  "hit-from-front"
    8  "hit-left"
    9  "hit-right"
    10 "strafe-right-intro"
    11 "strafe-right-loop"
    12 "strafe-right-outro"
    13 "strafe-left-intro"
    14 "strafe-left-loop"
    15 "strafe-left-outro"
    16 "jump"
    17 "overhead-smash"
    18 "grab-and-eat"
    19 "shuffle-turn-right-loop"
    20 "shuffle-turn-left-loop"
    21 "back-pedal"
    22 "die"
    23 "spawn"
    24 "upper-swipe-attack"
    25 "idle"
    */

    // spawn, roar, [x] die
    // [x] idle, walk, run, back
    // [x] strafe left/right,
    // [x] turn left/right
    // [x] upper/swipe attack, smash,
    // grab,
    // [x] jump
    // [x] hit

    void action( bool w, bool a, bool s, bool d, bool fire1, bool fire2, bool shift, bool hit ) {
        if( dead ) {
            return;
        }
        if( !on_ground ) {
            y -= 1;
            if( y < 0 ) {
                y = 0;
                on_ground = 1;
            }
        }
        if( on_ground ) {
            if( health <= 0 ) {
                c.play("die");
                dead = true;
                return;
            }
            if( hit ) {
                c.play("hit-from-front");
                health--;
                return;
            }

            /**/ if( fire1 )      { c.play("jump"); y+=10; on_ground = 0; return; }
            else if( fire2 )      {
                if( c.is_playing("idle-loop") || c.is_playing("run-intro") || c.is_playing("run-loop") ) {
                    c.play("swipe-attack"); //, "overhead-smash", "upper-swipe-attack");
                    c.push_factor(2.00);
                    return;
                }

                if( c.is_playing("swipe-attack") && c.progress() >= 50 ) {
                    c.push("upper-swipe-attack");
                    c.push_factor(2.00);
                    return;
                }
                if( c.is_playing("upper-swipe-attack") && c.progress() >= 50 ) {
                    c.push("overhead-smash");
                    c.push_factor(2.00);
                    return;
                }

            }

            /**/ if( w && s )     { c.play("shuffle-turn-right-loop", "shuffle-turn-right-loop"); return; }
            else if( w && shift ) { c.play("walk-loop"); return; }
            else if( w )          { c.play("run-intro", "run-loop"); return; }
            else if( s )          { c.play("back-pedal"); return; }

            /**/ if( a )          { c.play("strafe-left-intro", "strafe-left-loop" ); return; }
            else if( d )          { c.play("strafe-right-intro", "strafe-right-loop" ); return; }

            /**/ if( c.has_played("strafe-left-loop") ) {
                c.play("strafe-left-outro");
                return;
            }
            else if( c.has_played("strafe-right-loop") ) {
                c.play("strafe-right-outro");
                return;
            }
            else if( c.has_played("run-loop") ) {
                c.play("run-outro");
                return;
            }

            if( !c.is_playlist_playing() ) {
                if( c.has_played("run-intro") )
                    c.play("run-outro");
                else
                    c.play("idle-loop");
            }
        }
    }

    void update() {
        // grab positions here
        c.update();
        // apply offset positions
        // if env collides, restore them back
        // or, use dot(t,p) and bounce
    }

    std::string str() const {
        return
            wire::string("\1> (dead:\2,x:\3,y:\4,z:\5,ground:\6,health:\7) ", name, dead,x,y,z,on_ground,health ) +
            wire::string("\1", c.str() );
    }
};

BOURNE_DEFINE( player &it, (it.name, it.x, it.y, it.z, it.on_ground, it.dead, it.health));

extern int animfps;
extern float animtick, blendtick;
extern int blendpass;
extern int playing;
extern volatile int exiting;

#include <mutex>

player p1;
bool ok = false;

int load_animation( const char *the_asset_txt ) {

    playing = 0;
    animtick = 0;

    p1 = player();
    p1.name = "boss";

		ok = false;

    if( !p1.c.load(the_asset_txt) ) {
            return -1;
    }

    playing = 1;

    p1.c.insert_event( "idle-loop", 1100, EVENT_DEBUG, []{ std::cout << "idle event!" << std::endl; } );
    p1.c.play("idle-loop");

    std::cout << bourne::to_json(p1.c) << std::endl;

    unsigned attack_counter = 0;
    //boss.insert_event( "");

		ok = true;
    return 0;
}

int animate( float dt ) {
    if( !ok ) {
        // old anim controller
        if( playing ) {
            animtick += dt;
        }
        return 0;

    }

    std::cout << p1.str() << std::endl;

    bool w = GetAsyncKeyState(VK_UP) & 0x8000;
    bool a = GetAsyncKeyState(VK_LEFT) & 0x8000;
    bool s = GetAsyncKeyState(VK_DOWN) & 0x8000;
    bool d = GetAsyncKeyState(VK_RIGHT) & 0x8000;
    bool shift = GetAsyncKeyState(VK_SHIFT) & 0x8000;
    bool fire1 = GetAsyncKeyState('Z') & 0x8000;
    bool fire2 = GetAsyncKeyState('X') & 0x8000;
    bool hit   = GetAsyncKeyState('C') & 0x8000;

    p1.action(w,a,s,d,fire1,fire2,shift,hit);
    p1.update();

    if(playing) { 
        unsigned frame = p1.c.get_frame();
        animtick = frame - 1;

#if USE_ANIMATION_BLENDING
        frame = p1.c.prev.get_frame();
        blendtick = frame - 1;

        blendpass = p1.c.frame_to_blend;
#endif
    }

   	return 0;
}
