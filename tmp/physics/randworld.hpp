#pragma once
#include <cstdlib>

namespace eve {
    static float rand11() {
        float random = std::rand() / float(RAND_MAX);
        return (random - 0.5f) * 2;
    }
    template<typename T, typename U = int>
    static T rand( T min, U max ) {
        float r;
        do {
            r = std::rand();
        } while( r < min || r > max );
        return T(r);
    }
}

void render_random_world( int geometries, int dimension )
{
    struct render_random
    {
        static void cube( const eve::vec3 &pos, int min_dim, int max_dim )
        {
            float dim = eve::rand( min_dim, max_dim );

            eve::matrix::translate tr( pos - eve::vec3(0,0,dim/2) ); // + !!!
            eve::matrix::scale scl( dim, dim, dim );
            eve::geometry::cube cb;
        }
        static void pilar( const eve::vec3 &pos, int min_dim, int max_dim )
        {
            float dim = eve::rand( min_dim, max_dim / 2 );
            float height = eve::rand( dim, max_dim );

            eve::matrix::translate tr( pos - eve::vec3(0,0,height/2) ); // + !!!
            eve::matrix::scale scl( dim, dim, height );
            eve::geometry::cube cb;
        }
    };

    {
        // "floor"
        eve::matrix::translate tr( eve::vec3(0,0,0) );
        eve::matrix::scale scl( dimension, dimension, 0.1f );
        eve::invert::normals inv;
        eve::geometry::cube cb;
    }

    for( int i = 0; i < geometries; ++i )
    {
        eve::vec3 pos( eve::rand11() * dimension * 0.5f, eve::rand11() * dimension * 0.5f, i * 0.001f );
        if( i % 2 )
        render_random::cube( pos.xyz(), 1, dimension / 10 );
        else
        render_random::pilar( pos.yxz(), 1, dimension / 10 );
    }
}
