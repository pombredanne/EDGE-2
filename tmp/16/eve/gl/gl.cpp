#include "gl.hpp"

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GL/glu.h>

#include <cstdlib>

#ifdef _MSC_VER
#include <windows.h>
#endif

namespace eve
{
    void init_ogl()
    {
    //    glewInit();
    //    glutInit();
    //    glfwInit();
    }

    std::string callstack() {
        return std::string();
    }

    void die() {
        std::exit(-1);
    }

    void check_ogl(const char *file, int line)
    {
        GLenum err = glGetError();

        if (err != GL_NO_ERROR)
        {
            const char *error = (const char *) gluErrorString(err);

            std::stringstream hex, ss;

            // ridiculous C++
            hex << "0x" << std::hex << std::setw(4) << std::setfill('0') << err;

            ss << "OpenGL error #" << hex.str();
            ss << ( error ? std::string(": ") + error : std::string("") ) << "; ";
            ss << "seen at " << file << ":" << line << std::endl;
            ss << callstack() << std::endl;

            std::cerr << ss.str() << std::endl;

#ifdef _MSC_VER
            if( IsDebuggerPresent() ) {
                DebugBreak();
            }
#endif

            /*
            debugger(ss.str());
            die(-1);
            */
        }
    }
}
