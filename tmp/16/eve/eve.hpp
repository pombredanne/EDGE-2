// eve, a game engine where graphics are low priority
// - rlyeh, mit licensed

#pragma once

// I will hate Windows forever {
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
// }

//#include <drecho/drecho.hpp>

// gl
#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#endif

// spatial
#include "spatial.hpp"

// all
#include "gl/context.hpp"
#include "gl/disable.hpp"
#include "gl/enable.hpp"
#include "gl/ext.hpp"
#include "gl/fbo.hpp"
//#include "font.hpp"
#include "camera.hpp"
#include "hexcolor.hpp"
#include "geometry.hpp"
#include "texture/gif.hpp"
#include "gl/gl.hpp"
//#include "texture/image.hpp"
//#include "imgui.hpp"
#include "gl/invert.hpp"
#include "lists.hpp"
#include "matrix.hpp"
#include "mesh/mesh.hpp"
#include "gl/nocopy.hpp"
//#include "os.hpp"
#include "mesh/obj/parser.hpp"
//#include "pixel.hpp"
#include "render.hpp"
#include "gl/shader.hpp"
#include "texture/sprite.hpp"
//#include "stash.hpp"
#include "style.hpp"
#include "texture/texture.hpp"
#include "gl/vbo.hpp"
#include "gl/viewport.hpp"
#include "window.hpp"

// texture

// mesh

// camera

/* audio
#include <soloud.h>
#include <soloud_speech.h>
#include <soloud_modplug.h>
#include <soloud_wav.h>
#include <soloud_wavstream.h>
#include <soloud_sfxr.h>
#include <soloud_thread.h>
*/

#include <functional>
#include <map>

#include "todoby.hpp"

// stupid windows may arise in any header of above
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include "gl4/gl4.h"
#include "gl4/gl4ext.h"

#include "texture/texture.hpp"

#define EVE_TODOBY TODOBY
#define EVE_ASSERT assert
#define EVE_TEST   dessert
#define EVE_SUITE  desserts

#define EVE_VERSION_NUMBER "0.0.0"

#if defined(SHIPPING)
#define EVE_DEBUG(...)
#define EVE_RELEASE(...)
#define EVE_SHIPPING(...)  __VA_ARGS__
#elif defined(_NDEBUG) || defined (NDEBUG)
#define EVE_DEBUG(...)
#define EVE_RELEASE(...) __VA_ARGS__
#define EVE_SHIPPING(...)
#else
#define EVE_DEBUG(...)   __VA_ARGS__
#define EVE_SHIPPING(...)
#define EVE_RELEASE(...)
#endif

#define EVE_VERSION_FULL   "eve-" EVE_DEBUG("debug") EVE_RELEASE("release") EVE_SHIPPING("shipping") "-" EVE_VERSION_NUMBER
// _MSC_VER __FUNCSIG__ __FUNCTION__
// G++ __PRETTY_FUNCTION__
// other __func__
#define _ eve::profiler raii(__func__)


// select current api version
namespace eve {

    using string = std::string;
    using strings = std::vector<std::string>;

    // console api
    using arguments = const eve::strings &;
    void console( const std::string &title, const std::string &prompt, const std::map< eve::string, std::function<void(arguments)> > &cb );

    // app api
    class app {
    public:
        app()
        {}
        app( int argc, const char **argv, const std::string &options = std::string() ) {
            init( argc, argv, options );
        }
        bool init( int argc, const char **argv, const std::string &options = std::string() );
        bool quit();
        bool open();
        bool pump();
    };

    // init
    bool init( int argc, const char **argv );
}
