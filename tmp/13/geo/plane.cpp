#include <iostream>

#include "plane.hpp"

float distance( const geo::plane &plane, const geo::vec3 &point )
{
    return plane.d + math3d::dot( plane.normal, point );
}
