#include <msgpack.hpp> // include <msgpack.hpp> before <moon9/play/obj.hpp> in order to enable serialization
#include <moon9/play/obj.hpp>

#include <moon9/spatial/vec.hpp>

#include <map>
#include <iostream>
#include <moon9/string/string.hpp>

#include <moon9/time/now.hpp>
#include <algorithm>

template<typename T>
struct data
{
    std::vector< double > time;
    std::vector<      T > value;
    std::vector<      T > accum;

    data()
    {}

    operator const T&() const
    { return value.back(); }

    operator T&()
    { return value.back(); }

    void push_back( const T &t )
    {
        time.push_back( moon9::now() );

        if( !value.size() )
        {
            value.push_back( t );
            accum.push_back( t );
        }
        else
        {
            value.push_back( t );
            accum.push_back( accum.back() + t );
        }
    }

    void push_back()
    { push_back( value.back() ); }

    // time

    double from() const
    { return time.front(); }

    double to() const
    { return time.back(); }

    double length() const
    { return to() - from(); }

    // value

    size_t hits() const
    { return value.size(); }

    T min() const
    { return *std::min_element( value.begin(), value.end() ); }

    T cur() const
    { return value.back(); }

    T acc() const // accumulation
    { return accum.back(); }

    T max() const
    { return *std::max_element( value.begin(), value.end() ); }

    T avg() const
    {
        T i = *value.begin() - *value.begin();

        for( auto &it : value )
            i += it;

        return i / value.size();
    }

    // report

    std::string debug() const
    {
        moon9::string out;

        out << "times: ";
        for( auto &it : time )
            out << it << ',';
        out << std::endl;

        out << "values: ";
        for( auto &it : value )
            out << it << ',';
        out << std::endl;

        out << "accums: ";
        for( auto &it : accum )
            out << it << ',';
        out << std::endl;

        out << "value: " << cur() << std::endl;
        out << "min: " << min() << std::endl;
        out << "max: " << max() << std::endl;
        out << "avg: " << avg() << std::endl;
        out << "hits: " << hits() << std::endl;
        out << "acc: " << acc() << std::endl;

        return out;
    }
};

typedef size_t indexing_type;

std::map< indexing_type /*player*/, data<size_t> /*data*/ > HP;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > XP;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > SP;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > ENERGY;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > SPEED;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > MAGIC;

std::map< indexing_type /*player*/, data<size_t> /*data*/ > FRAGS;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > DEATHS;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > SUICIDES;

std::map< indexing_type /*player*/, data<size_t> /*data*/ > STABS;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > HEADSTABS;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > BODYSTABS;

std::map< indexing_type /*player*/, data<size_t> /*data*/ > SHOOTS;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > HEADSHOOTS;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > BODYSHOOTS;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > FRAGSSHOOTS;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > SUICIDESHOOTS;

std::map< indexing_type /*player*/, data<size_t> /*data*/ > GRENADES;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > HEADGRENADES;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > BODYGRENADES;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > FRAGSGRENADES;
std::map< indexing_type /*player*/, data<size_t> /*data*/ > SUICIDESGRENADES;

#include <moon9/string/id.hpp>

int main( int argc, const char **argv )
{
    size_t sergio = moon9::id("sergio");

    HP[sergio].push_back( 10 );
    HP[sergio].push_back( 20 );
    HP[sergio].push_back( 40 );

    std::cout << HP[sergio].debug() << std::endl;

//    HP[sergio].multiplier(2.f);
//    HP[sergio].bonus(30, -1); // -1 every second

    std::cout << HP[sergio].debug() << std::endl;

    return 0;
}

/*
sample:

daemon( notify )
{
    sleep(2)

    if( !msg.recv )
        daemon.restart

    if( msg.unlocked )
        render.queue(msg.notify.drawcmds)
}

daemon(@server)
{
    msgs = []
    listen(socket, msgs)
    sleep(2)

    achievemnts = msgs.select(type: achievement)
    if( num(achievements) >= num(players)/2 ) // this wont let solo :s
        broadcast( verify(achievements) );
        db.store( achievements );

    restart
}

notify("")
{

}



filterinc( who, data )
{
    data[who]++;
}

filterinc( who, data )
{
    data[who]++;
    data[teamof(who)]++;
    data[friendsof(who)]++;
    data[clanof(who)]++;
    data[factionof(who)]++;

    if( who == me || teamof(who) == myteam || friendsof(who) == myfriends
    || clanof(who) == myclan || factionof(who) == myfaction )
    {
        achievements[ frags(>=1000) ] = "mortal ninja" + "mortal clan" + "mortal faction"
        achievements[ frags(>=100) ] = "assasin"
        achievements[ frags(>=10) ] = "discokeeper"

        achievements[ bombs(>=1000) ] = "nuclear guy"
        achievements[ bombs(>=100) ] = "b52 guy"
        achievements[ bombs(>=10) ] = "bomber"

        achievements[ bombs(==1) && kills(>=15) ] = "carnival fest"

        if( achievements.find( type(frags), num(frags) ) )
        {
            notify( achievements.found.text )
        }

        ...


        /**/ if( num(frags) >= 1000 ) notify_once("mortal ninja|mortal clan|mortal...");
        else if( num(frags) >=  100 ) notify_once("asassin");
        else if( num(frags) >=   10 ) notify_once("discokeeper");

        /**/ if( num(bombs) >= 1000 ) notify_once("nuclear guy");
        else if( num(bombs) >=  100 ) notify_once("b52 guy");
        else if( num(bombs) >=   10 ) notify_once("bomber");

        /**/ if( num(bombs) == 1 && num(kills) >= 15 )
        notify_once("carnival fest");


    }
}



onchat
{}

oncare
{}



oncollect
{}

onplant
{}



onbuild
{}

onexplore
{}



onheal()
{}

onfrag( who, whom, with, where )
{
    filterinc( who, frags );
    filterinc( whom, fragged );

    if( with & bomb )
    {
        filterinc( who, bombs );
        filterinc( whom, bombed );

        if( with & fire )
            filterinc( who, firebombs ),
            filterinc( whom, firebombed );

        if( who == me )
        {

        }
    }

    if( with & knife )
    {
        filterinc( who, stabs );
        filterinc( whom, stabbed );

        if( where & head )
            filterinc( who, headstabs ),
            filterinc( whom, headstabbed );
    }

    if( with & shoot )
    {
        filterinc( who, shoots );
        filterinc( whom, shooted );

        if( where & head )
            filterinc( who, headshots ),
            filterinc( whom, headshotted );
    }
}


achievements()
{
}

notifications()
{

}

*/
