#include <cmath>

#include <limits>
#include <string>
#include <iostream>

#include "point.hpp"
#include "sphere.hpp"
#include "utils.hpp"

geo::sphere merge( const geo::sphere &s0, const geo::sphere &s1 ) // returns
{
    geo::sphere s;

    static const float epsilon = std::numeric_limits<float>::epsilon();

    // RTCD book, p269
    // compute squared distance between sphere centers
    geo::vec3 d = s1.center - s0.center;
    float dist2 = math3d::dot( d, d );

    if( sqr( s1.radius - s0.radius ) >= dist2 )
    {
        // sphere with larger radius encloses the other;
        // set s to be largest
        s = ( s1.radius >= s0.radius ? s1 : s0 );
        s.id = std::string("\"") + s.id + '\"';
    }
    else
    {
        // sphere overlapping or disjoint
        float dist = std::sqrt( dist2 );
        s.radius = ( dist + s0.radius + s1.radius ) * 0.5f;
        s.center = s0.center;
        if( dist > epsilon )
            s.center += (( s.radius - s0.radius ) / dist ) * d;

        s.id = std::string("(") + s0.id + '+' + s1.id + ')';
    }

    return s;
}

bool intersects( const geo::sphere &a, const geo::sphere &b )
{
    // RTCD book, p88
    // calculate squared distance between centers
    geo::vec3 d = a.center - b.center;
    float dist2 = math3d::dot(d,d);
    // spheres intersect if squared distance is less than squared sum of radii
    float radiusSum = a.radius + b.radius;
    return dist2 <= radiusSum * radiusSum;
}
