#pragma once
#include <string>
#include <sstream>
#include <stdio.h>
#include <utility>
#include <vector>
#include <iostream>

template<typename ostream, typename string>
bool to_netstring(ostream &os, const string &txt) {
    return (os << txt.size() << ":" << txt << ","), os.good();
}

template<typename istream, typename string>
bool from_netstring( istream &is, string &text ) {
    size_t size;
    is >> size;
    text.resize( size );
    is.ignore(1);
    is.read( &text[0], text.size() );
    is.ignore(1);
    return is.good();
}

template<typename ostream, typename vector>
bool to_netstrings( ostream &os, const vector &texts ) {
    for( auto &text : texts )
        if( !to_netstring( os, text ) ) {
            return false;
        }
    return true;
}

template<typename istream, typename vector>
bool from_netstrings( istream &is, vector &texts ) {
    while( is.peek() > 0 ) {
        texts.emplace_back();
        if( !from_netstring( is, texts.back() ) ) {
            return false;
        }
    }
    return true;
}

template<typename string>
string to_netstring( const string &text ) {
    std::stringstream ss;
    return to_netstring( ss, text ) ? ss.str() : string();
}

template<typename string>
string from_netstring( const string &text ) {
    string src;
    std::stringstream ss;
    return ss << text, from_netstring( ss, src ) ? src : string();
}

template<typename string, typename vector>
string to_netstrings( const vector &vec ) {
    std::stringstream ss;
    return to_netstrings( ss, vec ) ? ss.str() : string();
}

template<typename vector, typename string>
vector from_netstrings( const string &text ) {
    vector vec;
    std::stringstream ss;
    return ss << text, from_netstrings( ss, vec ) ? vec : vector();
}


#if 0
namespace pack {
    // note: <../bistring.hpp> header also defines a compatible 'bistring(s)' type
    typedef std::pair<std::string,std::string> bistring;
    typedef std::vector< bistring > bistrings;
    std::string netstring( const bistrings &bs );
}

namespace unpack {
    // note: <../bistring.hpp> header also defines a compatible 'bistring(s)' type
    typedef std::pair<std::string,std::string> bistring;
    typedef std::vector< bistring > bistrings;
    bistrings netstring( const std::string &s );
}

namespace pack {
    std::string netstring( const bistrings &p ) {
        std::string out;

        for( auto &it : p ) {
            out += to_netstring( it.first );
            out += to_netstring( it.second );
        }

        return out;
    }
}

namespace unpack {
    bistrings netstring( const std::string &s ) {
        bistrings bs;
        std::string line = s;

        while( line.size() > 0 ) {
            size_t offset;

            bistring b;

                b.first = from_netstring( line, &offset );
                line = line.substr( offset );

                b.second = from_netstring( line, &offset );
                line = line.substr( offset );

            bs.push_back( b );
        }

        return bs;
    }
}
#endif

#ifdef NETSTRING_BUILD_DEMO
#include <iostream>
#include <string>
#include <vector>

#include <windows.h>

int main() {
    std::cout << to_netstring<std::string>( "hello world" ) << std::endl;
    std::cout << from_netstring( to_netstring<std::string>( "hello world" ) ) << std::endl;

    std::vector<std::string> tests;
    tests.push_back( "hello world" );
    tests.push_back( "123456" );
    std::string net = to_netstrings<std::string>( tests );
    std::cout << net << std::endl;

    tests.clear();
    tests = from_netstrings<decltype(tests)>( net );

    for( auto &it : tests )
        std::cout << it << std::endl;

    std::pair<std::string, std::string> pair { "hello", "world" };
    std::cout << to_netstring( pair ) << std::endl;

    return 0;
}
#endif