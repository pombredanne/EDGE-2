/*
 * lua.cpp -- Lua core, libraries and interpreter in a single file
 */

#define luaall_c

#define LUA_COMPAT_MODULE

#pragma warning(push, 0)
#pragma warning(disable: 4996) //crap msvc

// LUA
extern "C" {
#include <lapi.c>
#include <lcode.c>
#include <lctype.c>
#include <ldebug.c>
#include <ldo.c>
#include <ldump.c>
#include <lfunc.c>
#include <lgc.c>
#include <linit.c>
#include <llex.c>
#include <lmem.c>
#include <lobject.c>
#include <lopcodes.c>
#include <lparser.c>
#include <lstate.c>
#include <lstring.c>
#include <ltable.c>
#include <ltm.c>
#include <lundump.c>
#include <lvm.c>
#include <lzio.c>

#include <lauxlib.c>
#include <lbaselib.c>
#include <lbitlib.c>
#include <lcorolib.c>
#include <ldblib.c>
#include <liolib.c>
#include <lmathlib.c>
#include <loadlib.c>
#include <loslib.c>
#include <lstrlib.c>
#include <ltablib.c>
#include <lutf8lib.c>

//#include <lua.c>
//#include <luac.c>
}

// LPEG 100
extern "C" {
#define match match2
#define codenot codenot2
#include <lpeg/lpcap.c>
#include <lpeg/lpcode.c>
#include <lpeg/lpprint.c>
#include <lpeg/lptree.c>
#include <lpeg/lpvm.c>
}

// LUAFS 163
extern "C" {
#define pusherror pusherror2
#include <lfs/lfs.c>
}

/*
// glew and gl3w
extern "C" {
#define GLEW_STATIC
#include <glew/src/glew.c>
#include <gl3w/src/gl3w.c>
}

// STB
extern "C" {
#include <stb/stb_image.c>
#include <stb/stb_image_write.c>
}
*/

#pragma warning(pop)
