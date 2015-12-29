#include <iostream>
#include <cassert>

#include "trophy.hpp"

enum maps    { map1, map2, map3, circus_map };
enum enemies { zombie, clown, cow };
enum weapons { bomb, knife, gun };
enum actions { frag, talk, grab, pickup };

struct status
{
    int action = 0;
    int weapon = 0;

    union {
        struct { int type; } frag;
        struct { int width, height; } resize;
        struct { int keysym, scancode, mod; } key;
        struct { int x, y, buttons; } motion;
        struct { int x, y, button, mod; } button;
        struct { int dummy[4]; } dummy;
    };

    status()
    {
        static_assert( sizeof(frag)   <= sizeof(dummy), "sizeof(dummy) has to be greater or equal than sizeof(frag)" );
        static_assert( sizeof(resize) <= sizeof(dummy), "sizeof(dummy) has to be greater or equal than sizeof(resize)" );
        static_assert( sizeof(key)    <= sizeof(dummy), "sizeof(dummy) has to be greater or equal than sizeof(key)" );
        static_assert( sizeof(motion) <= sizeof(dummy), "sizeof(dummy) has to be greater or equal than sizeof(motion)" );
        static_assert( sizeof(button) <= sizeof(dummy), "sizeof(dummy) has to be greater or equal than sizeof(button)" );

        memset( dummy.dummy, 0, sizeof(dummy) );
    }
};

struct player
{
    // per context
    status personal, clan, worldwide;
    // per frame
    status global, local;
};

/*  // eventually we'll do something like:
    evaluate( player.status );
    player.stats.global += player.local.status
    player.stats.clan += player.clan.status
*/

int main( int argc, const char **argv )
{
    status frame;

    ::trophies trophies;

    trophy add;

    // trophy "the butcher": stab 30 mad cows
    add.title = "The butcher";
    add.description = "Stab 30 mad cows";
    add.goal = 30;
    add.check = [&]() -> bool {
        if( frame.action == frag )
        if( frame.weapon == knife )
        if( frame.frag.type == cow )
            return true;
        return false;
    };
    trophies.todo.push_back( add );

    // trophy "carnival fest": stab 15 evil clowns
    add.title = "Carnival Outrage";
    add.description = "Stab 15 evil clowns";
    add.goal = 15;
    add.check = [&]() -> bool {
        if( frame.action == frag )
        if( frame.weapon == knife )
        if( frame.frag.type == clown )
            return true;
        return false;
    };
    trophies.todo.push_back( add );

    // show trophies before usage
    std::cout << "BEFORE" << std::endl;
    std::cout << trophies.debug() << std::endl;

    // simulate 30 frames
    for( int i = 0; i < 30; ++i )
    {
        frame.action = frag;
        frame.weapon = knife;
        frame.frag.type = cow;
        trophies.update();
    }

    // show trophies after usage
    std::cout << "AFTER" << std::endl;
    std::cout << trophies.debug() << std::endl;

    return 0;
}
