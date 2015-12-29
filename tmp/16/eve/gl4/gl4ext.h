// extension to evanw/gl4
// r-lyeh, zlib licensed

/*
 * buildFrustumPlanes()
 * Copyright 2012 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

/* @todo
shadertoy

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
uniform vec4      iDate;                 // (year, month, day, time in seconds)
uniform float     iSampleRate;           // sound sample rate (i.e., 44100)
*/

#pragma once

#include <GL/glew.h>
//#include <GL/gl3w.h>

#include <eve/eve.hpp>
//#include <GLXW/glxw.h>
//#include <GLFW/glfw3.h>
#include <math.h>
#include <functional>
#include <omp.h> // to remove

namespace {
    // goo colour scheme, all components make 255+192 (~) to keep
    // tone balance. red, purple and yellow tweak a little bit to
    // fit better with gray colours.
    using color3f = vec3;

    const color3f  black( 0,0,0 );
    const color3f   gray( 149/255.f,149/255.f,149/255.f ); // gray
    const color3f   blue(   0/255.f,192/255.f,255/255.f ); // blue
    const color3f   cyan(  48/255.f,255/255.f,144/255.f ); // cyan/turquoise
    const color3f   pink( 255/255.f, 48/255.f,144/255.f ); // pink
    const color3f orange( 255/255.f,144/255.f, 48/255.f ); // orange
    const color3f  green( 144/255.f,255/255.f, 48/255.f ); // green/lime
    const color3f purple( 178/255.f,128/255.f,255/255.f ); // purple
    const color3f yellow( 255/255.f,224/255.f,  0/255.f ); // yellow
    const color3f    red( 255/255.f, 48/255.f, 48/255.f ); // red
    const color3f  white( 1,1,1 );

    const color3f selection[] = { blue, cyan, pink, orange, green, purple, yellow, red };
    enum { SELECTION_COLORS = 8 };
}



struct solid_cube {
    struct VertexData { vec3 position, color; };
    VBO<VertexData> vbo;
    IBO<GLuint> ibo;

    solid_cube( const color3f &c = white ) {
        setup( c );
    }

    solid_cube( const std::vector<color3f> &colors ) {
        setup( colors );
    }

    void setup( const color3f &c = white ) {
        setup( {c,c,c,c,c,c} );
    }

    void setup( const std::vector<color3f> &colors ) {
        vbo.clear();
        vbo
            << VertexData {{  1.0f, 1.0f, 1.0f }, colors[0] } // face 0: vertex 0
            << VertexData {{ -1.0f, 1.0f, 1.0f }, colors[0] } // face 0: vertex 1
            << VertexData {{  1.0f,-1.0f, 1.0f }, colors[0] } // face 0: vertex 2
            << VertexData {{ -1.0f,-1.0f, 1.0f }, colors[0] } // face 0: vertex 3

            << VertexData {{  1.0f, 1.0f, 1.0f }, colors[1] } // face 1: vertex 0
            << VertexData {{  1.0f,-1.0f, 1.0f }, colors[1] } // face 1: vertex 1
            << VertexData {{  1.0f, 1.0f,-1.0f }, colors[1] } // face 1: vertex 2
            << VertexData {{  1.0f,-1.0f,-1.0f }, colors[1] } // face 1: vertex 3

            << VertexData {{  1.0f, 1.0f, 1.0f }, colors[2] } // face 2: vertex 0
            << VertexData {{  1.0f, 1.0f,-1.0f }, colors[2] } // face 2: vertex 1
            << VertexData {{ -1.0f, 1.0f, 1.0f }, colors[2] } // face 2: vertex 2
            << VertexData {{ -1.0f, 1.0f,-1.0f }, colors[2] } // face 2: vertex 3

            << VertexData {{  1.0f, 1.0f,-1.0f }, colors[3] } // face 3: vertex 0
            << VertexData {{  1.0f,-1.0f,-1.0f }, colors[3] } // face 3: vertex 1
            << VertexData {{ -1.0f, 1.0f,-1.0f }, colors[3] } // face 3: vertex 2
            << VertexData {{ -1.0f,-1.0f,-1.0f }, colors[3] } // face 3: vertex 3

            << VertexData {{ -1.0f, 1.0f, 1.0f }, colors[4] } // face 4: vertex 0
            << VertexData {{ -1.0f, 1.0f,-1.0f }, colors[4] } // face 4: vertex 1
            << VertexData {{ -1.0f,-1.0f, 1.0f }, colors[4] } // face 4: vertex 2
            << VertexData {{ -1.0f,-1.0f,-1.0f }, colors[4] } // face 4: vertex 3

            << VertexData {{  1.0f,-1.0f, 1.0f }, colors[5] } // face 5: vertex 0
            << VertexData {{ -1.0f,-1.0f, 1.0f }, colors[5] } // face 5: vertex 1
            << VertexData {{  1.0f,-1.0f,-1.0f }, colors[5] } // face 5: vertex 2
            << VertexData {{ -1.0f,-1.0f,-1.0f }, colors[5] } // face 5: vertex 3
        ;
        vbo.upload();

        ibo.clear();
        ibo << 0<<1<<2       // face 0: first triangle
            << 2<<1<<3       // face 0: second triangle
            << 4<<5<<6       // face 1: first triangle
            << 6<<5<<7       // face 1: second triangle
            << 8<<9<<10      // face 2: first triangle
            << 10<<9<<11     // face 2: second triangle
            << 12<<13<<14    // face 3: first triangle
            << 14<<13<<15    // face 3: second triangle
            << 16<<17<<18    // face 4: first triangle
            << 18<<17<<19    // face 4: second triangle
            << 20<<21<<22    // face 5: first triangle
            << 22<<21<<23;   // face 5: second triangle
        ibo.upload(GL_ELEMENT_ARRAY_BUFFER);
    }
};

struct textured_quad {
    struct Vertex { float x, y, z, u, v; };

    // handles
    IBO<GLuint> ibo;
    VBO<Vertex> vbo;

    textured_quad() {
        ibo << 0 << 1 << 2  // first triangle
            << 2 << 1 << 3; // second triangle
        ibo.upload(GL_ELEMENT_ARRAY_BUFFER);

                        //  X     Y     Z     U     V
        vbo << Vertex {  1.0f, 1.0f, 0.0f, 1.0f, 1.0f } // Vertex 0
            << Vertex { -1.0f, 1.0f, 0.0f, 0.0f, 1.0f } // Vertex 1
            << Vertex {  1.0f,-1.0f, 0.0f, 1.0f, 0.0f } // Vertex 2
            << Vertex { -1.0f,-1.0f, 0.0f, 0.0f, 0.0f };// Vertex 3
        vbo.upload(GL_ARRAY_BUFFER);
    }
};

struct transform {
    glm::mat4 ModelView; //aka, 'world' transformation
    glm::vec3 position = glm::vec3( 0, 0, 0 );
    glm::vec3 rotation = glm::vec3( 0, 0, 0 );
    glm::vec3 scale = glm::vec3( 1, 1, 1 );

    void update() {
        // translate the world/view position
        glm::mat4 View = glm::translate(glm::mat4(1.0f), position);

        // make the camera rotate around the origin
        View = glm::rotate(View, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        View = glm::rotate(View, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        View = glm::rotate(View, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        // scale the model matrix
        glm::mat4 Model = glm::scale( glm::mat4(1.0f), scale );
        ModelView = View * Model;
    }

    const float *get_modelview() const {
        return glm::value_ptr(ModelView);
    }
};

struct frustum {
    typedef float plane[4];
    plane nearby, remote, left, right, top, bottom;
};

struct camera : public transform, public frustum {
    glm::mat4 Projection;
    glm::mat4 ModelViewProjection;
    float fov = 60.f; // [close 50..90 sick] 55/60 good for me, 65 hard reset
    glm::vec2 aspect = glm::vec2( 4.0f , 3.0f );
    glm::vec2 distance = glm::vec2( 0.1f, 100.f );

    void update() {
        // calculate Projection matrix
        Projection = glm::perspective( glm::radians(fov), aspect.x / aspect.y, distance.x, distance.y );

        // calculate ModelView matrix
        transform::update();

        // calculate ModelViewProjection matrix
        ModelViewProjection = Projection*ModelView;
    }

    const float *get_modelviewproj() const {
        return glm::value_ptr(ModelViewProjection);
    }

    /*
     * Copyright 2012 Branimir Karadzic. All rights reserved.
     * License: http://www.opensource.org/licenses/BSD-2-Clause
     */
    static void buildFrustumPlanes(float _result[24], const float _viewProj[16])
    {
        const float xw = _viewProj[ 3];
        const float yw = _viewProj[ 7];
        const float zw = _viewProj[11];
        const float ww = _viewProj[15];

        const float xz = _viewProj[ 2];
        const float yz = _viewProj[ 6];
        const float zz = _viewProj[10];
        const float wz = _viewProj[14];

        // near
        _result[ 0] = xw - xz;
        _result[ 1] = yw - yz;
        _result[ 2] = zw - zz;
        _result[ 3] = ww - wz;

        // far
        _result[ 4] = xw + xz;
        _result[ 5] = yw + yz;
        _result[ 6] = zw + zz;
        _result[ 7] = ww + wz;

        const float xx = _viewProj[ 0];
        const float yx = _viewProj[ 4];
        const float zx = _viewProj[ 8];
        const float wx = _viewProj[12];

        // left
        _result[ 8] = xw - xx;
        _result[ 9] = yw - yx;
        _result[10] = zw - zx;
        _result[11] = ww - wx;

        // right
        _result[12] = xw + xx;
        _result[13] = yw + yx;
        _result[14] = zw + zx;
        _result[15] = ww + wx;

        const float xy = _viewProj[ 1];
        const float yy = _viewProj[ 5];
        const float zy = _viewProj[ 9];
        const float wy = _viewProj[13];

        // top
        _result[16] = xw + xy;
        _result[17] = yw + yy;
        _result[18] = zw + zy;
        _result[19] = ww + wy;

        // bottom
        _result[20] = xw - xy;
        _result[21] = yw - yy;
        _result[22] = zw - zy;
        _result[23] = ww - wy;

        float* plane = _result;
        for (uint32_t ii = 0; ii < 6; ++ii)
        {
            float invLen = 1.0f / sqrtf(plane[0]*plane[0] + plane[1]*plane[1] + plane[2]*plane[2]);
            plane[0] *= invLen;
            plane[1] *= invLen;
            plane[2] *= invLen;
            plane[3] *= invLen;
            plane += 4;
        }
    }
};

struct Pass {
    std::string name;
    VAO vao;
    Shader shader;
    bool enabled;
    textured_quad tq;
    GLint texture_location = -1, depth_location = -1, time_location = -1, width_location = -1, height_location = -1;

    Pass( const std::string &name, const std::string &fragment, const std::string &vertex =
        glsl(
            layout(location = 0) IN vec4 vposition; // or gl_FragData[0], do not write to gl_FragColor
            layout(location = 1) IN vec2 vtexcoord; // or gl_FragData[1], do not write to gl_FragColor
            void main() {
               GL_TEXCOORD = vtexcoord;
               gl_Position = vposition;
            }
        )
     ) : name(name), enabled(true) {
        std::string err = shader.vertexShader( vertex ).fragmentShader( fragment ).link();
        if( !err.empty() ) {
            std::cerr << err << std::endl;
        }

        if( texture_location == -1 ) texture_location = shader.uniform("tex");
        if( texture_location == -1 ) texture_location = shader.uniform("tex0");
        if( texture_location == -1 ) texture_location = shader.uniform("tColor");
        if( texture_location == -1 ) texture_location = shader.uniform("tDiffuse");
        if( texture_location == -1 ) texture_location = shader.uniform("intexture");
        if( texture_location == -1 ) texture_location = shader.uniform("bgl_RenderedTexture");
        if( texture_location == -1 ) texture_location = shader.uniform("iChannel0");

        if( depth_location == -1 ) depth_location = shader.uniform("tDepth");
        if( texture_location == -1 ) depth_location = shader.uniform("bgl_DepthTexture");

        if( width_location == -1 ) width_location = shader.uniform("width");
        if( height_location == -1 ) height_location = shader.uniform("height");
        if( time_location == -1 ) time_location = shader.uniform("time");

        // set quad
        vao.create(shader, tq.vbo, tq.ibo).attribute<float>("vposition", 3).attribute<float>("vtexcoord", 2).check();
    }
};


template<typename T>
struct mesh {
    // shader source code
    Shader shader;
    //
    VAO vao;
    //
    T data;
    //
    transform tf;

    mesh( const glm::vec3 &pos = glm::vec3(0,0,0) ) {
        // default shader
        std::string errors =
        shader.vertexShader( glsl(
            uniform mat4 ViewProjection; /* the projection matrix uniform gl_ModelViewMatrix */
            IN vec4 vposition;
            IN vec4 vcolor;
            OUT vec4 fcolor;
            void main() {
               fcolor = vcolor;
               gl_Position = ViewProjection*vposition;
            }
        ) ).fragmentShader( glsl(
            IN vec4 fcolor;
            void main() {
               GL_FRAGCOLOR = fcolor;
               /* the following line is required for fxaa (will not work with blending!) */
               GL_FRAGCOLOR.a = dot(fcolor.rgb, vec3(0.299, 0.587, 0.114));
            }
        ) ).link();

        if( shader.uniform("vposition") != -1 && shader.uniform("vcolor") != -1 ) {
            std::cout << errors << std::endl;
        } else {
            vao.create(shader, data.vbo, data.ibo).attribute<float>("vposition", 3).attribute<float>("vcolor", 3).check();
        }

        // update ModelView
        tf.position = pos;
        tf.update();
    }

    void draw( const glm::mat4 &ModelViewProjection ) const {
        // use the shader
        shader.use();

        // set mvp in the uniform
        int mvp = glGetUniformLocation(shader.id, "ViewProjection");
        glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

        // data for a cube
        vao.bind();
        vao.draw(GL_TRIANGLES);
    }

    void draw( const camera &cam ) const {
        glm::mat4 mvp = cam.ModelViewProjection*tf.ModelView;
        draw( mvp );
    }
};

struct postfx {
    // program and shader handles
    std::vector<Pass *> passes;
    // renderbuffer handle
    FBO fbo[2];
    // create texture, set texture parameters and content
    Texture diffuse[2], depth[2];
    // enable flag
    bool enabled;

    postfx( int width, int height ) {
        // create texture, set texture parameters and content
        diffuse[0].create(width, height, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE);
        depth[0].create(width, height, 1, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT, GL_NEAREST, GL_CLAMP_TO_EDGE);
        fbo[0].attachColor(diffuse[0]).attachColor(depth[0]).check();

        // create texture, set texture parameters and content
        diffuse[1].create(width, height, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE);
        depth[1].create(width, height, 1, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT, GL_NEAREST, GL_CLAMP_TO_EDGE);
        fbo[1].attachColor(diffuse[1]).attachColor(depth[1]).check();

        enabled = true;
    }

    std::vector<std::string> active;
    std::string list(char sep = ',') {
        std::string out;
        for( auto &name : active ) {
            out += name + sep;
        }
        return out.size() ? (out.pop_back(), out) : out;
    }

    template<typename T>
    void process( const T &fn ) {
        int numpasses = 0;
        active.clear();
        for( auto &pass : passes ) {
            numpasses += pass->enabled;
            if( pass->enabled ) active.push_back( pass->name );
        }

        if( !(enabled && numpasses) ) {
            fbo[0].unbind();
            fn();
            return;
        }

        if( GetAsyncKeyState(VK_F5) & 0x8000 ) {
            for( auto &pass : passes ) {
                pass->shader.reload();
            }
        }

        fbo[0].bind();
        fn();
        fbo[0].unbind();

        // we are not 3d rendering so no depth test
        glDisable(GL_DEPTH_TEST);

        int frame = 0, w = diffuse[0].width, h = diffuse[0].height, t = float(omp_get_wtime());

        for( auto &pass : passes ) {
            if( pass->enabled ) {
                // use the shader program
                pass->shader.use();

                // bind texture to texture unit 0
                diffuse[frame].bind(0);
                glUniform1i(pass->texture_location, 0);

                // bind depth to texture unit 1
                depth[frame].bind(1);
                glUniform1i(pass->depth_location, 1);

                glUniform1f(pass->time_location, t);
                /* if GLSL >= 300 */
                glUniform1f(pass->width_location, w);
                /* if GLSL >= 300 */
                glUniform1f(pass->height_location, h);

                frame ^= 1;

                // bind the vao
                int bound = --numpasses;
                if( bound ) fbo[frame].bind();
                    pass->vao.bind();
                    pass->vao.draw(GL_TRIANGLES);
                if( bound ) fbo[frame].unbind();
                else pass->shader.unuse();
            }
        }
    }
};
