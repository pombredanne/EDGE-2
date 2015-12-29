#pragma once

/* Using GLM for our transformation matrices */
#include <glm/glm.hpp> // 4minicam
#include <glm/gtc/matrix_transform.hpp> // 4minicam
#include <glm/gtc/type_ptr.hpp>

namespace geo
{
    struct mat4 : public glm::mat4 {
        mat4() : glm::mat4()
        {}
        template<typename T>
        mat4(const T &t) : glm::mat4(t)
        {}
        const float *data() const {
            const glm::mat4 &self = *this;
            return glm::value_ptr( self );
        }
        operator const float *() const {
            return data();
        }
    };
}
