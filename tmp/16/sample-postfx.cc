/* OpenGL example code - fbo & fxaa
 *
 * render the cube from the perspective example to a texture and
 * apply fxaa antialiasing to it.
 *
 * Autor: Jakob Progsch
 */

#include <eve/eve.hpp>

//#include <GLXW/glxw.h>
#include <GLFW/glfw3.h>

//glm is used to create perspective and transform matrices
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <eve/eve.hpp>


int width = 0;
int height = 0;
GLFWwindow *window = 0;

int setupgl( int width_, int height_ ) {
    if(glfwInit() == GL_FALSE) {
        std::cerr << "failed to init GLFW" << std::endl;
        return 0;
    }

    // select opengl version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // create a window
    if((window = glfwCreateWindow(width = width_, height = height_, "05fbo_fxaa", 0, 0)) == 0) {
        std::cerr << "failed to open window" << std::endl;
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

//    if(glxwInit()) {
    if(glewInit(),0) {
        std::cerr << "failed to init GL3W" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }

    return 1;
}
int setupgl() {
    return setupgl( 640, 480 );
}
void swapgl() {
    glfwSwapBuffers(window);
}
void quitgl() {
    // delete the created objects
    glfwDestroyWindow(window);
    glfwTerminate();
}
bool pollgl() {
    bool keep = !glfwWindowShouldClose(window);
    glfwPollEvents();
    return keep;
}

#include <fstream>
#include <sstream>
std::string glslf(int version, const std::string &filename ) {
    std::ifstream ifs( filename.c_str() );
    std::stringstream ss;
    ss << "#version " << version << std::endl << ifs.rdbuf();
    return ss.str();
}

int main() {

    if( !setupgl() ) {
        return 1;
    }

    //
    mesh<solid_cube> vao[100];

    for( auto &v : vao ) {
        int r = 10 + (rand() % 245);
        int g = 10 + (rand() % 245);
        int b = 10 + (rand() % 245);
        v.data.setup( color3f(r/255.f,g/255.f,b/255.f) );
        v.tf.position = glm::vec3((r-127)/10,(g-127)/10,(b-127)/10);
        v.tf.update();
    }

    postfx fx( width, height );

    // :(
    //fx.passes.push_back( new Pass("FxaaPass", "FxaaPass.fs" ) );
    //fx.passes.push_back( new Pass("LensPass", "LensPass.fs" ) );
    //fx.passes.push_back( new Pass("ScanlinesPass", "ScanlinesPass.fs" ) );
    //fx.passes.push_back( new Pass("GrainPass", "GrainPass.fs" ) );
    //fx.passes.push_back( new Pass("ToonPass", "ToonPass.fs", "ToonPass.vs" ) ); // no idea
    //fx.passes.push_back( new Pass("VignettePass", "VignettePass.fs" ) ); // gl_FragCoord
    //fx.passes.push_back( new Pass("PostPass", "PostPass.fs" ) ); // gl_FragCoord
    //fx.passes.push_back( new Pass("BarrelPass", "BarrelPass.fs" ) );
    //fx.passes.push_back( new Pass("BumpMappingUnparametrized", "BumpMappingUnparametrized.fs" ) );
    //fx.passes.push_back( new Pass("CRTPass", "CRTPass.fs" ) );
    //fx.passes.push_back( new Pass("DirtyOldTvPass", "DirtyOldTvPass.fs" ) );
    //fx.passes.push_back( new Pass("LensPass", "LensPass.fs" ) );
    //fx.passes.push_back( new Pass("SSAO2Pass", "SSAO2Pass.fs" ) );
    //fx.passes.push_back( new Pass("VCRPass", "VCRPass.fs" ) );
    // }

    // :) {
    fx.passes.push_back( new Pass("BleachBypassPass", "BleachBypassPass.fs" ) );
    fx.passes.push_back( new Pass("ContrastPass", "ContrastPass.fs" ) );
    fx.passes.push_back( new Pass("DofAltPass", "DofAltPass.fs" ) );
    fx.passes.push_back( new Pass("DofPass", "DofPass.fs" ) );
    fx.passes.push_back( new Pass("EdgePass", "EdgePass.fs" ) );
    fx.passes.push_back( new Pass("FXAA3Pass", "FXAA3Pass.fs" ) );
    fx.passes.push_back( new Pass("HorizontalTiltShifPass", "HorizontalTiltShifPass.fs" ) );
    fx.passes.push_back( new Pass("KaleidoscopePass", "KaleidoscopePass.fs" ) );
    fx.passes.push_back( new Pass("LimbDarkeningPass", "LimbDarkeningPass.fs" ) );
    fx.passes.push_back( new Pass("NoiseWarpPass", "NoiseWarpPass.fs" ) ); // :?
    fx.passes.push_back( new Pass("PixelatePass", "PixelatePass.fs" ) );
    fx.passes.push_back( new Pass("RGBShiftPass", "RGBShiftPass.fs" ) );
    fx.passes.push_back( new Pass("SSAOPass", "SSAOPass.fs" ) );
    fx.passes.push_back( new Pass("VerticalTiltShifPass", "VerticalTiltShifPass.fs" ) );
    fx.passes.push_back( new Pass("ZoomBlurPass", "ZoomBlurPass.fs" ) );
    // }

//:? fx.passes.push_back( new Pass("RimHighlightingPass", "RimHighlightingPass.fs", "RimHighlightingPass.vs" ) );
//:? fx.passes.push_back( new Pass("BloomPass", "2BloomPass.fs" ) );
//:? fx.passes.push_back( new Pass("ConvolutionPass", "2ConvolutionPass.fs" ) );
//:? fx.passes.push_back( new Pass("FakeSSSPass", "2FakeSSSPass.fs" ) );
//:? fx.passes.push_back( new Pass("GodRaysPass", "2GodRaysPass.fs" ) );
//:? fx.passes.push_back( new Pass("LUTPass", "2LUTPass.fs" ) );

    for( auto &pass : fx.passes ) {
        pass->enabled = false;
    }

    ::camera camera;

    bool enabled = false;
    bool keys[256] = {0};
    while( pollgl() ) {

        glfwSetWindowTitle(window, ( std::string("") + fx.list('/') ).c_str() );

        // get the time in seconds
        float t = glfwGetTime();

        // toggle fxaa on/off with space
#define KEY(X,CH,NUM) \
        if(glfwGetKey(window, X) && !keys[CH] && NUM < fx.passes.size() ) fx.passes[NUM]->enabled ^= true; \
        keys[CH] = glfwGetKey(window, X);

        KEY(GLFW_KEY_1,'1',0);
        KEY(GLFW_KEY_2,'2',1);
        KEY(GLFW_KEY_3,'3',2);
        KEY(GLFW_KEY_4,'4',3);
        KEY(GLFW_KEY_5,'5',4);
        KEY(GLFW_KEY_6,'6',5);
        KEY(GLFW_KEY_7,'7',6);
        KEY(GLFW_KEY_8,'8',7);
        KEY(GLFW_KEY_9,'9',8);
        KEY(GLFW_KEY_0,'0',9);
        KEY(GLFW_KEY_Q,'q',10);
        KEY(GLFW_KEY_W,'w',11);
        KEY(GLFW_KEY_E,'e',12);
        KEY(GLFW_KEY_R,'r',13);
        KEY(GLFW_KEY_T,'t',14);
        KEY(GLFW_KEY_Y,'y',15);
        KEY(GLFW_KEY_U,'u',16);
        KEY(GLFW_KEY_I,'i',17);
        KEY(GLFW_KEY_O,'o',18);
        KEY(GLFW_KEY_P,'p',19);

        // update camera and set the uniform
        float sin_t = glm::radians(90.0f)*sin(t/10);
        camera.position = glm::vec3(0.0f, 0.0f, -1.0f);
        camera.rotation = glm::vec3( sin_t, 0, sin_t );
        camera.update();

        fx.process( [&]{
            // clear first
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // bind and draw the vao
            for( auto &v : vao ) {
                v.draw(camera);
            }
        });

        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
            std::cerr << "GLError:" << error << std::endl;
            break;
        }

        // finally swap buffers
        swapgl();
    }

    quitgl();
    return 0;
}

