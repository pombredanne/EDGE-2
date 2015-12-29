#pragma once

#include "pod.hpp"

template<typename A, typename B>
float pct( const A &a, const B &b )
{
    if( b > 0 || b < 0 )
        return ( a * 100 ) / float(b);

    return 0; //a?
}

template<typename A, typename B>
float ratio( const A &a, const B &time )
{
    if( t > 0 )
        return a / float(time);

    return 0;
}

size_t hours( size_t seconds )
{
    return seconds / 3600;
}

#if 0

// absolute -> num
// 1000 frags

// avg -> float
// 5.04 per game

// records+stats
struct stats
{
    struct // personal
    {
        // num games played
        sizet0 games;
        // num games finished
        sizet0 endings;
        // success avg
        float success() const { return pct( endings, games ); }
        // seconds played
        sizet0 seconds;
        // enemies fragged
        sizet0 frags;
        // friends protected
        sizet0 covers;
        // friends resurrected
        sizet0 resurrected;

        // sample[contexes]:
        // healed on
        sizet0 healed[me][friends][team][clan][faction][npcs][spcs][enemies][items][others]
        // healed from
        sizet0 healed[me][friends][team][clan][faction][npcs][spcs][enemies][items][others]

        // attribs<num times>
        healed, frags, covers, damaged,

        // records
        most healed, most frags, most covers, most damaged
        // infrarecords
        less healed, less frags, less covers, less damaged

        // avg kills per hour
        float effectiveness() const { return pct( frags, hours(seconds) ); }

        // avg num items 'A' shared with me, avg used per game: 5.05
        // avg num items 'B' shared with me, avg used per game: 0.51
        // avg num items 'A' shared with teammates
        // avg num items 'B' shared with teammates
        // avg num items 'A' shared with npcs
        // avg num items 'B' shared with npcs
        // ...

        // most played solo map, higher % of gameplay time
        // less played solo map, higher % of gameplay time
        // most played vs map, higher % of gameplay time
        // less played vs map, higher % of gameplay time
        // most played coop map, higher % of gameplay time
        // less played coop map, higher % of gameplay time
        // ...

        // most played map, higher % of gameplay time
        // less played map, higher % of gameplay time
        // most played character, higher % of gameplay time
        // less played character, higher % of gameplay time
    } player;

    struct // weapon
    {
        // weapon 'A'
        damage_done, shots, impacts, headshots, kills
        accuracy%() { return impacts / shots; }
        headshots%() { return headshots / kills; }
    };

    struct // weapon family
    {
        // family stats: pistols, melee, shells, bullets
        damage_done, shots, impacts, headshots, kills
        accuracy%()
        headshots%()

        // most used weapon, higher num
        // less used weapon, lower num

        // most effective weapon, higher % of kills/shoots
        // less effective weapon, lower % of kills/shoots
    };

    struct // weapon globals
    {
        // global stats
        damage_done, shots, impacts, headshots, kills
        accuracy%()
        headshots%()

        // per family: pistols, melee, shells, bullets
        // per mouse slot: level1, level2, melee
    };

    struct // items, powerups, magic, skills
    // medals, cups
    {
        is magic that damages a weapon?
        is magic that heals so?
    };

    struct // match stats
    {
        ♠ damage / score(frags) / % of team
        ♥ love / score(heals/covers) / % of team
        ♣ built / score(buildings/area) / % of team
        ♦ taken / score(destroyed/harvested) / % of team

        most damage done in one life (record)
        most love given in one life
        most things built in one life
        most things taken in one life

        num deads
        num healed
        num ...
        casualities on
        [me][team][...]
        casualities to
        [me][...]
        heals[]...

        time played
        powerups used

        num pistols,melee,shells,bullets,powerups,... (useful here?)

        avg life span 0s
    };

    struct // match family
    {
        // pvp
        // pve
        // pva
        // arena (waves record)
        // survival (time record)
    };

    struct // match globals
    {
        sizet0 won, lose, drawn;

        games completed, games quitted
        played() = completed+quitted

        num reports
        votes[positive][negative]
        completion[finished][quits]
        reputation(votes)
        reputation(completion)
        reputation(match;global)
    };

    struct // social
    {
        sizet0 shares, likes, comments;

        num reports
        comments[positive][negative]
        shares[positive][negative]
        likes[positive][negative]
        times positive, negative
        reputation(comments)
        reputation(shares)
        reputation(likes)
        reputation(social;global)
    };

    struct // transactions
    {
        sizet0 right, wrong, match;

        num reports
        transaction[positive][negative]

        description[positive][negative]
        communication[positive][negative]
        //rapido en el envio
        //razonable el precio
        reputation(transactions)
        reputation(descriptions)
        reputation(communication)
        reputation()
        reputation(trust)
    };

    struct // as personal leaderboard (solo)
    {
        [vs friends][vs clan mates][vs regional][vs worldwide]

        show: current position, top 25
        show: ♥♦♣♠ x4
    };

    struct // as friends leaderboard
    {
        [vs regional gangs]
        [vs worldwide gangs]

        show: current position, top 25
        show: ♥♦♣♠ x4
    };

    struct // as clan leaderboard
    {
        [vs regional clans]
        [vs worldwide clans]

        show: current position, top 25
        show: ♥♦♣♠ x4
    };

    struct // cloud storage, space to store user data
    {
        sizet0 bytes_used;
        sizet0 bytes_given;
    };

    struct // company
    {
        // last visit
        sizet0 last;
        // num purchases
        sizet0 purchases;
        // money spent
        sizet0 spent;
        // avg money per purchase?
        // spent money in last 2 months?
        // most expensive purchase?
        // what was it?
        // how many of those has he/she bought?
        // does he/she look for that often? does he/she need more?
    };



};

#endif
