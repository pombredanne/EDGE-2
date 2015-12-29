#pragma once

#include <GL/glew.h>

#ifndef GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT 0x8CD8
#endif

#ifdef _WIN32
#   include <GL/wglew.h>
#   include <winsock2.h>
#   include <windows.h>
#   pragma comment( lib, "user32.lib" )
#   pragma comment( lib, "advapi32.lib" )
#   pragma comment( lib, "gdi32.lib" )
#   pragma comment( lib, "winmm.lib" )
#   pragma comment( lib, "opengl32.lib" )
#   pragma comment( lib, "glu32.lib" )
#endif

#include <GL/gl.h>
#include <GL/glu.h>
