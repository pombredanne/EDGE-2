#pragma once
#include <string>

namespace stdxx {
    using std::to_string;
    static inline const std::string &to_string( const std::string &s ) {
        return s;
    }
}

template< typename A >
int warn( const A &a )
{
    std::cout << "warning: " << stdxx::to_string( a ) << std::endl;
    return 1;
}

template< typename A, typename B >
int warn( const A &a, const B &b )
{
    std::cout << warn(a) << " " << stdxx::to_string( b ) << std::endl;
    return 1;
}

template< typename A, typename B, typename C >
int warn( const A &a, const B &b, const C &c )
{
    std::cout << warn(a,b) << " " << stdxx::to_string( c ) << std::endl;
    return 1;
}

#if defined(NDEBUG) || defined(_NDEBUG)
#define warn(...) (1)
#endif
