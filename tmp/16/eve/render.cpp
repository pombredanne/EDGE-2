#include "render.hpp"
#include "camera.hpp"

namespace eve
{
    namespace matrix
    {
        eve::vec3 translate::position = eve::vec3(0,0,0);
    }
}

#include <geo/frustum.hpp>

void render( const eve::camera &cam, const float &projection ) // [0 = position .. 1 = lookat, 2 = lookat*2, ...]
{
    eve::vec3 pos = cam.position + cam.direction * projection;

    eve::disable::lighting nl;
    eve::matrix::translate t( pos );

    if( eve::compile axesgeo = eve::compile("camera.render_itself()/[axes]") )
    {
        eve::matrix::scale sc( eve::vec3(0.75,0.75,0.75) );
        eve::style::pink color;
        eve::geometry::point();
        eve::geometry::axes();
    }

    eve::matrix::rotate_z rcz( cam.yaw() - 90 );
    eve::matrix::rotate_x rcx( cam.pitch() );

    if( eve::compile camgeo = eve::compile("camera.render_itself()/[outlines+camera]"))
    {
        {
            eve::style::thick th(3.0);
            eve::style::black color;
            eve::geometry::camera cam;
        }
        {
            eve::style::green color;
            eve::geometry::camera cam;
        }
    }

    eve::matrix::rotate_x rcx2( 90 );

    render( cam.frustum, 10.f ); // should be get_far()
}

#include "render.hpp"
#include <geo/frustum.hpp>

//@todo: fix blending at exit
void render( const eve::frustum &fr, float _farPlane )
{
    // [ref] http://www.songho.ca/opengl/gl_transform.html

    float fovY = fr.fov;
    float aspectRatio = fr.aspect;
    float nearPlane = fr.neard;
    float farPlane = _farPlane ? _farPlane : fr.fard;

    float tangent = tanf( deg2rad(fovY*0.5) );

    float nearHeight = nearPlane * tangent;
    float nearWidth = nearHeight * aspectRatio;
    float farHeight = farPlane * tangent;
    float farWidth = farHeight * aspectRatio;

    // compute 8 vertices of the frustum
    float vertices[8][3];
    // near top right
    vertices[0][0] = nearWidth;     vertices[0][1] = nearHeight;    vertices[0][2] = -nearPlane;
    // near top left
    vertices[1][0] = -nearWidth;    vertices[1][1] = nearHeight;    vertices[1][2] = -nearPlane;
    // near bottom left
    vertices[2][0] = -nearWidth;    vertices[2][1] = -nearHeight;   vertices[2][2] = -nearPlane;
    // near bottom right
    vertices[3][0] = nearWidth;     vertices[3][1] = -nearHeight;   vertices[3][2] = -nearPlane;
    // far top right
    vertices[4][0] = farWidth;      vertices[4][1] = farHeight;     vertices[4][2] = -farPlane;
    // far top left
    vertices[5][0] = -farWidth;     vertices[5][1] = farHeight;     vertices[5][2] = -farPlane;
    // far bottom left
    vertices[6][0] = -farWidth;     vertices[6][1] = -farHeight;    vertices[6][2] = -farPlane;
    // far bottom right
    vertices[7][0] = farWidth;      vertices[7][1] = -farHeight;    vertices[7][2] = -farPlane;

    float colorLine1[4] = { 0.7f, 0.7f, 0.7f, 0.7f };
    float colorLine2[4] = { 0.2f, 0.2f, 0.2f, 0.7f };
    float colorPlane[4] = { 0.5f, 0.5f, 0.5f, 0.5f };

    eve::disable::lighting nolit;
    //eve::disable::culling nocull;
    glDisable(GL_CULL_FACE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //beautiful :D

    // draw the edges around frustum
    glBegin(GL_LINES);
    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[4]);

    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[5]);

    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[6]);

    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[7]);
    glEnd();

    glColor4fv(colorLine1);
    glBegin(GL_LINE_LOOP);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[7]);
    glEnd();

    glColor4fv(colorLine1);
    glBegin(GL_LINE_LOOP);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[3]);
    glEnd();

    // draw near and far plane
    glColor4fv(colorPlane);
    glBegin(GL_QUADS);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[7]);
    glEnd();

    glEnable(GL_CULL_FACE);
}
