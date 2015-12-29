// A 2D/3D collision grid. MIT licensed
// - rlyeh, 2012

#pragma once

#include <map>
#include <set>
#include <cassert>
#include <tuple>

namespace geo
{
    template< typename T >
    class grid3
    {
        const int cell_size;

        typedef std::tuple<int,int,int> key;

        std::map< key, std::set< T > > map;
        typename std::map< key, std::set< T > >::iterator cursor;

        inline key at( int x, int y, int z ) const
        {
            return key( x / cell_size, y / cell_size, z / cell_size );
        }

        public:

        grid3( int _cell_size = 1 ) : cursor(map.end()), cell_size(_cell_size)
        {
            assert( _cell_size > 0 );
        }

        void setup( int _cell_size )
        {
            assert( _cell_size > 0 );

            cell_size = _cell_size;
            map = std::map< key, std::set< T > >();
            cursor = map.end();
        }

        bool find( int x, int y, int z )
        {
            cursor = map.find( at(x,y,z) );
            return( cursor != map.end() );
        }

        std::set< T > &found() const
        {
            return cursor->second;
        }

        size_t size() const
        {
            return map.size();
        }

        size_t count( const T &elem, int x, int y = 0, int z = 0 ) const
        {
            if( !find(x,y,z) )
                return 0;
            return cursor->count( elem );
        }

        void insert( const T &elem, int x, int y = 0, int z = 0 )
        {
            ( map[ at(x,y,z) ] = map[ at(x,y,z) ] ).insert( elem );
        }

        void erase( const T &elem, int x, int y = 0, int z = 0 )
        {
            ( map[ at(x,y,z) ] = map[ at(x,y,z) ] ).erase( elem );
        }

        void clear( int x, int y = 0, int z = 0 )
        {
            ( map[ at(x,y,z) ] = map[ at(x,y,z) ] ) = std::set< T >();
        }
    };    
}
