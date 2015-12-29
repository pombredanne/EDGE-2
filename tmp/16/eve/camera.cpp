#include "camera.hpp"
#include <GL/glew.h>
//#include <GL/freeglut.h>

namespace eve {

void camera::apply() const
{
    // Refresh viewport
    glViewport( viewport.x, viewport.y, viewport.z, viewport.w );

    // Apply matrices to fixed pipeline
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf( projection.data() );

    glMatrixMode( GL_MODELVIEW );
    glLoadMatrixf( view.data() );

    // CCW
    glFrontFace(GL_CCW);
}

void camera::apply90() const
{
    apply();

    // to xzy
    glScalef( 1.f, 1.f, -1.f );
    glRotatef(  90.f, 0.f, 0.f, 1.f );
    glRotatef( -90.f, 1.f, 0.f, 0.f );

    // CW
    glFrontFace(GL_CW);
}

geo::vec3 camera::unproject( float screen_x, float screen_y ) const //at_screen()
{
    //apply();

    float depth;
    glReadPixels( screen_x, viewport.w - screen_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth );

    geo::vec3 wincoord = geo::vec3( screen_x, screen_y, depth );
    geo::vec3 ret = glm::unProject( wincoord, view, projection, viewport );
#if 0
    if( depth == 1.f )
        std::cout << "depth: far" << std::endl;
    else
    if( depth > 0 )
        std::cout << "depth: " << depth << std::endl;
    else
        std::cout << "depth: near" << std::endl;
#endif
    return geo::vec3( ret.x, ret.y, -ret.z );
}

geo::vec3 camera::unproject2( float screen_x, float screen_y, float w, float h ) const //at_screen()
{
    // [ref] http://en.wikibooks.org/wiki/OpenGL_Programming/Object_selection

    float x = screen_x;
    float y = screen_y;
    float window_width = w; //glutGet(GLUT_WINDOW_WIDTH);
    float window_height = h; //glutGet(GLUT_WINDOW_HEIGHT);

//            GLbyte color[4];
    GLfloat depth;
//            GLuint index;

    //  glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    //  glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    glm::vec4 viewport = glm::vec4(0, 0, window_width, window_height);
    glm::vec3 wincoord = glm::vec3(x, window_height - y - 1, depth);
    glm::vec3 objcoord = glm::unProject(wincoord, view, projection, viewport);

    return geo::vec3( objcoord );

#if 0
    window_width = glutGet(GLUT_WINDOW_WIDTH);
    window_height = glutGet(GLUT_WINDOW_HEIGHT);

    GLbyte color[4];
    GLfloat depth;
    GLuint index;

    glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
        x, y, color[0], color[1], color[2], color[3], depth, index);
#endif
}

}
