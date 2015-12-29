#pragma once

#include <iostream>
#include <string>

#include "point.hpp"

namespace geo
{
    struct sphere
    {
        geo::point center;
        float radius;

        std::string id;

        sphere &init( float x, float y, float z, float _radius, const std::string &_id = std::string() )
        {
            this->id = _id;
            this->center.x = x;
            this->center.y = y;
            this->center.z = z;
            this->radius = _radius;

            return *this;
        }

        const bool operator==( const sphere &sph ) const
        {
            return radius == sph.radius && center.x == sph.center.x && center.y == sph.center.y && center.z == sph.center.z;
        }

#ifdef  MSGPACK_DEFINE
        MSGPACK_DEFINE( center, radius, id );
#endif
#ifdef  MEDEA_DEFINE
        MEDEA_DEFINE( center, radius, id );
#endif

        template<typename ostream>
        inline friend ostream &operator<<( ostream &os, const sphere &s ) {
            os << '{' << s.center << ',' << s.radius << '}';
            return os;
        }
    };
}

geo::sphere merge( const geo::sphere &s0, const geo::sphere &s1 );
bool intersects( const geo::sphere &a, const geo::sphere &b );
std::string nff( const geo::sphere &s );
