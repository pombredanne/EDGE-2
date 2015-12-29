//fix transparent floor

#include <cmath>
#include <cstdlib>

#include <map>
#include <sstream>
#include <iostream>

#include "camera.hpp"
#include <eve/gl/gl.hpp>
#include <eve/gl/shader.hpp>
#include <eve/render.hpp>
#include <eve/window.hpp>
#include <eve/eve.hpp>

#include <GL/freeglut.h>

#if 1

#include <GLFW/glfw3.h>

namespace
{
    void init_glfw()
    {
        static struct once { once() {
            int argc = 0;
            glfwInit();
            std::atexit( glfwTerminate );

            // init data
            /*
            for( int i = 0; i < 4; ++i ) {
                if( eve::ls(".data").empty() ) {
                    eve::chdir("..");
                } else {
                    eve::chdir(".data");
                    break;
                }
            }
            */
        }} _1;
    }

    void init_glew()
    {
        static struct once { once() {
            // Initialize GLEW
            glewExperimental= GL_TRUE;
            GLenum err = glewInit ();
            if (GLEW_OK != err)
            {
                // Problem: glewInit failed, something is seriously wrong.
                std::cerr << "<eve/render/window.cpp> says: Error: " << glewGetErrorString (err) << std::endl;
                std::exit(1);
            }
            // Print some infos about user's OpenGL implementation
            if(0)
            {
                std::cout << "OpenGL Version String: " << glGetString (GL_VERSION) << std::endl;
                std::cout << "GLU Version String: " << gluGetString (GLU_VERSION) << std::endl;
                std::cout << "GLEW Version String: " << glewGetString (GLEW_VERSION) <<std::endl;
            }
        }} _2;
    }

    void init_gl()
    {
            int argc = 1;
            char *argv[] = { "dummy", 0 };
            glutInit(&argc, argv);

            // Enable Texture Mapping
            glEnable(GL_TEXTURE_2D);
            // Use trilinear interpolation (GL_LINEAR_MIPMAP_LINEAR)
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

            glClearColor( 1.f, 0.f, 1.f, 1.f ); //0.15f, 0.15f, 0.20f, 1.f);
            glClearDepth(1.0f);                                 // Depth Buffer Setup
            glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
            glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do
            glDepthMask(GL_TRUE);

            if( 1 )
            {
                /*
                GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
                GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
                GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
                GLfloat light_position[] = { 1.0, 1.0, 0.0, 0.0 };

                glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
                glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
                glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
                glLightfv (GL_LIGHT0, GL_POSITION, light_position);
*/
                GLfloat global_ambient[] = { 0.6f, 0.6f, 1.0, 1.0 };
                glLightModelfv (GL_LIGHT_MODEL_AMBIENT, global_ambient);

                // Enable Lighting
                glDisable(GL_LIGHTING);
                glDisable(GL_LIGHT0);

                //glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
                // Enable Material Coloring (color texture maps)
                //glMaterial will control the polygon's specular and emission colours and the ambient and diffuse will both be set using glColor
                glEnable(GL_COLOR_MATERIAL);
                glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
            }


            glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading


            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice Perspective Correction

            glEnable( GL_LINE_SMOOTH );
            glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
            glLineWidth( 1.0f );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

            /* decal */
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            glDisable( GL_CULL_FACE ); //glDisable
            //glCullFace( GL_FRONT );
            //glCullFace( GL_BACK );

            /* small opt */

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
    }
}

namespace {
    inline GLFWwindow *to_glfw( void *ptr ) {
        return (GLFWwindow *)ptr;
    }
}

namespace eve
{

        window::window( const std::string &_title, float proportional_size, bool is_fullscreen )
        : w(640), h(480), is_fullscreen(false), is_closing( false ), title( _title ), t(0), dt(0),
        fbw(0), fbh(0)
        {
            init_glfw();

            parent_w = glfwGetVideoMode( glfwGetPrimaryMonitor() )->width;
            parent_h = glfwGetVideoMode( glfwGetPrimaryMonitor() )->height;

            {
                // reinit :P

                proportional_size = ( proportional_size <= 0 ? 1 : proportional_size );

                w = parent_w * proportional_size;
                h = parent_h * proportional_size;
            }

#if 0
            //wglSwapIntervalEXT(1);
            glfwOpenWindowHint( GLFW_ACCUM_RED_BITS, 16 );
            glfwOpenWindowHint( GLFW_ACCUM_GREEN_BITS, 16 );
            glfwOpenWindowHint( GLFW_ACCUM_BLUE_BITS, 16 );
            glfwOpenWindowHint( GLFW_ACCUM_ALPHA_BITS, 16 );
            if( glfwOpenWindow( w, h, 0,0,0,0,0,0, fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW ) != GL_TRUE )
            {
                std::cerr << "<eve/render/window.hpp> says: Cant create " << w << 'x' << h << " window!" << std::endl;
                exit(1);
            }
#endif

            pimpl = (void *)glfwCreateWindow(w, h, title.c_str(), NULL, NULL);
            if (!pimpl)
            {
                glfwTerminate();
                std::cerr << "<eve/window.hpp> says: Can't create window! (" << w << ',' << h << ')' << std::endl;
                exit(1);
            }

            glfwMakeContextCurrent( to_glfw(pimpl) );

            glfwSetWindowTitle( to_glfw(pimpl), title.c_str() );

            resize( w, h );

            init_glew();

            init_gl();
        }

        void window::toggle_fullscreen()
        {}

        bool window::is_blur() const
        {
            return glfwGetWindowAttrib( to_glfw(pimpl), GLFW_ICONIFIED ) != 0;
        }

        void window::flush( float dt )
        {
            glfwSetWindowTitle( to_glfw(pimpl), title.c_str() );
            glfwSwapInterval(1);
            glfwSwapBuffers( to_glfw(pimpl) );
            glfwPollEvents();

            this->dt = dt;
            t += this->dt;

            int nw, nh;
            glfwGetWindowSize( to_glfw(pimpl), &nw, &nh );
            resize( nw, nh );
        }

        bool window::is_open() const
        {
            if( is_closing )
                return false;

            return !glfwWindowShouldClose( to_glfw(pimpl) );

//            frames++;

/*
            if( win->dt.s() > 1.0 )
            {
                win->fps = win->frames / win->dt.s();

                // todo: spf too

                eve::format title("%s // %3.2f fps", win->title.c_str(), win->fps );

                #if defined( kBooTargetWindows ) && defined( UNICODE )
                glutSetWindowTitle( (const char *)( std::wstring( title.begin(), title.end() ).c_str() ) );
                #else
                glutSetWindowTitle( title.c_str() );
                #endif

                win->frames = 0;
                win->dt.reset();
            }
*/

//            eve::wink();

            return true;
        }

        void window::resize( int x, int y )
        {
            w = (x <= 0 ? 0 : x);
            h = (y <= 0 ? 1 : y);

            glfwSetWindowSize( to_glfw(pimpl), w, h );
            glfwSetWindowPos( to_glfw(pimpl), ( parent_w - w ) / 2, ( parent_h - h ) / 2 );

            glfwGetFramebufferSize(to_glfw(pimpl), &fbw, &fbh);
            glViewport(0, 0, fbw, fbh);
        }

        void window::close()
        {
            //glfwCloseWindow();
            is_closing = true;
        }

} // ns eve


#endif
