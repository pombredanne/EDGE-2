// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "linalg-tests.h"
#include "catch.hpp"

using namespace linalg::aliases;

TEST_CASE( "Vector valued functions", "[functions]" )
{
    float3 a = {-1,-2,-3}, b = {1,2,3};
    REQUIRE( dot(a,b) == a.x*b.x + a.y*b.y + a.z*b.z );
    REQUIRE( abs(a) == b );
    REQUIRE( min(a,b) == a );
    REQUIRE( max(a,b) == b );
}