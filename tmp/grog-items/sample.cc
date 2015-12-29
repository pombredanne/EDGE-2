// Simple attribute/item/inventory system. MIT licensed.
// - rlyeh ~~ listening to Elder / Knot

#include "attrib.hpp"
#include "item.hpp"
#include <bitset>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

size_t &id() {
    static size_t id = 0;
    return (++id), id;
}

// cool tips:
// on heal : player.hp = player.max.hp
// on sanity : player.sp = player.max.sp
// player.armor
// player.speed
// player.attack
// player.lightrange / lightemit?
// player.[...]

// npc:
// alias { "boy", "john", "lord john" }

// weapon
// damage per bullet, clip size, reload speed -> "maximum damage per second" unit
// use this to calibrate weapons, ie, machine gun vs sniper



struct player
{
    std::vector< attribute * > attributes;    // base
    //std::vector< attribute > level;       // progress

    ::inventory inventory;                  // carry

    attribute speed;
    attribute armor;
    attribute hp;

    player()
    {
        speed.name = "speed";
        armor.name = "armor";
        hp.name = "hp";

        attributes.push_back( &speed );
        attributes.push_back( &armor );
        attributes.push_back( &hp );
    }

    void pickup( const ::item &item )
    { inventory.pickup( item ); }

    void drop( const ::item &item )
    { inventory.drop( item ); }

    void apply( int message ) // ie, wet
    { inventory.apply( message ); }

    // ...

    void levelup()
    {}

    void save( const std::string &name )
    {}

    void restore( const std::string &name )
    {
        for( auto &in : attributes )
            if( in->name == name )
            {
                *in = attribute();
                in->name = name;
            }
    }
};



enum { on_flame };

void handle_fire( item &it, int message ) {
    std::cout << it << ": burn!!!" << std::endl;
}

void handle_water( item &it, int message ) {
    std::cout << it << ": melted!!!" << std::endl;
}


int main( void )
{
    std::cout << debug_prop(poison) << std::endl;

    attribute paperring;
    paperring.name = "speed";
    paperring.add = 10;
    paperring.mul = 2.0f;
    paperring.handle = handle_fire;

    attribute icering;
    icering.name = "speed";
    icering.add = 30;
    icering.mul = 1.0f;
    icering.handle = handle_water;

    ::player player;
    player.pickup( item(paperring) );
    player.pickup( item(icering) );

    player.apply( on_flame );

    player.speed.add = 1;
    std::cout << player.speed() << std::endl;
    std::cout << debug( player.speed ) << std::endl;

    equip( player.speed, item(paperring) );
    std::cout << player.speed() << std::endl;
    std::cout << debug( player.speed ) << std::endl;

    equip( player.speed, item(icering) );
    std::cout << player.speed() << std::endl;
    unequip( player.speed, item(icering) );

    unequip( player.speed, item(paperring) );
    std::cout << player.speed() << std::endl;
    std::cout << debug( player.speed ) << std::endl;

    std::cout << debug( player.hp ) << std::endl;

    std::cout << "---" << std::endl;

    attribute a;
    a.on = true;
    a.name = "speed";
    a.add = 10;
    a.mul = 0.75f;
    verify(a);
    std::cout << debug(a) << std::endl;
    a.add = -10;
    a.mul = 1.5f;
    std::cout << debug(a) << std::endl;
    std::cout << value( a, 100 ) << std::endl;

    return 0;
}


