#pragma once

namespace eve
{
    void init_ogl();
    void check_ogl(const char *file, int line);

    template<typename T>
    T check_ogl( const char *file, int line, const T &t ) {
        check_ogl( file, line );
        return t;
    }
}

// _0 if fn returns void, else _N
#if defined(NDEBUG) || defined(_NDEBUG)
#define EVE_GL_0(...) (__VA_ARGS__)
#define EVE_GL_N(...) (__VA_ARGS__)
#else
#define EVE_GL_0(...) ((__VA_ARGS__), eve::check_ogl(__FILE__, __LINE__))
#define EVE_GL_N(...) eve::check_ogl( __FILE__, __LINE__, (__VA_ARGS__) )
#endif

#define $glCreateProgram(...)           EVE_GL_N( glCreateProgram(__VA_ARGS__) )
#define $glCreateShader(...)            EVE_GL_N( glCreateShader(__VA_ARGS__) )
#define $glGetAttribLocation(...)       EVE_GL_N( glGetAttribLocation(__VA_ARGS__) )
#define $glGetFragDataLocation(...)     EVE_GL_N( glGetFragDataLocation(__VA_ARGS__) )
#define $glGetUniformLocation(...)      EVE_GL_N( glGetUniformLocation(__VA_ARGS__) )
#define $glIsBuffer(...)                EVE_GL_N( glIsBuffer(__VA_ARGS__) )
#define $glIsProgram(...)               EVE_GL_N( glIsProgram(__VA_ARGS__) )
#define $glIsQuery(...)                 EVE_GL_N( glIsQuery(__VA_ARGS__) )
#define $glIsShader(...)                EVE_GL_N( glIsShader(__VA_ARGS__) )
#define $glIsVertexArray(...)           EVE_GL_N( glIsVertexArray(__VA_ARGS__) )
#define $glMapBuffer(...)               EVE_GL_N( glMapBuffer(__VA_ARGS__) )
#define $gluBuild2DMipmaps(...)         EVE_GL_N( gluBuild2DMipmaps(__VA_ARGS__) )
#define $glUnmapBuffer(...)             EVE_GL_N( glUnmapBuffer(__VA_ARGS__) )

#define $glActiveTexture(...)           EVE_GL_0( glActiveTexture(__VA_ARGS__) )
#define $glActiveTextureARB(...)        EVE_GL_0( glActiveTextureARB(__VA_ARGS__) )
#define $glAlphaFunc(...)               EVE_GL_0( glAlphaFunc(__VA_ARGS__) )
#define $glAttachShader(...)            EVE_GL_0( glAttachShader(__VA_ARGS__) )
#define $glBegin(...)                   EVE_GL_0( glBegin(__VA_ARGS__) )
#define $glBindBuffer(...)              EVE_GL_0( glBindBuffer(__VA_ARGS__) )
#define $glBindFramebufferEXT(...)      EVE_GL_0( glBindFramebufferEXT(__VA_ARGS__) )
#define $glBindTexture(...)             EVE_GL_0( glBindTexture(__VA_ARGS__) )
#define $glBlendFunc(...)               EVE_GL_0( glBlendFunc(__VA_ARGS__) )
#define $glBufferData(...)              EVE_GL_0( glBufferData(__VA_ARGS__) )
#define $glClear(...)                   EVE_GL_0( glClear(__VA_ARGS__) )
#define $glClearColor(...)              EVE_GL_0( glClearColor(__VA_ARGS__) )
#define $glColorMask(...)               EVE_GL_0( glColorMask(__VA_ARGS__) )
#define $glColor3ub(...)                EVE_GL_0( glColor3ub(__VA_ARGS__) )
#define $glCompileShader(...)           EVE_GL_0( glCompileShader(__VA_ARGS__) )
#define $glDeleteBuffers(...)           EVE_GL_0( glDeleteBuffers(__VA_ARGS__) )
#define $glDeleteFramebuffersEXT(...)   EVE_GL_0( glDeleteFramebuffersEXT(__VA_ARGS__) )
#define $glDeleteProgram(...)           EVE_GL_0( glDeleteProgram(__VA_ARGS__) )
#define $glDeleteShader(...)            EVE_GL_0( glDeleteShader(__VA_ARGS__) )
#define $glDeleteTextures(...)          EVE_GL_0( glDeleteTextures(__VA_ARGS__) )
#define $glDetachShader(...)            EVE_GL_0( glDetachShader(__VA_ARGS__) )
#define $glDisable(...)                 EVE_GL_0( glDisable(__VA_ARGS__) )
#define $glEnable(...)                  EVE_GL_0( glEnable(__VA_ARGS__) )
#define $glEnd(...)                     EVE_GL_0( glEnd(__VA_ARGS__) )
#define $glFogf(...)                    EVE_GL_0( glFogf(__VA_ARGS__) )
#define $glFogfv(...)                   EVE_GL_0( glFogfv(__VA_ARGS__) )
#define $glFogi(...)                    EVE_GL_0( glFogi(__VA_ARGS__) )
#define $glFramebufferTexture2DEXT(...) EVE_GL_0( glFramebufferTexture2DEXT(__VA_ARGS__) )
#define $glFrustum(...)                 EVE_GL_0( glFrustum(__VA_ARGS__) )
#define $glGenBuffers(...)              EVE_GL_0( glGenBuffers(__VA_ARGS__) )
#define $glGenFramebuffersEXT(...)      EVE_GL_0( glGenFramebuffersEXT(__VA_ARGS__) )
#define $glGenTextures(...)             EVE_GL_0( glGenTextures(__VA_ARGS__) )
#define $glGetIntegerv(...)             EVE_GL_0( glGetIntegerv(__VA_ARGS__) )
#define $glGetProgramInfoLog(...)       EVE_GL_0( glGetProgramInfoLog(__VA_ARGS__) )
#define $glGetProgramiv(...)            EVE_GL_0( glGetProgramiv(__VA_ARGS__) )
#define $glGetShaderInfoLog(...)        EVE_GL_0( glGetShaderInfoLog(__VA_ARGS__) )
#define $glGetShaderiv(...)             EVE_GL_0( glGetShaderiv(__VA_ARGS__) )
#define $glLightfv(...)                 EVE_GL_0( glLightfv(__VA_ARGS__) )
#define $glLightModelfv(...)            EVE_GL_0( glLightModelfv(__VA_ARGS__) )
#define $glLinkProgram(...)             EVE_GL_0( glLinkProgram(__VA_ARGS__) )
#define $glLoadIdentity(...)            EVE_GL_0( glLoadIdentity(__VA_ARGS__) )
#define $glMatrixMode(...)              EVE_GL_0( glMatrixMode(__VA_ARGS__) )
#define $glMultMatrixf(...)             EVE_GL_0( glMultMatrixf(__VA_ARGS__) )
#define $glNormal3fv(...)               EVE_GL_0( glNormal3fv(__VA_ARGS__) )
#define $glOrtho(...)                   EVE_GL_0( glOrtho(__VA_ARGS__) )
#define $glPixelStorei(...)             EVE_GL_0( glPixelStorei(__VA_ARGS__) )
#define $glPolygonMode(...)             EVE_GL_0( glPolygonMode(__VA_ARGS__) )
#define $glPolygonOffset(...)           EVE_GL_0( glPolygonOffset(__VA_ARGS__) )
#define $glPopAttrib(...)               EVE_GL_0( glPopAttrib(__VA_ARGS__) )
#define $glPopMatrix(...)               EVE_GL_0( glPopMatrix(__VA_ARGS__) )
#define $glPushAttrib(...)              EVE_GL_0( glPushAttrib(__VA_ARGS__) )
#define $glPushMatrix(...)              EVE_GL_0( glPushMatrix(__VA_ARGS__) )
#define $glReadPixels(...)              EVE_GL_0( glReadPixels(__VA_ARGS__) )
#define $glRotatef(...)                 EVE_GL_0( glRotatef(__VA_ARGS__) )
#define $glScalef(...)                  EVE_GL_0( glScalef(__VA_ARGS__) )
#define $glScissor(...)                 EVE_GL_0( glScissor(__VA_ARGS__) )
#define $glShaderSource(...)            EVE_GL_0( glShaderSource(__VA_ARGS__) )
#define $glTexCoord2f(...)              EVE_GL_0( glTexCoord2f(__VA_ARGS__) )
#define $glTexCoord2i(...)              EVE_GL_0( glTexCoord2i(__VA_ARGS__) )
#define $glTexCoord3fv(...)             EVE_GL_0( glTexCoord3fv(__VA_ARGS__) )
#define $glTexImage2D(...)              EVE_GL_0( glTexImage2D(__VA_ARGS__) )
#define $glTexParameteri(...)           EVE_GL_0( glTexParameteri(__VA_ARGS__) )
#define $glTranslatef(...)              EVE_GL_0( glTranslatef(__VA_ARGS__) )
#define $glUniform1f(...)               EVE_GL_0( glUniform1f(__VA_ARGS__) )
#define $glUniform1i(...)               EVE_GL_0( glUniform1i(__VA_ARGS__) )
#define $glUniform2f(...)               EVE_GL_0( glUniform2f(__VA_ARGS__) )
#define $glUniform2fv(...)              EVE_GL_0( glUniform2fv(__VA_ARGS__) )
#define $glUniform2i(...)               EVE_GL_0( glUniform2i(__VA_ARGS__) )
#define $glUniform2iv(...)              EVE_GL_0( glUniform2iv(__VA_ARGS__) )
#define $glUniform3f(...)               EVE_GL_0( glUniform3f(__VA_ARGS__) )
#define $glUniform3fv(...)              EVE_GL_0( glUniform3fv(__VA_ARGS__) )
#define $glUniform3i(...)               EVE_GL_0( glUniform3i(__VA_ARGS__) )
#define $glUniform3iv(...)              EVE_GL_0( glUniform3iv(__VA_ARGS__) )
#define $glUniform4f(...)               EVE_GL_0( glUniform4f(__VA_ARGS__) )
#define $glUniform4fv(...)              EVE_GL_0( glUniform4fv(__VA_ARGS__) )
#define $glUniform4i(...)               EVE_GL_0( glUniform4i(__VA_ARGS__) )
#define $glUniform4iv(...)              EVE_GL_0( glUniform4iv(__VA_ARGS__) )
#define $glUniformMatrix4fv(...)        EVE_GL_0( glUniformMatrix4fv(__VA_ARGS__) )
#define $glUseProgram(...)              EVE_GL_0( glUseProgram(__VA_ARGS__) )
#define $glVertex3f(...)                EVE_GL_0( glVertex3f(__VA_ARGS__) )
#define $glVertex4fv(...)               EVE_GL_0( glVertex4fv(__VA_ARGS__) )
#define $glViewport(...)                EVE_GL_0( glViewport(__VA_ARGS__) )

// this needs proper 0/N check
#define $glBindAttribLocation(...)      EVE_GL_0( glBindAttribLocation(__VA_ARGS__) )
#define $glBindBufferBase(...)          EVE_GL_0( glBindBufferBase(__VA_ARGS__) )
#define $glBufferSubData(...)           EVE_GL_0( glBufferSubData(__VA_ARGS__) )
#define $glClearDepth(...)              EVE_GL_0( glClearDepth(__VA_ARGS__) )
#define $glColor3f(...)                 EVE_GL_0( glColor3f(__VA_ARGS__) )
#define $glCullFace(...)                EVE_GL_0( glCullFace(__VA_ARGS__) )
#define $glDepthFunc(...)               EVE_GL_0( glDepthFunc(__VA_ARGS__) )
#define $glDisableClientState(...)      EVE_GL_0( glDisableClientState(__VA_ARGS__) )
#define $glDisableVertexAttribArray(...) EVE_GL_0( glDisableVertexAttribArray(__VA_ARGS__) )
#define $glDrawElements(...)            EVE_GL_0( glDrawElements(__VA_ARGS__) )
#define $glEnableClientState(...)       EVE_GL_0( glEnableClientState(__VA_ARGS__) )
#define $glEnableVertexAttribArray(...) EVE_GL_0( glEnableVertexAttribArray(__VA_ARGS__) )
#define $glGetActiveUniformBlockiv(...) EVE_GL_0( glGetActiveUniformBlockiv(__VA_ARGS__) )
#define $glGetActiveUniformsiv(...)     EVE_GL_0( glGetActiveUniformsiv(__VA_ARGS__) )
#define $glGetUniformIndices(...)       EVE_GL_0( glGetUniformIndices(__VA_ARGS__) )
#define $glMaterialfv(...)              EVE_GL_0( glMaterialfv(__VA_ARGS__) )
#define $glNormalPointer(...)           EVE_GL_0( glNormalPointer(__VA_ARGS__) )
#define $glTexCoordPointer(...)         EVE_GL_0( glTexCoordPointer(__VA_ARGS__) )
#define $glUniformBlockBinding(...)     EVE_GL_0( glUniformBlockBinding(__VA_ARGS__) )
#define $glUniformMatrix3x4fv(...)      EVE_GL_0( glUniformMatrix3x4fv(__VA_ARGS__) )
#define $glVertexAttribPointer(...)     EVE_GL_0( glVertexAttribPointer(__VA_ARGS__) )
#define $glVertexPointer(...)           EVE_GL_0( glVertexPointer(__VA_ARGS__) )

#include "ext.hpp"
#include "shader.hpp"
#include "vbo.hpp"
#include "fbo.hpp"

// context
// window
// light
// mesh
// sprite
// snapshot
// camera
