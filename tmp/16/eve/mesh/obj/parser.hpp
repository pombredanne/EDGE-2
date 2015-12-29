#pragma once

#include <string>

namespace eve
{
    struct mesh;

    namespace loader {
        bool obj( eve::mesh &self, const std::string &pathfile );
    }

    struct lightwave {
        bool operator()( eve::mesh &self, const std::string &pathfile ) {
            return eve::loader::obj( self, pathfile );
        }
    };
}
