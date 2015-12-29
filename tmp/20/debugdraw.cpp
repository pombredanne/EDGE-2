// missing:
// [ ] show bones
// [ ] show skeleton
// [ ] show normals
// [ ] show nodes

#include "opengl.hpp"
#include "debugdraw.hpp"
#include <eve/geometry.hpp>

int x_position = 0;
int y_position = 0;

void debugclear() {
	x_position = 0;
	y_position = 0;
}

void gotoxy( float x, float y ) {
	glRasterPos2f(x+0.375, y+0.375);
}

float getcol( float col ) {
	return 11 + col*16;
}

void drawstring(float x, float y, const std::string &s) {
	gotoxy( x, y );
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *)s.c_str());
}

void debugstring( const std::string &s ) {
	eve::geometry::text2d( getcol(x_position), getcol(y_position++), s );
#if 0
//  3d pos, 2d text
	gotoxy( getcol(x_position), getcol(y_position++) );
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *)s);
#endif
}

#include <windows.h>
#include <utility>
#include <map>
#include <vector>
#include <string>

int &get_selection( const std::vector<std::string> &options ) {
	static std::map< std::vector<std::string>, int > map;
	auto found = map.find( options );
	if( found != map.end() ) {
		return found->second;
	}
	return (map[ options ] = map[ options ]) = 0;
}

int debugmenu( const std::vector<std::string> &options ) {
	if( options.empty() ) {
		return 0;
	}

	debugstring("");
	debugstring("");

	static bool prev_up = 0, prev_down = 0;
	bool up = (GetAsyncKeyState(VK_UP) & 0x8000) > 0, up0 = up;
	bool down = (GetAsyncKeyState(VK_DOWN) & 0x8000) > 0, down0 = down;

	up &= ~prev_up;
	down &= ~prev_down;

	prev_up = up0;
	prev_down = down0;

	int &s = get_selection( options );
	if( up ) s--;
	if( down ) s++;

	if( s <= 0 ) {
		s = 0;
	}
	else
	if( s >= options.size() ) {
		s = options.size() - 1;
	}

	for( size_t i = 0, e = options.size(); i < e; ++i) {
		if( i == s ) {
			debugstring( (std::string(">> ") + options[i]).c_str() );
		} else {
			debugstring( (std::string(" - ") + options[i]).c_str() );
		}
	}

	return GetAsyncKeyState( VK_RETURN ) & 0x8000 ? s : -1;
}

