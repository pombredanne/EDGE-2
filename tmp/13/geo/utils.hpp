#pragma once

#include "point.hpp"
#include "vec.hpp"

namespace math2d
{
    float len( const geo::vec3 &v );
    float len2( const geo::vec3 &v );
    float len( const geo::vec3 &a, const geo::vec3 &b );
    float len2( const geo::vec3 &a, const geo::vec3 &b );
    geo::vec3 norm( const geo::vec3 &v );
    geo::vec3 perp90( const geo::vec3 &v );
    geo::vec3 perp45( const geo::vec3 &v );
    geo::vec3 inv( const geo::vec3 &v );
    float dot( const geo::vec3 &v, const geo::vec3 &w );
    float perp_dot( const geo::vec3 &v, const geo::vec3 &w );
}
namespace math3d
{
    float len( const geo::vec3 &v );
    float len2( const geo::vec3 &v );
    float len( const geo::vec3 &a, const geo::vec3 &b );
    float len2( const geo::vec3 &a, const geo::vec3 &b );
    geo::vec3 norm( const geo::vec3 &v );
    geo::vec3 inv( const geo::vec3 &v );
    float dot( const geo::vec3 &v, const geo::vec3 &w );
    geo::vec3 cross( const geo::vec3 &v, const geo::vec3 &w );
}

using namespace math3d;

float sqr( float n);
geo::vec3 lerp( const geo::vec3 &a, const geo::vec3 &b, float dt01 );

double deg2rad( double deg );
double rad2deg( double rad );

float shortest_angle_xy( const geo::vec3 &a, const geo::vec3 &b );
float angle_xy( const geo::vec3 &a, const geo::vec3 &b ); //[-PI..PI]
geo::vec3  rotate( const geo::vec3 &v, float angle );

