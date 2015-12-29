#pragma once

#include "gl.hpp"
#include "nocopy.hpp"

namespace eve
{
    namespace invert
    {
        struct normals : nocopy
        {
            normals()
            {
                glGetIntegerv( GL_FRONT_FACE, &mode );
                glFrontFace( mode == GL_CW ? GL_CCW : GL_CW );
            }

            ~normals()
            {
                glFrontFace( mode );
            }

            protected: GLint mode;
        };
    }
}
