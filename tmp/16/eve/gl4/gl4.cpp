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

#include "gl4.h"

#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {
    // taken from https://github.com/r-lyeh/wire
    std::deque< std::string > tokenize( const std::string &self, const std::string &delimiters ) {
        std::string map( 256, '\0' );
        for( const unsigned char &ch : delimiters )
            map[ ch ] = '\1';
        std::deque< std::string > tokens(1);
        for( const unsigned char &ch : self ) {
            /**/ if( !map.at(ch)          ) tokens.back().push_back( char(ch) );
            else if( tokens.back().size() ) tokens.push_back( std::string() );
        }
        while( tokens.size() && !tokens.back().size() ) tokens.pop_back();
        return tokens;
    }
    std::deque< std::string > split( const std::string &self, const std::string &delimiters ) {
        std::string str;
        std::deque< std::string > tokens;
        for( auto &ch : self ) {
            if( delimiters.find_first_of( ch ) != std::string::npos ) {
                if( str.size() ) tokens.push_back( str ), str = "";
                tokens.push_back( std::string() + ch );
            } else str += ch;
        }
        return str.empty() ? tokens : ( tokens.push_back( str ), tokens );
    }
    std::string replace( std::string s, const std::string &target, const std::string &replacement ) {
        size_t found = 0;
        while( ( found = s.find( target, found ) ) != std::string::npos ) {
            s.replace( found, target.length(), replacement );
            found += replacement.length();
        }
        return s;
    }
}

mat4 &mat4::transpose() {
    std::swap(m01, m10); std::swap(m02, m20); std::swap(m03, m30);
    std::swap(m12, m21); std::swap(m13, m31); std::swap(m23, m32);
    return *this;
}

mat4 &mat4::rotateX(float degrees) {
    float radians = degrees * (M_PI / 180);
    float s = sinf(radians), c = cosf(radians);
    float t01 = m01, t02 = m02;
    float t11 = m11, t12 = m12;
    float t21 = m21, t22 = m22;
    float t31 = m31, t32 = m32;
    m01 = c * t01 - s * t02;
    m02 = c * t02 + s * t01;
    m11 = c * t11 - s * t12;
    m12 = c * t12 + s * t11;
    m21 = c * t21 - s * t22;
    m22 = c * t22 + s * t21;
    m31 = c * t31 - s * t32;
    m32 = c * t32 + s * t31;
    return *this;
}

mat4 &mat4::rotateY(float degrees) {
    float radians = degrees * (M_PI / 180);
    float s = sinf(radians), c = cosf(radians);
    float t02 = m02, t00 = m00;
    float t12 = m12, t10 = m10;
    float t22 = m22, t20 = m20;
    float t32 = m32, t30 = m30;
    m02 = c * t02 - s * t00;
    m00 = c * t00 + s * t02;
    m12 = c * t12 - s * t10;
    m10 = c * t10 + s * t12;
    m22 = c * t22 - s * t20;
    m20 = c * t20 + s * t22;
    m32 = c * t32 - s * t30;
    m30 = c * t30 + s * t32;
    return *this;
}

mat4 &mat4::rotateZ(float degrees) {
    float radians = degrees * (M_PI / 180);
    float s = sinf(radians), c = cosf(radians);
    float t00 = m00, t01 = m01;
    float t10 = m10, t11 = m11;
    float t20 = m20, t21 = m21;
    float t30 = m30, t31 = m31;
    m00 = c * t00 - s * t01;
    m01 = c * t01 + s * t00;
    m10 = c * t10 - s * t11;
    m11 = c * t11 + s * t10;
    m20 = c * t20 - s * t21;
    m21 = c * t21 + s * t20;
    m30 = c * t30 - s * t31;
    m31 = c * t31 + s * t30;
    return *this;
}

mat4 &mat4::rotate(float degrees, float x, float y, float z) {
    float radians = degrees * (M_PI / 180);
    float d = sqrtf(x*x + y*y + z*z);
    float s = sinf(radians), c = cosf(radians), t = 1 - c;
    x /= d; y /= d; z /= d;
    return *this *= mat4(
        x*x*t + c, x*y*t - z*s, x*z*t + y*s, 0,
        y*x*t + z*s, y*y*t + c, y*z*t - x*s, 0,
        z*x*t - y*s, z*y*t + x*s, z*z*t + c, 0,
        0, 0, 0, 1);
}

mat4 &mat4::scale(float x, float y, float z) {
    m00 *= x; m01 *= y; m02 *= z;
    m10 *= x; m11 *= y; m12 *= z;
    m20 *= x; m21 *= y; m22 *= z;
    m30 *= x; m31 *= y; m32 *= z;
    return *this;
}

mat4 &mat4::translate(float x, float y, float z) {
    m03 += m00 * x + m01 * y + m02 * z;
    m13 += m10 * x + m11 * y + m12 * z;
    m23 += m20 * x + m21 * y + m22 * z;
    m33 += m30 * x + m31 * y + m32 * z;
    return *this;
}

mat4 &mat4::ortho(float l, float r, float b, float t, float n, float f) {
    return *this *= mat4(
        2 / (r - l), 0, 0, (r + l) / (l - r),
        0, 2 / (t - b), 0, (t + b) / (b - t),
        0, 0, 2 / (n - f), (f + n) / (n - f),
        0, 0, 0, 1);
}

mat4 &mat4::frustum(float l, float r, float b, float t, float n, float f) {
    return *this *= mat4(
        2 * n / (r - l), 0, (r + l) / (r - l), 0,
        0, 2 * n / (t - b), (t + b) / (t - b), 0,
        0, 0, (f + n) / (n - f), 2 * f * n / (n - f),
        0, 0, -1, 0);
}

mat4 &mat4::perspective(float fov, float aspect, float neard, float fard) {
    float y = tanf(fov * M_PI / 360) * neard, x = y * aspect;
    return frustum(-x, x, -y, y, neard, fard);
}

mat4 &mat4::invert() {
    float t00 = m00, t01 = m01, t02 = m02, t03 = m03;
    *this = mat4(
        m11*m22*m33 - m11*m32*m23 - m12*m21*m33 + m12*m31*m23 + m13*m21*m32 - m13*m31*m22,
        -m01*m22*m33 + m01*m32*m23 + m02*m21*m33 - m02*m31*m23 - m03*m21*m32 + m03*m31*m22,
        m01*m12*m33 - m01*m32*m13 - m02*m11*m33 + m02*m31*m13 + m03*m11*m32 - m03*m31*m12,
        -m01*m12*m23 + m01*m22*m13 + m02*m11*m23 - m02*m21*m13 - m03*m11*m22 + m03*m21*m12,

        -m10*m22*m33 + m10*m32*m23 + m12*m20*m33 - m12*m30*m23 - m13*m20*m32 + m13*m30*m22,
        m00*m22*m33 - m00*m32*m23 - m02*m20*m33 + m02*m30*m23 + m03*m20*m32 - m03*m30*m22,
        -m00*m12*m33 + m00*m32*m13 + m02*m10*m33 - m02*m30*m13 - m03*m10*m32 + m03*m30*m12,
        m00*m12*m23 - m00*m22*m13 - m02*m10*m23 + m02*m20*m13 + m03*m10*m22 - m03*m20*m12,

        m10*m21*m33 - m10*m31*m23 - m11*m20*m33 + m11*m30*m23 + m13*m20*m31 - m13*m30*m21,
        -m00*m21*m33 + m00*m31*m23 + m01*m20*m33 - m01*m30*m23 - m03*m20*m31 + m03*m30*m21,
        m00*m11*m33 - m00*m31*m13 - m01*m10*m33 + m01*m30*m13 + m03*m10*m31 - m03*m30*m11,
        -m00*m11*m23 + m00*m21*m13 + m01*m10*m23 - m01*m20*m13 - m03*m10*m21 + m03*m20*m11,

        -m10*m21*m32 + m10*m31*m22 + m11*m20*m32 - m11*m30*m22 - m12*m20*m31 + m12*m30*m21,
        m00*m21*m32 - m00*m31*m22 - m01*m20*m32 + m01*m30*m22 + m02*m20*m31 - m02*m30*m21,
        -m00*m11*m32 + m00*m31*m12 + m01*m10*m32 - m01*m30*m12 - m02*m10*m31 + m02*m30*m11,
        m00*m11*m22 - m00*m21*m12 - m01*m10*m22 + m01*m20*m12 + m02*m10*m21 - m02*m20*m11
    );
    float det = m00 * t00 + m10 * t01 + m20 * t02 + m30 * t03;
    for (int i = 0; i < 16; i++) m[i] /= det;
    return *this;
}

mat4 &mat4::operator *= (const mat4 &t) {
    *this = mat4(
        m00*t.m00 + m01*t.m10 + m02*t.m20 + m03*t.m30,
        m00*t.m01 + m01*t.m11 + m02*t.m21 + m03*t.m31,
        m00*t.m02 + m01*t.m12 + m02*t.m22 + m03*t.m32,
        m00*t.m03 + m01*t.m13 + m02*t.m23 + m03*t.m33,

        m10*t.m00 + m11*t.m10 + m12*t.m20 + m13*t.m30,
        m10*t.m01 + m11*t.m11 + m12*t.m21 + m13*t.m31,
        m10*t.m02 + m11*t.m12 + m12*t.m22 + m13*t.m32,
        m10*t.m03 + m11*t.m13 + m12*t.m23 + m13*t.m33,

        m20*t.m00 + m21*t.m10 + m22*t.m20 + m23*t.m30,
        m20*t.m01 + m21*t.m11 + m22*t.m21 + m23*t.m31,
        m20*t.m02 + m21*t.m12 + m22*t.m22 + m23*t.m32,
        m20*t.m03 + m21*t.m13 + m22*t.m23 + m23*t.m33,

        m30*t.m00 + m31*t.m10 + m32*t.m20 + m33*t.m30,
        m30*t.m01 + m31*t.m11 + m32*t.m21 + m33*t.m31,
        m30*t.m02 + m31*t.m12 + m32*t.m22 + m33*t.m32,
        m30*t.m03 + m31*t.m13 + m32*t.m23 + m33*t.m33
    );
    return *this;
}

vec4 mat4::operator * (const vec4 &v) {
    return vec4(
        m00*v.x + m01*v.y + m02*v.z + m03*v.w,
        m10*v.x + m11*v.y + m12*v.z + m13*v.w,
        m20*v.x + m21*v.y + m22*v.z + m23*v.w,
        m30*v.x + m31*v.y + m32*v.z + m33*v.w
    );
}

vec4 operator * (const vec4 &v, const mat4 &t) {
    return vec4(
        t.m00*v.x + t.m10*v.y + t.m20*v.z + t.m30*v.w,
        t.m01*v.x + t.m11*v.y + t.m21*v.z + t.m31*v.w,
        t.m02*v.x + t.m12*v.y + t.m22*v.z + t.m32*v.w,
        t.m03*v.x + t.m13*v.y + t.m23*v.z + t.m33*v.w
    );
}

Texture &Texture::create(int w, int h, int d, int internalFormat, int format, int type, int filter, int wrap, void *data) {
    target = (d == 1) ? GL_TEXTURE_2D : GL_TEXTURE_3D;
    width = w;
    height = h;
    depth = d;
    this->internalFormat = internalFormat;
    this->format = format;
    this->type = type;
    if (!id) glGenTextures(1, &id);
    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);
    if (target == GL_TEXTURE_2D) {
        glTexImage2D(target, 0, internalFormat, w, h, 0, format, type, data);
    } else {
        glTexParameteri(target, GL_TEXTURE_WRAP_R, wrap);
        glTexImage3D(target, 0, internalFormat, w, h, d, 0, format, type, data);
    }
    unbind();
    return *this;
}

void Texture::swapWith(Texture &other) {
    std::swap(id, other.id);
    std::swap(target, other.target);
    std::swap(width, other.width);
    std::swap(height, other.height);
    std::swap(depth, other.depth);
    std::swap(internalFormat, other.internalFormat);
    std::swap(format, other.format);
    std::swap(type, other.type);
}

void FBO::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    if (resizeViewport) {
        glGetIntegerv(GL_VIEWPORT, oldViewport);
        glViewport(newViewport[0], newViewport[1], newViewport[2], newViewport[3]);
    }
}

void FBO::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (resizeViewport) {
        glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
    }
}

FBO &FBO::attachColor(const Texture &texture, unsigned int attachment, unsigned int layer) {
    newViewport[2] = texture.width;
    newViewport[3] = texture.height;
    if (!id) glGenFramebuffers(1, &id);
    bind();

    unsigned int attach;
    switch( texture.format ) {
        default: attach = GL_COLOR_ATTACHMENT0 + attachment; break;
        case GL_DEPTH_COMPONENT: attach = GL_DEPTH_ATTACHMENT; break;
        case GL_DEPTH_STENCIL: attach = GL_DEPTH_STENCIL_ATTACHMENT; break;
    }

    // Bind a 2D texture (using a 2D layer of a 3D texture)
    if (texture.target == GL_TEXTURE_2D) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attach, texture.target, texture.id, 0);
    } else {
        glFramebufferTexture3D(GL_FRAMEBUFFER, attach, texture.target, texture.id, 0, layer);
    }

    // Need to call glDrawBuffers() for OpenGL to draw to multiple attachments
    if( texture.format != GL_DEPTH_COMPONENT && texture.format != GL_DEPTH_STENCIL ) {
        if (attachment >= drawBuffers.size()) drawBuffers.resize(attachment + 1, GL_NONE);
        drawBuffers[attachment] = attach;
        glDrawBuffers(drawBuffers.size(), drawBuffers.data());
    }

    unbind();
    return *this;
}

FBO &FBO::detachColor(unsigned int attachment) {
    bind();

    // Update the draw buffers
    if (attachment < drawBuffers.size()) {
        drawBuffers[attachment] = GL_NONE;
        glDrawBuffers(drawBuffers.size(), drawBuffers.data());
    }

    unbind();
    return *this;
}

FBO &FBO::check() {
    bind();
    if (autoDepth) {
        if (renderbufferWidth != newViewport[2] || renderbufferHeight != newViewport[3]) {
            renderbufferWidth = newViewport[2];
            renderbufferHeight = newViewport[3];
            /*
            if (!renderbuffer) glGenRenderbuffers(1, &renderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, renderbufferWidth, renderbufferHeight);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            */
        }
        //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    }
    switch (glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
        case GL_FRAMEBUFFER_COMPLETE: break;
        case GL_FRAMEBUFFER_UNDEFINED: printf("%s\n","GL_FRAMEBUFFER_UNDEFINED"); exit(0);
        case GL_FRAMEBUFFER_UNSUPPORTED: printf("%s\n","GL_FRAMEBUFFER_UNSUPPORTED"); exit(0);
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: printf("%s\n","GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); exit(0);
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: printf("%s\n","GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"); exit(0);
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: printf("%s\n","GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"); exit(0);
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: printf("%s\n","GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"); exit(0);
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT: printf("%s\n","GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT"); exit(0);
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: printf("%s\n","GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"); exit(0);
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: printf("%s\n","GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT"); exit(0);
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: printf("%s\n","GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); exit(0);
        default: printf("%s\n","Unknown glCheckFramebufferStatus error"); exit(0);
    }
    unbind();
    return *this;
}

Shader::~Shader() {
    for (size_t i = 0; i < stages.size(); i++) {
        glDetachShader(id, stages[i]);
    }
    for (size_t i = 0; i < stages.size(); i++) {
        glDeleteShader(stages[i]);
    }
    glDeleteProgram(id);
}

static
std::string get_glsl_headers(int type, int version) {
    std::stringstream ss;

    ss << "#ifdef GL_ES " << std::endl;
    ss << "# define LOWP lowp" << std::endl;
    ss << "# define MED mediump" << std::endl;
    ss << "# define HIGH highp" << std::endl;
    ss << "# ifdef GL_FRAGMENT_PRECISION_HIGH" << std::endl;
    ss << "   precision highp float;" << std::endl;
    ss << "# else" << std::endl;
    ss << "   precision mediump float;" << std::endl;
    ss << "# endif" << std::endl;
    ss << "#else" << std::endl;
    ss << "# define LOWP" << std::endl;
    ss << "# define MED" << std::endl;
    ss << "# define HIGH" << std::endl;
    ss << "#endif" << std::endl;

    bool arb = 0;
    if( arb ) {
        ss << "#define SAMPLER2D sampler2DRect" << std::endl;
        ss << "#define TEXTURE2D texture2DRect" << std::endl;
        ss << "#extension GL_ARB_texture_rectangle : enable" << std::endl;
    } else {
        ss << "#define SAMPLER2D sampler2D" << std::endl;
        ss << "#define TEXTURE2D texture2D" << std::endl;
    }

    bool legacy = (version == 110 || version == 120);

    if( legacy ) {
        ss << "#define IN varying" << std::endl; //attribute //uniform" << std::endl;
        ss << "#define ATTRIBUTE varying" << std::endl;
        ss << "#define OUT varying" << std::endl;
        ss << "#define VARYING varying" << std::endl;
    } else {
        ss << "#define IN in" << std::endl;
        ss << "#define ATTRIBUTE in" << std::endl;
        ss << "#define OUT out" << std::endl;
        ss << "#define VARYING out" << std::endl;
    }

    // #version 410, or...
    //ss << "#extension GL_ARB_separate_shader_objects : enable" << std::endl; // layout(x)

    if( GL_VERTEX_SHADER == type )
    {
        if( legacy ) {
            ss << "#define GL_TEXCOORD gl_TexCoord[0]" << std::endl;
        } else {
            ss << "OUT vec2 GL_TEXCOORD;" << std::endl;
        }
    }

    if( GL_FRAGMENT_SHADER == type )
    {
        if( legacy ) {
            ss << "#define GL_TEXCOORD gl_TexCoord[0]" << std::endl;
            ss << "#define GL_FRAGCOLOR gl_FragColor" << std::endl;
        } else {
            //ss << "layout(location = 0) "
            ss << "OUT vec4 GL_FRAGCOLOR;" << std::endl;
            //ss << "layout(location = 1) "
            ss << "IN vec2 GL_TEXCOORD;" << std::endl;
        }
    }

    ss << "uniform float width = 1.f;" << std::endl;
    ss << "uniform float height = 1.f;" << std::endl;
    ss << "uniform float time = 0.f;" << std::endl;

    return ss.str();
}

#include <sstream>

static
std::string error(const char *type, const char *error, const char *source = NULL) {
    std::stringstream ss;
    if (source) {
        ss << "----- source code -----\n";
        unsigned line = 0;
        while( source && *source ) {
            ss << std::setw(3) << std::setfill('0') << (++line) << ": " ;
            while( source && *source && *source != '\n' ) ss << *source++;
            if( source && *source ) ss << *source++;
        }
    }
    ss << "----- " << type << " -----\n";
    ss << error << "\n";
    return ss.str();
}

Shader &Shader::shader(int type, const std::string &content) {

    // if filename, read and continue
    std::string data;
    std::ifstream ifs( content.c_str() );
    if( ifs.good() ) {
        std::stringstream ss;
        ss << ifs.rdbuf();
        data = ss.str();
    } else {
        data = content;
    }

    // headers & cache
    int version = 0;
    if( data[0] == '#' && data[1] == 'v' ) {
        std::stringstream v;
        v << data;
        std::string tag;
        v >> tag;
        if( !(v >> version) ) {
            version = 0;
        }
    } else {
        std::stringstream v;
        v << data;
        if( !(v >> version) ) {
            version = 0;
        }
    }

    if( !version ) {
#ifdef GL_ES_VERSION_2_0
        version = 100;
#else
        //GLSL         // OPENGL
        version = 410; // GL_VERSION_4_1
        version = 400; // GL_VERSION_4_0
        version = 330; // GL_VERSION_3_3

        version = 150; // GL_VERSION_3_2
        version = 140; // GL_VERSION_3_1 --> removed attribute/varying
        version = 130; // GL_VERSION_3_0 --> deprecates attribute/varying
        version = 120; // GL_VERSION_2_1
        version = 110; // GL_VERSION_2_0

        version = 330;
#endif
    }
    data = replace(data, "#version", "//#version" );

    std::string info;
    switch( type ) {
        default:
        case GL_VERTEX_SHADER: info = "// --- GL_VERTEX_SHADER"; break;
        case GL_FRAGMENT_SHADER: info = "// --- GL_FRAGMENT_SHADER"; break;
        case GL_GEOMETRY_SHADER: info = "// --- GL_GEOMETRY_SHADER"; break;
        case GL_TESS_CONTROL_SHADER: info = "// --- GL_TESS_CONTROL_SHADER"; break;
        case GL_TESS_EVALUATION_SHADER: info = "// --- GL_TESS_EVALUATION_SHADER"; break;
    }
    if( content.size() < 32 ) info += " (" + content + ")\n"; else info += "\n";

    int extra = 0
        | (data.find("SAMPLER2D") != std::string::npos)
        | (data.find("TEXTURE2D") != std::string::npos)
        | (data.find("IN") != std::string::npos)
        | (data.find("OUT") != std::string::npos)
        | (data.find("ATTRIBUTE") != std::string::npos)
        | (data.find("VARYING") != std::string::npos)
        | (data.find("GL_FRAGCOLOR") != std::string::npos)
        | (data.find("GL_TEXCOORD") != std::string::npos);

    data = std::string()
        + info
        + std::string("#version ") + std::to_string(version) + "\n"
        + (extra ? get_glsl_headers(type,version) : std::string())
        + data;

    cache[type] = data;
    sources += data;

    const char *source = data.c_str();

    // Compile shader
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    stages.push_back(shader);

    // Check for errors
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if( compiled != GL_TRUE ) {
        char buffer[512];
        int length;
        glGetShaderInfoLog(shader, sizeof(buffer), &length, buffer);
        if( length ) {
            errors += error("compile error", buffer, source);
            //std::cout << "[ FAIL ] Error during shader compilation." << std::endl;
            //std::cout << data << std::endl;
        }
    }

    // Allow chaining
    return *this;
}

std::string Shader::link() {
    // Create and link program
    if (!id) id = glCreateProgram();
    for (size_t i = 0; i < stages.size(); i++) {
        glAttachShader(id, stages[i]);
    }
    glLinkProgram(id);

    // Check for errors
    GLint linked;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if( linked != GL_TRUE ) {
        char buffer[512];
        int length;
        glGetProgramInfoLog(id, sizeof(buffer), &length, buffer);
        if( length ) {
            errors += error("linkage error", buffer);
            //std::cout << "[ FAIL ] Error during shader linkage." << std::endl;
        }
    }

    return errors;
}

void VAO::check() {
    if (vertices && vertices->currentTarget() != GL_ARRAY_BUFFER) {
        printf("expected vertices to have GL_ARRAY_BUFFER, got 0x%04X\n", vertices->currentTarget());
        exit(0);
    }
    if (indices && indices->currentTarget() != GL_ELEMENT_ARRAY_BUFFER) {
        printf("expected indices to have GL_ELEMENT_ARRAY_BUFFER, got 0x%04X\n", indices->currentTarget());
        exit(0);
    }
    if (offset != stride) {
        printf("expected size of attributes (%d bytes) to add up to size of vertex (%d bytes)\n", offset, stride);
        exit(0);
    }
}
