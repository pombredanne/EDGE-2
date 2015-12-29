#ifndef EE_GLES_HPP
#define EE_GLES_HPP

#ifdef _WIN32
#include <GL/gl3w.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <SDL.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

extern "C" GLuint loadShader(GLenum type, const char *source);
extern "C" GLuint buildProgram(GLuint vertexShader, GLuint fragmentShader, const char * vertexPositionName);
extern "C" int initGL(int width, int height);
extern "C" void drawTriangle(float *translation);

#endif
