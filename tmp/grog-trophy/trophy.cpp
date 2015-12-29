// workflow:
// - el cliente comprueba datos en cada frame.
// - si los ve unlocked hace display al usuario, y lo manda al server en background+tickframe.
// - el server al final de la partida mira la lista de cosas a comprobar y el tickframe de cada una.
// - hace las comprobaciones que le piden, incluso las geoespaciales (tiene toda la partida en mem).
// - borra de la lista los trophies que no cumplan (cheats?).
// - actualiza la DB con la lista que queda: locked -> unlocked / date / reach=goal

#include <string>
#include <sstream>
#include <chrono>

#include "trophy.hpp"

namespace {
    auto epoch = [](){
        return std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
    };

    struct build {
        build( std::ostream& o )
        {}
        template<class First, class... Rest>
        build( std::ostream& o, const First& value, const Rest&... rest ) {
            o << value;
            build(o, rest...);
        }
    };
    template<class... T>
    std::string concat( const T&... value ) {
        std::ostringstream o;
        build(o, value...);
        return o.str();
    }

    // *estimated* lapse of time in short human readable terms
    std::string lapse( size_t date1, size_t date2 ) {
        const char *pl[2] = { "s", "" };
        int diff = date1 - date2;

        if( diff < 0 )
            diff = -diff;

        if( diff < 60 ) {
            diff = diff;
            return std::to_string(diff) + " second" + pl[!diff];
        }

        if( diff < 3600 ) {
            diff /= 60;
            return std::to_string(diff) + " minute" + pl[!diff];
        }

        if( diff < 86400 ) {
            diff /= 3600;
            return std::to_string(diff) + " hour" + pl[!diff];
        }

        // ~90 days, then months, then years
        if( diff < 7776000 ) {
            diff /= 86400;
            return std::to_string(diff) + " day" + pl[!diff];
        }

        if( diff < 31536000 ) {
            diff /= 2592000; // 30 days
            return std::to_string(diff) + " month" + pl[!diff];
        }

        diff /= 31536000;
        return std::to_string(diff) + " year" + pl[!diff];
    }
}

std::string trophy::debug() const {
    std::ostringstream ss;

    if( date > 0 )
        ss << concat("[X] ", title, " (completed ", lapse(epoch(), date), " ago)\n" ),
        ss << "    " << description << std::endl;
    else
        ss << concat("[ ] ", title, " (", counter, "/", goal, ", ", int(counter * 100) / (goal > 0 ? goal : 1), "% complete)\n" ),
        ss << "    " << description << std::endl;

    if( image.size() )
        ss << "    " << image << std::endl;

    return ss.str();
}

void trophies::update()
{
    for( auto it = todo.begin(); it != todo.end(); ) {

        auto &trophy = *it;
        if( !trophy.date && trophy.check && trophy.check() ) {
            if( ++trophy.counter >= trophy.goal ) {
                trophy.date = epoch() - 1;
                trophy.counter = trophy.goal;
                done.push_back( trophy );
                show.push_back( trophy );
				it = todo.erase( it );
                continue;
            }
        }

        ++it;
    }
}

std::string trophies::debug() const {
    std::ostringstream ss;

    // show all trophies status

    ss << "*** show ***" << std::endl;
    for( auto &in : show )
        ss << in.debug() << std::endl;

    ss << "*** todo ***" << std::endl; // render in b/w?
    for( auto &in : todo )
        ss << in.debug() << std::endl;

    ss << "*** done ***" << std::endl;
    for( auto &in : done )
        ss << in.debug() << std::endl;

    return ss.str();
}

#if 0

bool import( ::trophies &trophies, const std::string &json )
{
    if( bad(json) )
        return false;

    // clear
    trophies = ::trophies();

    // fill up
    for( auto &cast : json )
    {
        trophy object = cast;

        trophies.todo += object;
        trophies.done += object;
    }

    return true;
}

#endif
