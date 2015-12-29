// [ref] based on code by Lighthouse3D

#pragma once

#include <string>
#include <iostream>

#include "vec.hpp"

namespace geo
{
    class plane
    {
        public:

        geo::vec3 normal;
        float d;

        plane()
        {}

        plane( const geo::vec3 &_normal, const geo::vec3 &_point )
        {
            normal = norm(_normal);
            d = -math3d::dot( normal, _point );
        }

        plane( const geo::vec3 &p1, const geo::vec3 &p2, const geo::vec3 &p3 ) // 3 points
        {
            normal = norm(cross( p3 - p2, p1 - p2 ));
            d = -math3d::dot( normal, p2 );
        }

        plane( float a, float b, float c, float d ) // 4 coefficients
        {
            // set the normal vector
            normal = geo::vec3( a, b, c );
            //compute the lenght of the vector
            float l = len(normal);
            // normalize the vector
            normal = geo::vec3( a/l, b/l, c/l );
            // and divide d by th length as well
            this->d = d/l;
        }

        template<typename ostream>
        inline friend ostream &operator<<( ostream &os, const plane &p ) {
            os << '{' << p.normal << ',' << p.d << '}';
            return os;
        }
    };
}

float distance( const geo::plane &plane, const geo::vec3 &point );

