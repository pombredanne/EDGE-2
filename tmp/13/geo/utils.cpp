#include "point.hpp"
#include "vec.hpp"

#include "utils.hpp"


namespace math2d
{
    float len( const geo::vec3 &v )
    {
        return sqrt( v.x*v.x + v.y*v.y );
    }
    float len2( const geo::vec3 &v )
    {
        return v.x*v.x + v.y*v.y;
    }
    float len( const geo::vec3 &a, const geo::vec3 &b )
    {
        return len( b - a );
    }
    float len2( const geo::vec3 &a, const geo::vec3 &b )
    {
        return len2( b - a );
    }
    geo::vec3 norm( const geo::vec3 &v )
    {
        float length = len(v);
        return length ? geo::vec3( v.x/length, v.y/length, 0 ) : geo::vec3( 0,0,0 );
    }
    geo::vec3 perp90( const geo::vec3 &v )
    {
        return geo::vec3( -v.y, v.x, 0 );
    }
    geo::vec3 perp45( const geo::vec3 &v )
    {
        const double invroot2 = 0.70710678118654752440084436210485;
        geo::vec3 vv = perp90( v );
        return geo::vec3( (v.x+vv.x)*invroot2, (v.y+vv.y)*invroot2, 0 );
    }
    geo::vec3 inv( const geo::vec3 &v )
    {
        return geo::vec3( -v.x, -v.y, 0 );
    }
    float dot( const geo::vec3 &v, const geo::vec3 &w )
    {
        return v.x * w.x + v.y * w.y;
    }
    float perp_dot( const geo::vec3 &v, const geo::vec3 &w )
    {
        return v.x * w.y - v.y * w.x;
    }
}
namespace math3d
{
    float len( const geo::vec3 &v )
    {
        return sqrt( v.x*v.x + v.y*v.y + v.z*v.z );
    }
    float len2( const geo::vec3 &v )
    {
        return v.x*v.x + v.y*v.y + v.z*v.z;
    }
    float len( const geo::vec3 &a, const geo::vec3 &b )
    {
        return len( b - a );
    }
    float len2( const geo::vec3 &a, const geo::vec3 &b )
    {
        return len2( b - a );
    }
    geo::vec3 norm( const geo::vec3 &v )
    {
        float length = len(v);
        return length ? geo::vec3( v.x/length, v.y/length, v.z/length ) : geo::vec3( 0,0,0 );
    }
    geo::vec3 inv( const geo::vec3 &v )
    {
        return geo::vec3( -v.x, -v.y, -v.z );
    }
    float dot( const geo::vec3 &v, const geo::vec3 &w )
    {
        return v.x * w.x + v.y * w.y + v.z * w.z;
    }
    geo::vec3 cross( const geo::vec3 &a, const geo::vec3 &b )
    {
    #if 1
        geo::vec3 result;
        result.x = a.y * b.z - a.z * b.y;
        result.y = a.z * b.x - a.x * b.z;
        result.z = a.x * b.y - a.y * b.x;
        return result;
    #else
        return math3d::cross( a, b );
    #endif
    }
}

float sqr( float n )
{
    return n * n;
}

geo::vec3 lerp( const geo::vec3 &a, const geo::vec3 &b, float dt01 )
{
    return a * geo::vec3( dt01, dt01, dt01 ) + b * geo::vec3( 1.f - dt01, 1.f - dt01, 1.f - dt01 );
}

double deg2rad( double deg )
{
    const double _M_PI = 3.1415926535897932384626433832795;
    return (deg * _M_PI / 180);
}

double rad2deg( double rad )
{
    const double _M_PI = 3.1415926535897932384626433832795;
    return (rad * 180 / _M_PI);
}

float shortest_angle_xy( const geo::vec3 &a, const geo::vec3 &b )
{
    float angle_rad = acos( math3d::dot( norm(a), norm(b) ) );
    return rad2deg( angle_rad );
}

float angle_xy( const geo::vec3 &a, const geo::vec3 &b ) //[-PI..PI]
{
    float angle = atan2( math2d::perp_dot(a,b), math2d::dot(a,b) );
    return rad2deg(angle);
}

geo::vec3 rotate( const geo::vec3 &v, float angle )
{
    angle = deg2rad(angle);

    float c = cos(angle);
    float s = sin(angle);
    return geo::vec3( v.x * c - v.y * s, v.x * s  + v.y * c, v.z );
}
