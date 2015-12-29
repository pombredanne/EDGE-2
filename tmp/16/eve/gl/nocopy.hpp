#pragma once

namespace eve
{
    struct nocopy
    {
        protected: nocopy() {}
        private:   nocopy( const nocopy &other );
        private:   nocopy operator=( const nocopy &other );
    };
}
