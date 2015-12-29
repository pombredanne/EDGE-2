// A 2D/3D separate axis theorem class. MIT licensed
// - rlyeh, 2012-2013

#pragma once

#include <cassert>
#include <map>
#include <set>
#include <tuple>

//     0   1   2   3
//  -+---+---+---+---+-
// 0 | A | AB| B |   |
//  -+---+---+---+---+-
// 1 |   | B | BC| C |
//  -+---+---+---+---+-
// 2 |   |   |   |   |
//  -+---+---+---+---+-
// 3 |   |   |   |   |
//  -+---+---+---+---+-

template< typename T >
class sat3
{
    typedef typename std::set< T > set;

    std::map< int, set > X;
    std::map< int, set > Y;
    std::map< int, set > Z;

public:

    sat3()
    {}

    void insert( T elem, int x, int y, int z ) {
        X[ x ].insert( elem );
        Y[ y ].insert( elem );
        Z[ z ].insert( elem );
    }
    void erase( T elem, int x, int y, int z ) {
        X[ x ].erase( elem );
        Y[ y ].erase( elem );
        Z[ z ].erase( elem );
    }
    set nearby( int x, int y, int z, int radius = 1 ) {

        auto xcursor = X.find(x), xcursorprev = xcursor - 1;
        auto xdist = abs( xcursor - xcursorprev );
        if( xdist < radius ) return set();

        auto ycursor = Y.find(y), ycursorprev = ycursor - 1;
        auto ydist = abs( ycursor - ycursorprev );
        if( ydist < radius ) return set();

        auto zcursor = Z.find(z), zcursorprev = zcursor - 1;
        auto zdist = abs( zcursor - zcursorprev );
        if( zdist < radius ) return set();

        auto union3 = [&]( const set &s1, const set &s2, const set &s3 ) -> set {
            set out;
            set_union( s1.begin(), s1.end(), s2.begin(), s2.end(), out.begin() );
            for( auto &in : s3 )
                out.insert( in );
            return out;
        };

        return union3( X[x], Y[y], Z[z] );
    }


};
