#include <eve/render.hpp>
#include "mesh.hpp"

void eve::render( const eve::mesh &mesh )
{
    // activate and specify pointers to vertex and normal arrays. order is important
    array::vertex4f v4f( mesh.vertices.data() );
    array::normal3f n3f( mesh.normals.size() ? mesh.normals.data() : 0 );

    // submit triangles
    array::submit::triangles( mesh.triangles );
}
