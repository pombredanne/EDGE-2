#include "concat.hpp"
#include "sphere.hpp"

// http://tog.acm.org/resources/SPD/NFF.TXT

std::string nff( const geo::sphere &sph ) {
	return concat<' '>("s",sph.center.x,sph.center.y,sph.center.z,sph.radius,"\n");
}

/*
--- begin of file
s x y z r
--- end of file

The full specification of the NFF format can be found here. However, ASSIMP extends this specification and supports more basic shapes, including all platonic solids ('#' starts a comment line):
testplatonicsolids.nff

--- begin of file
# A tetrahedron at -10 0 0 with a 'radius' of 2
tet -10 0 0 2
# A cube at -7 0 0 with a 'radius' (a/2) of 2
hex -7 0 0 2
# An octahedron at -4 0 0 with a 'radius' of 2
hex -4 0 0 2
# A dodecahedron at -1 0 0 with a 'radius' (a/2) of 2
hex -1 0 0 2
# An icosahedron at 2 0 0 with a 'radius' (a/2) of 2
# This is a non-tesselated sphere. 'tess' sets the number of subdivisions.
# The default value for spheres is 4.
tess 0 s 2 0 0 2
--- end of file
(the 'radius' is the radius of the#
*/
