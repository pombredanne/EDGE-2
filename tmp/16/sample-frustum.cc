/* ------------------------------------------------------

 View Frustum Culling - Lighthouse3D

  -----------------------------------------------------*/

#include <eve/render.hpp>
#include <eve/camera.hpp>
#include <eve/window.hpp>

#include <geo/vec.hpp>
#include <geo/frustum.hpp>
#include <geo/plane.hpp>

#include <sstream>

#if 0
#include <hyde/keyboard.hpp>
#endif

float a = 0;

float nearP = 1.0f, farP = 100.0f;
float fov = 45, aspect = 1;

int frame=0, timebase=0;

int frustumOn = 1;
int spheresDrawn = 0;
int spheresTotal = 0;
int draw_debug = 0;

eve::frustum frustum;

void changeSize(int w, int h)
{
	aspect = float(w) / float( h > 0 ? h : 1 );

	// Reset the coordinate system before modifying
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	// Set the viewport to be the entire window
    glViewport( 0, 0, w, h );

	// Set the correct perspective.
	gluPerspective( fov, aspect, nearP, farP );
	glMatrixMode( GL_MODELVIEW );

	frustum.setup( fov, aspect, nearP, farP );
}

void render() {

	glColor3f(0,1,0);
	spheresTotal=0;
	spheresDrawn=0;

/*
	if(draw_debug&1)
	frustum.draw_points();
	if(draw_debug&2)
	frustum.draw_lines();
	if(draw_debug&4)
	frustum.draw_normals();
	if(draw_debug&8)
	frustum.draw_planes();
*/

	for (int i = -200; i < 200; i+=4)
			for(int k =  -200; k < 200; k+=4) {
				spheresTotal++;
				eve::vec3 a(i,0,k);

				bool ix = collide(frustum,a,0.5);

				if ( !frustumOn || ix ) {
					glPushMatrix();
					glTranslatef(i,0,k);
					glutSolidSphere(0.5,5,5);
					glPopMatrix();
					spheresDrawn++;
				}
			}

}


eve::vec3 p(0,0,5),l(0,0,0),u(0,1,0);


void input()
{
#if 0
	static eve::windows::keyboard keyboard(0);
	keyboard.update();

	if( keyboard.s.hold() ) {
		p = p + eve::vec3(0,0,1) * 0.1f;
		l = l + eve::vec3(0,0,1) * 0.1f;
	}
	else
	if( keyboard.w.hold() ) {
		p = p - eve::vec3(0,0,1) * 0.1f;
		l = l - eve::vec3(0,0,1) * 0.1f;
	}

	if( keyboard.d.hold() ) {
		p = p + eve::vec3(1,0,0)*0.1f;
		l = l + eve::vec3(1,0,0)*0.1f;
	}
	else
	if( keyboard.a.hold() ) {
		p = p - eve::vec3(1,0,0)*0.1f;
		l = l - eve::vec3(1,0,0)*0.1f;
	}

	if( keyboard.e.hold() ) {
		p = p + eve::vec3(0,1,0)*0.1f;
		l = l + eve::vec3(0,1,0)*0.1f;
	}
	else
	if( keyboard.c.hold() ) {
		p = p - eve::vec3(0,1,0)*0.1f;
		l = l - eve::vec3(0,1,0)*0.1f;
	}

	if( keyboard.r.trigger() ) {
		p = eve::vec3(0,0,5);
		l = eve::vec3(0,0,0);
		u = eve::vec3(0,1,0);
	}

	if( keyboard.f.trigger() )
		frustumOn = !frustumOn;

	if( keyboard.escape.trigger() )
		exit(0);

	if(keyboard.zero.trigger())
		draw_debug = 0;

	if(keyboard.one.trigger())
		draw_debug |= 1;

	if(keyboard.two.trigger())
		draw_debug |= 2;

	if(keyboard.three.trigger())
		draw_debug |= 4;

	if(keyboard.four.trigger())
		draw_debug |= 8;
#endif
}

#include <cassert>
#include <chrono>
#include <thread>

int main(int argc, char **argv)
{
	{
		using eve::vec3;
		eve::plane pl( vec3(0,0,0),vec3(1,0,0),vec3(1,1,0) );
		assert( distance( pl, vec3(1,1,1) ) == 1 );
	}

	eve::window win("eve::frustum", 0.75f);
	eve::camera camera;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	printf("Help Screen\n\nW,S,A,D,E,C: Move around\nR: Reset Position\nF: Turn frustum On/Off");

	double t = 0;
	auto t_start = std::chrono::high_resolution_clock::now();
	float fps = 0;

	while( win.is_open() ) {
		input();

		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.resize( win.w, win.h );
		camera.update();
		camera.apply();

		glLoadIdentity();
		gluLookAt(p.x,p.y,p.z,
			      l.x,l.y,l.z,
				  u.x,u.y,u.z);

		frustum.update(p,l,u);

		render();

		frame++;
		auto t_end = std::chrono::high_resolution_clock::now();
		float dt =  std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();
		t_start = t_end;
		t += dt;
		if (t > 1000) {
			t = 0;
			fps = frame;
			frame = 0;

			std::stringstream title;
			title << "Frustum:" << frustumOn << " Spheres Drawn:" << spheresDrawn << " Total Spheres:" << spheresTotal << " FPS:" << fps;

			win.title = title.str();
		}

		win.flush( dt );
	}

	return 0;
}

