#include <msgpack.hpp> // include <msgpack.hpp> before <moon9/play/obj.hpp> in order to enable serialization
#include <moon9/play/obj.hpp>

#include <moon9/spatial/vec.hpp>

// our obj components

objfield< std::string > what("what", "que");
objfield< std::string > which("which", "cual");
objfield< std::string > where("where", "donde");
objfield< std::string > when("when", "cuando");
objfield< std::string > why("why", "por que");
objfield< std::string > who("who", "quien");
objfield< std::string > whom("whom", "a quien");
objfield< std::string > whose("whose", "de quien");
objfield< std::string > how("how", "como");
objfield< std::string > howmany("howmany", "cuanto");
objfield< std::string > howlong("howlong", "duracion");
objfield< std::string > howoften("howoften", "frecuencia");
objfield< std::string > howsure("howsure", "certeza");
objfield< std::string > whatorder("whatorder", "orden");
objfield< std::string > whatdegree("whatdegree", "grado");

// our obj initialization

void obj::user_defined_clone( const obj *other )
{
    copy_field( what, other );
    copy_field( which, other );
    copy_field( where, other );
    copy_field( when, other );
    copy_field( why, other );
    copy_field( who, other );
    copy_field( whom, other );
    copy_field( whose, other );
    copy_field( how, other );
    copy_field( howmany, other );
    copy_field( howlong, other );
    copy_field( howoften, other );
    copy_field( howsure, other );
    copy_field( whatorder, other );
    copy_field( whatdegree, other );
}

void obj::user_defined_erase()
{
    erase_field( what );
    erase_field( which );
    erase_field( where );
    erase_field( when );
    erase_field( why );
    erase_field( who );
    erase_field( whom );
    erase_field( whose );
    erase_field( how );
    erase_field( howmany );
    erase_field( howlong );
    erase_field( howoften );
    erase_field( howsure );
    erase_field( whatorder );
    erase_field( whatdegree );
}

void obj::user_defined_print() const
{
    print_field( what );
    print_field( which );
    print_field( where );
    print_field( when );
    print_field( why );
    print_field( who );
    print_field( whom );
    print_field( whose );
    print_field( how );
    print_field( howmany );
    print_field( howlong );
    print_field( howoften );
    print_field( howsure );
    print_field( whatorder );
    print_field( whatdegree );
}

void obj::user_defined_patch()
{
    patch_field( what );
    patch_field( which );
    patch_field( where );
    patch_field( when );
    patch_field( why );
    patch_field( who );
    patch_field( whom );
    patch_field( whose );
    patch_field( how );
    patch_field( howmany );
    patch_field( howlong );
    patch_field( howoften );
    patch_field( howsure );
    patch_field( whatorder );
    patch_field( whatdegree );
}

void obj::user_defined_diff( const obj *other ) const
{
    diff_field( what, other );
    diff_field( which, other );
    diff_field( where, other );
    diff_field( when, other );
    diff_field( why, other );
    diff_field( who, other );
    diff_field( whom, other );
    diff_field( whose, other );
    diff_field( how, other );
    diff_field( howmany, other );
    diff_field( howlong, other );
    diff_field( howoften, other );
    diff_field( howsure, other );
    diff_field( whatorder, other );
    diff_field( whatdegree, other );
}

#include <iostream>
#include <moon9/string/string.hpp>
#include <moon9/spatial/stats.hpp>
#include "pod.hpp"


//typedef int indexing_type;
typedef std::string indexing_type;

struct stats
{
    std::map< std::string, moon9::stats<unsigned char> > bytes;
    std::map< std::string, moon9::stats<int> > ints;
    std::map< std::string, moon9::stats<size_t> > sizes;
    std::map< std::string, moon9::stats<float> > floats;
    //std::map< std::string, moon9::stats<float00> > vec2s;
    //std::map< std::string, moon9::stats<float000> > vec3s;

    std::string debug() const
    {
        moon9::string out;

        for( auto &it : bytes )
            out << it.first << ':' << it.second.back() << std::endl;

        for( auto &it : ints )
            out << it.first << ':' << it.second.back() << std::endl;

        for( auto &it : sizes )
            out << it.first << ':' << it.second.back() << std::endl;

        for( auto &it : floats )
            out << it.first << ':' << it.second.back() << std::endl;

        //for( auto &it : vec2s )
        //    out << it.first << ':' << it.second.back() << std::endl;

        //for( auto &it : vec3s )
        //    out << it.first << ':' << it.second.back() << std::endl;

        out << sizes.begin()->second.report() << std::endl;

        return out;
    }
};

std::map< indexing_type, stats > players;

void account( const obj &o )
{
    if( o.has(what) && o[what] == "frags" )
    {
        std::cout << moon9::string( "* \1 \2 \3 (\4)\n", o[who], o[what], o[whom], o[howmany] );

        auto &who = players["sergio"];
        auto &what = who.sizes["frags"];

        int next = moon9::string( o[howmany] ).as<int>();

        what.push_back( next );
    }

    std::cout << players["sergio"].debug() << std::endl;
    std::cout << o.str() << std::endl;
}

int main( int argc, const char **argv )
{
    obj o;

    o[who] = "sergio";
    o[what] = "frags";
    o[whom] = "mario";

    o[howmany] = "100";
    account( o );

    o[howmany] = "50";
    account( o );

    o[howmany] = "120";
    account( o );

    o[howmany] = "25";
    account( o );

    return 0;
}


