#pragma once

enum properties
{
    dissolvable,    // water?
    flammable,      // fire?
    erosionable,    // time? wind? water?
    oxidable,       // time? water?
    corrodible,     // acid?

    stackable,
    embeddable,

    edible,

    wieldable,      // trigger fire left ?
    shieldable,     // trigger fire right ?
    throwable,      // trigger E ?
    useable,        // trigger E ? toggleable? triggerable?

    pickable,
    dropable,

    lightable,      // emits light on use
                    // emits heat on use
                    // emits cold on use
                    // emits rad hazard on use
                    // emits poison on use
                    // emits ...

    consumable,     // timeout?
    refilleable,    // can refill?
    rechargeable,   // can recharge? (auto refill)

    equipable,      // on/off + req skills + bonus
    questable,      // got/missing + visible/hidden -> only npcs can remove it!

    displayable,    // @inventory
    renderable,     // @battleground

    TOTAL,
    UNDEFINED
};

enum class attribs
{
    // hearts

    // spades

    // clubs

    // diamonds

    // enemies

    // others
    damage, // { melee, fire { bullets, shells }, magic, poison }
    armor,  // { phys,  fire { bullets, shells }, magic, poison }, cold/hot, fire/water
    range,  // when destructing/defending/healing/exploring
    health,

    speed,
    accel,
    dodge,
    jump,

    stamina,
    strength,

    size,
    weight,
    precision,

    silence,
    visibility,

    quantity,       // for stacking
    price,          // xp/level/skill required | for shops / spawning
    chance,         // luck
    rarity,         // for findings
    affinity,       // the more you use it, the better you get at it
    aggro4,         // player types that may handle this
    designed4,      // artisan types that may handle this
    skill,          // required skill to handle this

    position,
    orientation,
    lookat,

    shape,
    slot, // 0 [] 1 | 2 -+_ 3 -| 4 _| 5 _+- 6 |_

    spawns, spawn_rate, spawn_dir, die_as,
    neutrality, // friendly, enemy, neutral | faction
    // ...
    // ♥♦♣♠ aggro
    // ...
    // light radiation { howmuch, distance }
    // hazard radiation { howmuch, distance }, irradiates
    // heat radiation, ignites
    // poison radiation
    // cold radiation

    TOTAL,
    UNDEFINED
};

size_t bitset( int a = -1, int b = -1, int c = -1, int d = -1,
               int e = -1, int f = -1, int g = -1, int h = -1 )
{
    return 0
    | ( a < 0 ? 0 : 1 << a )
    | ( b < 0 ? 0 : 1 << b )
    | ( c < 0 ? 0 : 1 << c )
    | ( d < 0 ? 0 : 1 << d )
    | ( e < 0 ? 0 : 1 << e )
    | ( f < 0 ? 0 : 1 << f )
    | ( g < 0 ? 0 : 1 << g )
    | ( h < 0 ? 0 : 1 << h );
}

static const size_t poison = bitset( dissolvable, edible, useable, throwable, wieldable );

#include <bitset>
#include <sstream>

static inline std::string debug_prop( size_t properties ) {
    std::stringstream ss;
    ss << std::bitset< ::properties::TOTAL >( properties );
    return ss.str();
}

