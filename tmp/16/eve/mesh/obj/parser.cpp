#include <cassert>
#include <iostream>
#include <vector>

#include "../mesh.hpp"
#include "parser.hpp"
#include "obj.h"

#include <eve/render.hpp>
#include <geo/vec.hpp>


// @todo: layers: http://www.clockworkcoders.com/oglsl/tutorial8.htm
// @todo: lights: http://www.clockworkcoders.com/oglsl/tutorial5.htm

bool eve::loader::obj( mesh &self, const std::string &pathfile )
{
    self = mesh();

    auto &texcoords = self.texcoords;
    auto &vertices = self.vertices;
    auto &normals = self.normals;

    auto &triangles = self.triangles;
    auto &points = self.points;
    auto &lines = self.lines;

    obj_model_t mesh;
    obj_model_t *mdl = &mesh;

    if( !ReadOBJModel( pathfile.c_str(), &mesh) )
        return false;

    if( mdl->num_texCoords > 0 )
    {
        texcoords.resize( mdl->num_texCoords );
        for( int i = 0; i < mdl->num_texCoords; ++i )
        {
            texcoords[i][0] = mdl->texCoords[i].uvw[0];
            texcoords[i][1] = mdl->texCoords[i].uvw[1];
            texcoords[i][2] = mdl->texCoords[i].uvw[2];
        }
    }

    if( 0 )
    if( mdl->num_normals > 0 )
    {
        normals.resize( mdl->num_normals );
        for( int i = 0; i < mdl->num_normals; ++i )
        {
            normals[i][0] = mdl->normals[i].ijk[0];
            normals[i][1] = mdl->normals[i].ijk[1];
            normals[i][2] = mdl->normals[i].ijk[2];
        }
    }

    if( mdl->num_verts > 0 )
    {
        vertices.resize( mdl->num_verts );
        for( int i = 0; i < mdl->num_verts; ++i )
        {
            vertices[i][0] = mdl->vertices[i].xyzw[0];
            vertices[i][1] = mdl->vertices[i].xyzw[1];
            vertices[i][2] = mdl->vertices[i].xyzw[2];
            vertices[i][3] = mdl->vertices[i].xyzw[3];
        }
    }

    bool has_uv = mdl->has_texCoords != 0;
    bool has_normals = mdl->has_normals != 0;

    float3 normzero = { 0,0,0 };
    normals.resize( mdl->num_faces, normzero );

    for( int i = 0; i < mdl->num_faces; ++i )
    {
        size_t elements = mdl->faces[i].num_elems;

        std::vector<unsigned int> *indices = 0;

        if( elements == 0 )
        {
            // huh?
            continue;
        }
        else
        if( elements == 1 )
        {
            indices = &points;
        }
        else
        if( elements == 2 )
        {
            indices = &lines;
        }
        else
        if( elements == 3 )
        {
            indices = &triangles;
        }
        else
        if( elements == 4 )
        {
            // quad... unsupported type (deprecated)
            continue;
        }
        else
        {
            // polygon... unsupported type (deprecated)
            continue;
        }

        if( 0 ) // this seems to be working :)
        for( unsigned j = 0; j < elements; ++j )
        {
            indices->push_back( mdl->faces[i].vert_indices[j] );
/*
            if( has_normals )
            indices.push_back( mdl->faces[i].norm_indices[j] );

            if( has_uv )
            indices.push_back( mdl->faces[i].uvw_indices[j] );
*/
        }

        if( elements == 3 )
        {
            // from http://www.iquilezles.org/www/articles/normals/normals.htm

            const int ia = mdl->faces[i].vert_indices[0];
            const int ib = mdl->faces[i].vert_indices[1];
            const int ic = mdl->faces[i].vert_indices[2];

            const eve::vec3 e1 = eve::vec3( vertices[ia] ) - vertices[ib];
            const eve::vec3 e2 = eve::vec3( vertices[ic] ) - vertices[ib];
            const eve::vec3 no = -cross( e1, e2 );

            normals[ia].x += no.x;
            normals[ia].y += no.y;
            normals[ia].z += no.z;

            normals[ib].x += no.x;
            normals[ib].y += no.y;
            normals[ib].z += no.z;

            normals[ic].x += no.x;
            normals[ic].y += no.y;
            normals[ic].z += no.z;

            // verts
            indices->push_back( ia );
            indices->push_back( ib );
            indices->push_back( ic );

            // norms
            indices->push_back( ia );
            indices->push_back( ib );
            indices->push_back( ic );
        }
    }

    FreeModel(&mesh);

//            normals.clear(); // disable normals by clearing them :D

    return true;
}
