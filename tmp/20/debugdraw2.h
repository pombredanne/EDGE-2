// @todo
// Antialiased 2D Grid, Marker, and Arrow Shaders
// http://jcgt.org/published/0003/04/01/paper.pdf
//

#pragma once
#include <stddef.h>
#include <initializer_list>
#include <vector>

union b2Vec2 {
    struct { float x, y; };
    struct { float e[2]; };

    b2Vec2() : x(0),y(0)
    {}

    b2Vec2(float x, float y) :x(x),y(y)
    {}

    template<typename T>
    b2Vec2( std::initializer_list<T> list ) {
        auto it = list.begin();
        x = *it++;
        y = *it++;
    }

    template<typename T>
    b2Vec2( const T &t ) {

    }

    b2Vec2(const b2Vec2 &other) : x(other.x),y(other.y)
    {}

    b2Vec2 &operator=( const b2Vec2 &other ) {
        if( this != &other ) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }

    b2Vec2 operator+( const b2Vec2 &other ) const {
        return b2Vec2( x + other.x, y + other.y );
    }
    b2Vec2 operator*( const b2Vec2 &other ) const {
        return b2Vec2( x * other.x, y * other.y );
    }

    b2Vec2 operator+( const float &other ) const {
        return b2Vec2( x + other, y + other );
    }
    b2Vec2 operator*( const float &other ) const {
        return b2Vec2( x * other, y * other );
    }

    const float *data() const {
        return &e[0];
    }
    float *data() {
        return &e[0];
    }

    operator const float *() const {
        return &e[0];
    }
    operator float *() {
        return &e[0];
    }
};

using floats = std::vector<float>;
using b2Vec2s = std::vector<b2Vec2>;

static inline
b2Vec2 operator*( const float &n, const b2Vec2 &other ) {
	return b2Vec2( n * other.x, n * other.y );
}
static inline
b2Vec2 operator+( const float &n, const b2Vec2 &other ) {
	return b2Vec2( n + other.x, n + other.y );
}

struct b2Transform {
	struct { b2Vec2 col1, col2; } R;
	b2Vec2 position;
};

#include <eve/hexcolor.hpp>
#include <string>

using b2Color = eve::hexcolor;
using b2Colors = eve::hexcolors;

#define SS(...) wire::string(__VA_ARGS__) // SAFE STRING: SS("hello \1 \2", "world", 123.f)
#define CS(...) wire::format(__VA_ARGS__) // C STRING:    CS("hello %s %f", "world", 123.f)

void DrawString(const b2Vec2 &p, const std::string &string, double seconds = 1.0, const b2Color &color = flatui::yellow );

void DrawCircle(const b2Vec2 &center, float radius, double seconds = 1.0, const b2Color &color = flatui::orange );
void DrawSolidCircle(const b2Vec2 &center, float radius, double seconds = 1.0, const b2Color &color = flatui::orange );

void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, double seconds = 1.0, const b2Color &color = flatui::green );

void DrawPoint(const b2Vec2 &p, float size, double seconds = 1.0, const b2Color &color = flatui::purple );



void DrawAABB(const b2Vec2 &min, const b2Vec2 &max, double seconds = 1.0, const b2Color &color = flatui::red );

void DrawPolygon(const float* vertices, size_t vertexCount, double seconds = 1.0, const b2Color &color = flatui::blue );
void DrawSolidPolygon( const float* vertices, size_t vertexCount, double seconds = 1.0, const b2Color &color = flatui::blue );

void DrawTransform(const b2Transform& xf, double seconds = 1.0, const b2Colors &colors = { flatui::red, flatui::green, flatui::blue, flatui::white } );

void DebugDrawAll();

// wip {
static inline
void DrawSolidPolygon( const floats &t, double seconds = 1.0, const b2Color &color = flatui::purple ) {
    DrawSolidPolygon( t.data(), t.size() / 2, seconds, color );
}
static inline
void DrawSolidPolygon( const b2Vec2s &t, double seconds = 1.0, const b2Color &color = flatui::purple ) {
    DrawSolidPolygon( &t[0].x, t.size(), seconds, color );
}

static inline
void DrawPolygon( const floats &t, double seconds = 1.0, const b2Color &color = flatui::purple ) {
    DrawPolygon( t.data(), t.size() / 2, seconds, color );
}
static inline
void DrawPolygon( const b2Vec2s &t, double seconds = 1.0, const b2Color &color = flatui::purple ) {
    DrawPolygon( &t[0].x, t.size(), seconds, color );
}

// }
