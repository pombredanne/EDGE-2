#pragma once

#include "texture.hpp"
#include <deque>

namespace eve
{
    namespace loader
    {
        std::deque< eve::texture * > gif( const std::string &filename, size_t frame_start = 0, size_t frame_end = ~0 );
    }
}
