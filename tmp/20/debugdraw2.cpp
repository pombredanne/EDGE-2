/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* iPhone port by Simon Oliver - http://www.simonoliver.com - http://www.handcircus.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include <GL/glew.h>
#include <GL/freeglut.h> // glutBitmapString()
#include "debugdraw2.h"

#include <cstdio>
#include <cstdarg>
#include <cmath>
#include <cstring>

using namespace std;

#define b2_pi 3.1415926535897932384626433832795

namespace GL {

    void DrawPolygon( const float* vertices, unsigned vertexCount, const b2Color &color )
    {
        glColor4f(color.r, color.g, color.b, color.a);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    }

    void DrawSolidPolygon( const float* vertices, unsigned vertexCount, const b2Color &color )
    {
        glVertexPointer(2, GL_FLOAT, 0, vertices);

        glColor4f(color.r, color.g, color.b, color.a * 0.5f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

        glColor4f(color.r, color.g, color.b, color.a);
        glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    }

    #if 0
    void DrawSolidPolygon( const float* vertices, size_t vertexCount, const b2Color &color )
    {
        //set up vertex array
        glVertexPointer(2, GL_FLOAT, 0, vertices); //tell OpenGL where to find vertices
        glEnableClientState(GL_VERTEX_ARRAY); //use vertices in subsequent calls to glDrawArrays

        //draw solid area
        glColor4f( color.r, color.g, color.b, color.a);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

        //draw lines
        glLineWidth(3); //fat lines
        glColor4f( 1, 0, 1, color.a ); //purple
        glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    }
    #endif

    void DrawSegment( const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color )
    {
        glColor4f(color.r, color.g, color.b, color.a);
        GLfloat glVertices[] = {
            p1.x,p1.y,p2.x,p2.y
        };
        glVertexPointer(2, GL_FLOAT, 0, glVertices);
        glDrawArrays(GL_LINES, 0, 2);
    }

    template<bool SOLID>
    void DrawCircle( const b2Vec2 &center, float radius, const b2Color &color )
    {
        enum { vertexCount = 16 };
        const float k_segments = 16.0f;
        const float k_increment = 2.0f * b2_pi / k_segments;
        float theta = 0.0f;

        GLfloat glVertices[vertexCount*2];
        for (unsigned i = 0; i < k_segments; ++i)
        {
            b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
            glVertices[i*2]=v.x;
            glVertices[i*2+1]=v.y;
            theta += k_increment;
        }

        glColor4f(color.r, color.g, color.b, color.a);
        glVertexPointer(2, GL_FLOAT, 0, glVertices);

        glDrawArrays(SOLID ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, vertexCount);
    }

    void DrawTransform( const b2Transform &xf, const b2Colors &colors )
    {
        b2Vec2 p1 = xf.position, p2;
        const float k_axisScale = 0.4f;

        p2 = p1 + k_axisScale * xf.R.col1;
        DrawSegment(p1,p2,colors[0]);

        p2 = p1 + k_axisScale * xf.R.col2;
        DrawSegment(p1,p2,colors[1]);
    }

    void DrawPoint( const b2Vec2 &p, float size, const b2Color &color )
    {
        glColor4f(color.r, color.g, color.b, color.a);
        glPointSize(size);
        GLfloat glVertices[] = {
            p.x,p.y
        };
        glVertexPointer(2, GL_FLOAT, 0, glVertices);
        glDrawArrays(GL_POINTS, 0, 1);
        glPointSize(1.0f);
    }

    void DrawAABB( const b2Vec2 &min, const b2Vec2 &max, const b2Color &c )
    {
        glColor4f(c.r, c.g, c.b, c.a);

        GLfloat glVertices[] = {
            min.x, min.y,
            max.x, min.y,
            max.x, max.y,
            min.x, max.y
        };
        glVertexPointer(2, GL_FLOAT, 0, glVertices);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
    }

    void DrawString( const float *pos, const std::string &string, const b2Color &color ) {
        glColor4f( color.r, color.g, color.b, color.a);
        glRasterPos2f( pos[0]+0.375, pos[1]+0.375 );
        glutBitmapString( GLUT_BITMAP_8_BY_13, (const unsigned char *)string.c_str() );
    }
}

// Command buffer

#include <functional>
#include <deque>

namespace {
    using cmd = std::function<void()>;
    using frame = std::vector<cmd>;
    using queue = std::deque<frame>;
    enum { Nth = 256 };
    queue buffer(Nth);
}

#include <eve/geometry.hpp>

void DebugDrawAll() {
    if( !buffer.empty() ) {
        eve::matrix::ortho o2d;
        glEnableClientState(GL_VERTEX_ARRAY); //use vertices in subsequent calls to glDrawArrays
        for( auto &cmd : buffer.front() ) {
            cmd();
        }
        // remove frame & push new back to queue (to be rendered in Nth frames)
        buffer.pop_front();
        buffer.push_back( frame() );
    }
}

#define MAX(a,b) (a > b ? a : b)

void DrawCircle( const b2Vec2 &center, float radius, double seconds, const b2Color &color ) {
    auto frames = unsigned(seconds*60);
    buffer.resize( MAX( Nth, frames ) );
    for( auto i = 0u; i < frames; ++i ) {
        buffer[i].emplace_back( [=]{
            GL::DrawCircle<0>( center, radius, color.alpha(1 - (i/float(frames))) );
        } );
    }
}
void DrawSolidCircle( const b2Vec2 &center, float radius, double seconds, const b2Color &color ) {
    auto frames = unsigned(seconds*60);
    buffer.resize( MAX( Nth, frames ) );
    for( auto i = 0u; i < frames; ++i ) {
        buffer[i].emplace_back( [=]{
            GL::DrawCircle<1>( center, radius, color.alpha(1 - (i/float(frames))) );
        } );
    }
}
void DrawSegment( const b2Vec2 &p1, const b2Vec2 &p2, double seconds, const b2Color &color ) {
    auto frames = unsigned(seconds*60);
    buffer.resize( MAX( Nth, frames ) );
    for( auto i = 0u; i < frames; ++i ) {
        buffer[i].emplace_back( [=]{
            GL::DrawSegment( p1, p2, color.alpha(1 - (i/float(frames))) );
        } );
    }
}
void DrawTransform( const b2Transform &xf, double seconds, const b2Colors &colors ) {
    auto frames = unsigned(seconds*60);
    buffer.resize( MAX( Nth, frames ) );
    for( auto i = 0u; i < frames; ++i ) {
        buffer[i].emplace_back( [=]{
            GL::DrawTransform( xf, colors );
        } );
    }
}

void DrawPoint( const b2Vec2 &p, float size, double seconds, const b2Color &color ) {
    auto frames = unsigned(seconds*60);
    buffer.resize( MAX( Nth, frames ) );
    for( auto i = 0u; i < frames; ++i ) {
        buffer[i].emplace_back( [=]{
            GL::DrawPoint( p, size, color.alpha(1 - (i/float(frames))) );
        } );
    }
}

void DrawString(const b2Vec2 &p, const std::string &string, double seconds, const b2Color &color ) {
    auto frames = unsigned(seconds*60);
    buffer.resize( MAX( Nth, frames ) );
    for( auto i = 0u; i < frames; ++i ) {
        buffer[i].emplace_back( [=]{
            GL::DrawString( &p.x, string, color.alpha(1 - (i/float(frames))) );
        } );
    }
}

void DrawAABB( const b2Vec2 &min, const b2Vec2 &max, double seconds, const b2Color &color ) {
    auto frames = unsigned(seconds*60);
    buffer.resize( MAX( Nth, frames ) );
    for( auto i = 0u; i < frames; ++i ) {
        buffer[i].emplace_back( [=]{
            GL::DrawAABB( min, max, color.alpha(1 - (i/float(frames))) );
        } );
    }
}

void DrawSolidPolygon( const float* vertices, size_t vertexCount, double seconds, const b2Color &color ) {
    auto frames = unsigned(seconds*60);
    buffer.resize( MAX( Nth, frames ) );
    for( auto i = 0u; i < frames; ++i ) {
        buffer[i].emplace_back( [=]{
            GL::DrawSolidPolygon( vertices, vertexCount, color.alpha(1 - (i/float(frames))) );
        } );
    }
}
void DrawPolygon( const float* vertices, size_t vertexCount, double seconds, const b2Color &color ) {
    auto frames = unsigned(seconds*60);
    buffer.resize( MAX( Nth, frames ) );
    for( auto i = 0u; i < frames; ++i ) {
        buffer[i].emplace_back( [=]{
            GL::DrawPolygon( vertices, vertexCount, color.alpha(1 - (i/float(frames))) );
        } );
    }
}

#undef MAX
