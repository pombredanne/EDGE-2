// [ref] based on code by Lighthouse3D

#pragma once

#include "vec.hpp"

namespace geo
{
	class aabox
	{
		public:

		geo::vec3 corner;
		float x,y,z;

		aabox()
		{
			corner = geo::vec3(0,0,0);

			x = 1.0f;
			y = 1.0f;
			z = 1.0f;
		}

		aabox( const geo::vec3 &_corner, float x, float y, float z )
		{
			corner = _corner;

			if (x < 0.0) {
				x = -x;
				corner.x -= x;
			}
			if (y < 0.0) {
				y = -y;
				corner.y -= y;
			}
			if (z < 0.0) {
				z = -z;
				corner.z -= z;
			}

			this->x = x;
			this->y = y;
			this->z = z;
		}

		// for use in frustum computations

		geo::vec3 getVertexP( const geo::vec3 &normal ) const
		{
			geo::vec3 res = corner;

			if (normal.x > 0)
				res.x += x;

			if (normal.y > 0)
				res.y += y;

			if (normal.z > 0)
				res.z += z;

			return res;
		}

		geo::vec3 getVertexN( const geo::vec3 &normal ) const
		{
			geo::vec3 res = corner;

			if (normal.x < 0)
				res.x += x;

			if (normal.y < 0)
				res.y += y;

			if (normal.z < 0)
				res.z += z;

			return res;
		}
	};
}
