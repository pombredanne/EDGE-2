// missing:
// [ ] show bones
// [ ] show skeleton
// [ ] show normals
// [ ] show nodes

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "opengl.hpp"
#include "assview.hpp"
#include "asset.hpp"
#include "app.hpp"
#include "animator.hpp"
#include "debugdraw.hpp"

#include <apathy/apathy.hpp>
#include <hyde/hyde.hpp>

hyde::windows::keyboard keymatrix(0);

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CLAMP(x,a,b) MIN(MAX(x,a),b)

/*
 * Boring UI and GLUT hooks.
 */

#include "getopt.h"

#define ISOMETRIC 35.264	// true isometric view
#define DIMETRIC 30		// 2:1 'isometric' as seen in pixel art

aiScene *g_scene = NULL;
int lasttime = 0;
aiAnimation *curanim = NULL;
int animfps = 30, animlen = 0;
float animtick = 0;
int playing = 1;
unsigned quantizers_pos = 0, quantizers_pos_max = 6;
unsigned quantizers_rot = 0, quantizers_rot_max = 1;
unsigned quantizers_sca = 0, quantizers_sca_max = 2;

int showhelp = 0;
int doplane = 1;
int docamera = 0;
int doalpha = 0;
int dowire = 0;
int dotexture = 1;
int dobackface = 1;
int dotwosided = 1;
int doperspective = 1;

int screenw = 800, screenh = 600;
int mousex, mousey, mouseleft = 0, mousemiddle = 0, mouseright = 0;

int gridsize = 3;
float mindist = 1;
float maxdist = 10;

float light_position[4] = { -1, 2, 2, 0 };

camera_t camera = { 3, 45, -DIMETRIC, { 0, 1, 0 } };

	float zoom = 1;



void setanim(int i)
{
	if (!g_scene) return;
	if (g_scene->mNumAnimations == 0) return;
	i = MIN(i, g_scene->mNumAnimations - 1);
	curanim = g_scene->mAnimations[i];
	animlen = animationlength(curanim);
	animfps = 30;
	animtick = 0;
	if (animfps < 1)
		animfps = 30;
}

bool load_assets( const std::string &resource ) {
	asset the_asset;

    bool is_folder = !apathy::file( resource ).is_file();

    if( is_folder ) {
		if( !the_asset.load( resource )) {
			return false;
		}
		if( !the_asset.txt.empty() ) {
	      	load_animation( the_asset.txt.c_str() );
	    }
	    if( !the_asset.geo.empty() ) {
			std::ifstream ifs( the_asset.geo.c_str(), std::ios::binary );
			std::stringstream ss;
			ss << ifs.rdbuf();
			the_asset.data = ss.str();
	    }

		strcpy(basedir(), the_asset.geo.c_str());
		char *p = strrchr(basedir(), '/');
		if (!p) p = strrchr(basedir(), '\\');
		if (!p) strcpy(basedir(), ""); else p[1] = 0;
    }
 	else {
 		// nff geometry string instead?
 		the_asset.data = resource;
    }

	if( !the_asset.data.empty() ) {
		int flags = aiProcess_Triangulate;
		flags |= aiProcess_JoinIdenticalVertices;
		flags |= aiProcess_GenSmoothNormals;
		flags |= aiProcess_GenUVCoords;
		flags |= aiProcess_TransformUVCoords;
		flags |= aiProcess_RemoveComponent;

		flags = 0
       //| aiProcess_FlipUVs
       | aiProcessPreset_TargetRealtime_MaxQuality
       //| aiProcessPreset_TargetRealtime_Quality
       //| aiProcessPreset_TargetRealtime_Fast
       //| aiProcess_ConvertToLeftHanded // D3D
       | aiProcess_LimitBoneWeights //(4, defined as AI_LMW_MAX_WEIGHTS in config.h)
	   | aiProcess_JoinIdenticalVertices 
	   | aiProcess_RemoveRedundantMaterials
       | aiProcess_ImproveCacheLocality 
       | aiProcess_OptimizeGraph
       | aiProcess_OptimizeMeshes
       | aiProcess_SplitLargeMeshes;

		glutSetWindowTitle(the_asset.dir.c_str());

		g_scene = (aiScene*) aiImportFileFromMemory(the_asset.data.c_str(), the_asset.data.size(), flags, 
				the_asset.dae.size() ? "dae" : (the_asset.fbx.size() ? "fbx" : "nff") );
		if (g_scene) {
			printscene(g_scene);
			initscene(g_scene);

			float radius = measurescene(g_scene, camera.center);
			camera.distance = radius * 2 * zoom;
			gridsize = (int)radius + 1;
			mindist = radius * 0.1;
			maxdist = radius * 10;

			setanim(0);

			return true;
		} else {
			fprintf(stderr, "cannot import scene: '%s'\n", the_asset.dir.c_str());
		}
	}

	return false;
}

void perspective(float fov, float aspect, float znear, float zfar)
{
	fov = fov * 3.14159 / 360.0;
	fov = tan(fov) * znear;
	glFrustum(-fov * aspect, fov * aspect, -fov, fov, znear, zfar);
}

void orthogonal(float fov, float aspect, float znear, float zfar)
{
	glOrtho(-fov * aspect, fov * aspect, -fov, fov, znear, zfar);
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) mouseleft = state == GLUT_DOWN;
	if (button == GLUT_MIDDLE_BUTTON) mousemiddle = state == GLUT_DOWN;
	if (button == GLUT_RIGHT_BUTTON) mouseright = state == GLUT_DOWN;
	mousex = x;
	mousey = y;
}

void motion(int x, int y)
{
	int dx = x - mousex;
	int dy = y - mousey;
	if (mouseleft) {
		camera.yaw -= dx * 0.3;
		camera.pitch -= dy * 0.2;
		if (camera.pitch < -85) camera.pitch = -85;
		if (camera.pitch > 85) camera.pitch = 85;
		if (camera.yaw < 0) camera.yaw += 360;
		if (camera.yaw > 360) camera.yaw -= 360;
	}
	if (mousemiddle || mouseright) {
		camera.distance += dy * 0.01 * camera.distance;
		if (camera.distance < mindist) camera.distance = mindist;
		if (camera.distance > maxdist) camera.distance = maxdist;
	}
	mousex = x;
	mousey = y;
	glutPostRedisplay();
}

void togglefullscreen(void)
{
	static int oldw = 100, oldh = 100;
	static int oldx = 0, oldy = 0;
	static int isfullscreen = 0;
	if (!isfullscreen) {
		oldw = screenw;
		oldh = screenh;
		oldx = glutGet(GLUT_WINDOW_X);
		oldy = glutGet(GLUT_WINDOW_Y);
		glutFullScreen();
	} else {
		glutPositionWindow(oldx, oldy);
		glutReshapeWindow(oldw, oldh);
	}
	isfullscreen = !isfullscreen;
}

int get_game_time() {
    double multiplier = 1.0;
    {
	    if( GetAsyncKeyState(VK_MENU) & 0x8000 ) multiplier *= 2;
	    if( GetAsyncKeyState(VK_CONTROL) & 0x8000 ) multiplier *= 5;
    }
    double dt = glutGet(GLUT_ELAPSED_TIME) * multiplier;
	if( dt > 1000 ) {
		dt = 1000;
	}
	if( dt < 1 ) {
		dt = 1;
	}
	return int(dt);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27: case 'Q': case 'q': glutLeaveMainLoop(); break;
	case 'H':
	case 'h': case '?': showhelp = !showhelp; break;
	case 'F':
	case 'f': togglefullscreen(); break;
	case 'i': 
	case 'I': docamera = docamera^1; doperspective = 0; camera.yaw = 45; camera.pitch = docamera ? -ISOMETRIC : -DIMETRIC; break;
	case 'P':
	case 'p': doperspective = !doperspective; break;
	case '0': animtick = 0; animfps = 30; break;
	case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8':
	case '9': setanim(key - '1'); break;
	case ' ': playing = !playing; break;
	case '.': animtick = floor(animtick) + 1; break;
	case ',': animtick = floor(animtick) - 1; break;
	case '[': animfps = MAX(5, animfps-5); break;
	case ']': animfps = MIN(60, animfps+5); break;
	case 'G':
	case 'g': doplane = !doplane; break;
    case 'T':
	case 't': dotexture = !dotexture; break;
	case 'A': doalpha--; if(doalpha<0) doalpha = 4; break;
	case 'a': doalpha++; if(doalpha>4) doalpha = 0; break;
	case 'W':
	case 'w': dowire = !dowire; break;
	case 'B':
	case 'b': dobackface = !dobackface; break;
	case 'L':
	case 'l': dotwosided = !dotwosided; break;
	}

	if (playing)
		lasttime = get_game_time();

	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1: showhelp = !showhelp; break;
	case GLUT_KEY_F4: glutLeaveMainLoop(); break;
	case GLUT_KEY_F5: quantizers_pos++; if(quantizers_pos > quantizers_pos_max) quantizers_pos = 0; break;
	case GLUT_KEY_F6: quantizers_rot++; if(quantizers_rot > quantizers_rot_max) quantizers_rot = 0; break;
	case GLUT_KEY_F7: quantizers_sca++; if(quantizers_sca > quantizers_sca_max) quantizers_sca = 0; break;
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	screenw = w;
	screenh = h;
	glViewport(0, 0, w, h);
}

void display(void)
{
	int i;
	char buf[256];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	debugclear();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (doperspective)
		perspective(50, (float)screenw/screenh, mindist/5, maxdist*5);
	else
		orthogonal(camera.distance/2, (float)screenw/screenh, mindist/5, maxdist*5);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glTranslatef(0, 0, -camera.distance);
	glRotatef(-camera.pitch, 1, 0, 0);
	glRotatef(-camera.yaw, 0, 1, 0);
	glTranslatef(-camera.center[0], -camera.center[1], -camera.center[2]);

	if (dotexture)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	if (dowire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (dobackface)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, dotwosided);

	doalpha = CLAMP(doalpha, 0, 4);
	switch (doalpha) {
	// No alpha transparency.
	case 0:
		if (g_scene) drawscene(g_scene);
		break;

	// Alpha test only. Always correct, but aliased and ugly.
	case 1:
		glAlphaFunc(GL_GREATER, 0.2);
		glEnable(GL_ALPHA_TEST);
		if (g_scene) drawscene(g_scene);
		glDisable(GL_ALPHA_TEST);
		break;

	// Quick-and-dirty hack: render with both test and blend.
	// Background may leak through depending on drawing order.
	case 2:
		glAlphaFunc(GL_GREATER, 0.2);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		if (g_scene) drawscene(g_scene);
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		break;

	// For best looking alpha blending, render twice.
	// Solid parts first to fill the depth buffer.
	// Transparent parts after, with z-write disabled.
	// Background is safe, but internal blend order may be wrong.
	case 3:
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_EQUAL, 1);
		if (g_scene) drawscene(g_scene);

		glAlphaFunc(GL_LESS, 1);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		if (g_scene) drawscene(g_scene);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		break;

	// If we have a multisample buffer, we can get 'perfect' transparency
	// by using alpha-as-coverage. This does have a few limitations, depending
	// on the number of samples available you'll get banding or dithering artefacts.
	case 4:
		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		if (g_scene) drawscene(g_scene);
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		break;
	}

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	if (doplane) {
		glBegin(GL_LINES);
		glColor4f(0.4, 0.4, 0.4, 1);
		for (i = -gridsize; i <= gridsize; i ++) {
			glVertex3f(i, 0, -gridsize); glVertex3f(i, 0, gridsize);
			glVertex3f(-gridsize, 0, i); glVertex3f(gridsize, 0, i);
		}
		glEnd();
	}

	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenw, screenh, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	keymatrix.update();

	glColor4f(1, 1, 1, 1);
	if (g_scene) {
		sprintf(buf, "%d meshes; %d vertices; %d faces ", meshcount, vertexcount, facecount);
		drawstring(8, 18+0, buf);
		if (curanim) {
			sprintf(buf, "frame %03d / %03d (%d fps)", (int)animtick+1, animlen, animfps);
			drawstring(8, 18+20, buf);
		}
	} else {
		drawstring(8, 18, "No model loaded!");

		static std::string path = "./";
		static std::vector<std::string> folders;
		if( folders.empty() ) {

			asset the_asset;
			std::string search[] = { "./4047/40/data/", "../../", "../40-space-vikings/data/", "../bin/data/" };
			if( the_asset.load(search[0] + "models/demon_boss_v2/") ) {
			}
			else
			if( the_asset.load(search[1] + "models/demon_boss_v2/") ) {
				search[0] = search[1];
			}

			apathy::pushd();
			apathy::chdir(path); //search[0]);

			apathy::set sorted; //*/models*;*/joints*;*/worlds*" );
			sorted.insert("..");
			for( auto &d : apathy::lsd( "*" ) ) {
				if( d.size() >= 2 && d.substr(0, 2) == "./" )
				sorted.insert(d.substr(2));
				else
				sorted.insert(d);
			}

			folders.clear(); // = std::vector<std::string>( sorted.begin(), sorted.end() );
			for( auto &s : sorted ) {
				folders.push_back( s + '/' );
			}

			sorted.clear();
			for( auto &d : apathy::lsf( "*" ) ) {
				if( d.size() >= 2 && d.substr(0, 2) == "./" )
				sorted.insert(d.substr(2));
				else
				sorted.insert(d);
			}

			for( auto &s : sorted ) {
				folders.push_back( s );
			}

			apathy::popd();
		}
		int choice = debugmenu( folders );
		if( choice >= 0 && keymatrix.enter.trigger() ) {
			std::cout << "chosen " << folders[choice] << std::endl;
			if( folders[choice] == ".." ) {
				path = "../";
				folders.clear();
			} else {
				load_assets( folders[choice] );
				// and then in case load_assets(); failed...
				path = folders[choice];
				folders.clear();
			}
		}
	}

	if (showhelp) {
		#define Y(n) 18+40+n*16
		glColor4f(1, 1, 0.5, 1);

		drawstring(8, Y(0), std::string() + "h - help screen");
		drawstring(8, Y(1), std::string() + "a - change transparency mode " + std::to_string(doalpha));
		drawstring(8, Y(2), std::string() + "t - toggle textures " + std::string(dotexture ? "(on)":"(off)"));
		drawstring(8, Y(3), std::string() + "w - toggle wireframe " + std::string(dowire ? "(on)":"(off)") );
		drawstring(8, Y(4), std::string() + "b - toggle backface culling " + std::string(dobackface ? "(on)":"(off)"));
		drawstring(8, Y(5), std::string() + "l - toggle two-sided lighting " + std::string(dotwosided ? "(on)":"(off)"));
		drawstring(8, Y(6), std::string() + "g - toggle ground plane " + std::string(doplane ? "(on)":"(off)"));
		drawstring(8, Y(7), std::string() + "p - toggle orthogonal/perspective camera " + std::string(doperspective ? "(on)":"(off)"));
		drawstring(8, Y(8), std::string() + "i - toggle dimetric(2:1)/isometric camera " + std::string(docamera ? "(on)":"(off)"));
		drawstring(8, Y(9), std::string() + "F5 - quant position " + std::string(quantizers_pos ? "(on) " : "(off) ") + std::to_string(quantizers_pos) );
		drawstring(8, Y(10), std::string() + "F6 - quant rotation " + std::string(quantizers_rot ? "(on) " : "(off) ") + std::to_string(quantizers_rot) );
		drawstring(8, Y(11), std::string() + "F7 - quant scale " + std::string(quantizers_sca ? "(on) " : "(off) ") + std::to_string(quantizers_sca) );

		if (1|| curanim) {
			drawstring(8, Y(13), std::string() + "space - play/pause animation " + std::string(playing ? "(on)":"(off)"));
			drawstring(8, Y(14), std::string() + "[ and ] - change animation playback speed " + std::to_string(animfps));
			drawstring(8, Y(15), std::string() + ", and . - single step animation " + std::to_string(animtick));

			drawstring(8, Y(17), std::string() + "cursors,shift,z,x,c - anim controller (biped)");
		}
	}

	glutSwapBuffers();

	i = glGetError();
	if (i) fprintf(stderr, "opengl error: %d\n", i);
}

void usage(void)
{
	fprintf(stderr, "usage: assview [-geometry WxH] [options] asset.dae\n");
	fprintf(stderr, "\t-i\tdimetric (2:1) camera\n");
	fprintf(stderr, "\t-I\ttrue isometric camera\n");
	fprintf(stderr, "\t-a\talpha transparency mode; use more times for higher quality.\n");
	fprintf(stderr, "\t-b\tdon't render backfaces\n");
	fprintf(stderr, "\t-g\trender ground plane\n");
	fprintf(stderr, "\t-l\tone-sided lighting\n");
	fprintf(stderr, "\t-t\tdon't render textures\n");
	fprintf(stderr, "\t-w\trender wireframe\n");
	fprintf(stderr, "\t-c r,g,b\tbackground color\n");
	fprintf(stderr, "\t-r n\trotate camera n degrees (yaw)\n");
	fprintf(stderr, "\t-p n\tpitch camera n degrees\n");
	fprintf(stderr, "\t-z n\tzoom camera n times\n");
	fprintf(stderr, "\t-f n\trender animation at frame n\n");
	exit(1);
}

void update(int data) {
	int time, timestep;

	time = get_game_time();
	timestep = 1 + time - lasttime;
	lasttime = time;

	animate(timestep);
    glutTimerFunc(1000/animfps, update, -1); // call drive() again in 16 milliseconds

	if (g_scene) {
		if (curanim) {
			if(1) { //if (playing) {
				//animtick = animtick + (timestep/1000.0) * animfps;
			}
			while (animtick < 0) animtick += animlen;
			while (animtick >= animlen) animtick -= animlen;
			animatescene(g_scene, curanim, animtick);
		}
	}

	int i = glGetError();
	if (i) fprintf(stderr, "opengl error: %d\n", i);

    glutPostRedisplay();
}


#include <thread>
#include <string>
#include <fstream>

#include "asset.hpp"

int main(int argc, char **argv)
{
#ifdef SHIPPING
	fclose(stdout);
	fclose(stderr);
#endif

	float clearcolor[4] = { 0.22, 0.22, 0.22, 1.0 };
	int c;

	glutInitWindowPosition(50, 50+24);
	glutInitWindowSize(screenw, screenh);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	while ((c = getopt(argc, argv, "iIgtawblc:r:p:z:f:")) != -1) {
		switch (c) {
		case 'i': docamera = !docamera; doperspective = 0; camera.yaw = 45; camera.pitch = docamera ? -ISOMETRIC : -DIMETRIC; break;
		case 'g': doplane = !doplane; break;
		case 't': dotexture = !dotexture; break;
		case 'a': doalpha++; if(doalpha > 4) doalpha = 0; break;
		case 'w': dowire = !dowire; break;
		case 'b': dobackface = !dobackface; break;
		case 'l': dotwosided = !dotwosided; break;
		case 'c': sscanf(optarg, "%g,%g,%g", clearcolor+0, clearcolor+1, clearcolor+2); break;
		case 'r': camera.yaw = atof(optarg); break;
		case 'p': camera.pitch = atof(optarg); break;
		case 'z': zoom = atof(optarg); break;
		case 'f': animtick = atof(optarg); break;
		default: usage(); break;
		}
	}

	glutCreateWindow("Asset Viewer");
	screenw = glutGet(GLUT_WINDOW_WIDTH);
	screenh = glutGet(GLUT_WINDOW_HEIGHT);

	setup_gl_context();

	initchecker();
	inittextureless();

	std::string folder;
	if (optind < argc) {
		folder = argv[1];
	}

	if( load_assets( folder ) ) {
		// ok :)
	}

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_NORMALIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glClearColor(clearcolor[0], clearcolor[1], clearcolor[2], clearcolor[3]);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutTimerFunc(30, update, -1);

	glutMainLoop();

	return 0;
}
