#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#endif
#include <eve/eve.hpp>


Shader shader;
Buffer<vec2> quad4;
VAO layout;

void setup() {
    shader.vertexShader(glsl(410,
        in vec2 vertex;
        out vec2 coord;
        void main() {
            coord = vertex * 0.5 + 0.5;
            gl_Position = vec4(vertex, 0.0, 1.0);
        }
    )).fragmentShader(glsl(410,
        in vec2 coord;
        out vec4 color;
        void main() {
            color = vec4(coord, 0.0, 1.0);
        }
    )).link();

    quad4 << vec2(-1, -1) << vec2(1, -1) << vec2(-1, 1) << vec2(1, 1);
    quad4.upload();
    layout.create(shader, quad4).attribute<float>("vertex", 2).check();
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    layout.draw(GL_TRIANGLE_STRIP);
    shader.unuse();
}

void shutdown() {
    std::cout << "shutdown" << std::endl;
}


namespace {
    bool executing = true;
    void closing() {
        executing = false;
    }
}

int main( int argc, const char **argv ) {
    glutInit(&argc, (char **)argv);
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitContextVersion (3, 2);
    glutInitContextFlags (GLUT_FORWARD_COMPATIBLE);
#endif
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Example");
    glewExperimental = true;
    glewInit();
    glutReshapeWindow(800, 600);
    glutDisplayFunc(draw);
    glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutCloseFunc(closing);

    setup();
    while( executing ) { glutMainLoopEvent(); glutSwapBuffers(); }
    shutdown();

    return 0;
}
