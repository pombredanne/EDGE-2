// Copyright (c) 2015 Sterling Orsten
//   This software is provided 'as-is', without any express or implied
// warranty. In no event will the author be held liable for any damages
// arising from the use of this software. You are granted a perpetual, 
// irrevocable, world-wide license to copy, modify, and redistribute
// this software for any purpose, including commercial applications.

#include "linalg-tests.h"
#include "catch.hpp"



template<class T, int M> void TestMatrixLayout()
{
    const linalg::mat<T,M,2> m2;
    REQUIRE(sizeof(m2) == sizeof(T)*M*2);
    REQUIRE(&m2.x < &m2.y);
    REQUIRE(&m2[0] == &m2.x);
    REQUIRE(&m2[1] == &m2.y);
    
    const linalg::mat<T,M,3> m3;
    REQUIRE(sizeof(m3) == sizeof(T)*M*3);
    REQUIRE(&m3.x < &m3.y);
    REQUIRE(&m3.y < &m3.z);
    REQUIRE(&m3[0] == &m3.x);
    REQUIRE(&m3[1] == &m3.y);
    REQUIRE(&m3[2] == &m3.z);

    const linalg::mat<T,M,4> m4;
    REQUIRE(sizeof(m4) == sizeof(T)*M*4);
    REQUIRE(&m4.x < &m4.y);
    REQUIRE(&m4.y < &m4.z);
    REQUIRE(&m4.z < &m4.w);
    REQUIRE(&m4[0] == &m4.x);
    REQUIRE(&m4[1] == &m4.y);
    REQUIRE(&m4[2] == &m4.z);
    REQUIRE(&m4[3] == &m4.w);
}

template<class T> void TestLayout()
{
    const linalg::vec<T,2> v2;
    REQUIRE(sizeof(v2) == sizeof(T)*2);
    REQUIRE(&v2.x < &v2.y);
    REQUIRE(&v2[0] == &v2.x);
    REQUIRE(&v2[1] == &v2.y);
    
    const linalg::vec<T,3> v3;
    REQUIRE(sizeof(v3) == sizeof(T)*3);
    REQUIRE(&v3.x < &v3.y);
    REQUIRE(&v3.y < &v3.z);
    REQUIRE(&v3[0] == &v3.x);
    REQUIRE(&v3[1] == &v3.y);
    REQUIRE(&v3[2] == &v3.z);

    const linalg::vec<T,4> v4;
    REQUIRE(sizeof(v4) == sizeof(T)*4);
    REQUIRE(&v4.x < &v4.y);
    REQUIRE(&v4.y < &v4.z);
    REQUIRE(&v4.z < &v4.w);
    REQUIRE(&v4[0] == &v4.x);
    REQUIRE(&v4[1] == &v4.y);
    REQUIRE(&v4[2] == &v4.z);
    REQUIRE(&v4[3] == &v4.w);

    TestMatrixLayout<T,2>();
    TestMatrixLayout<T,3>();
    TestMatrixLayout<T,4>();
}

TEST_CASE( "Vectors and matrices are laid out sequentially and tightly packed", "[layout]" )
{
    TestLayout<uint8_t>();
    TestLayout<int>();
    TestLayout<float>();
    TestLayout<double>();
}



template<class T> void CheckConstness(T &, bool expected) { REQUIRE(std::is_const<T>::value == expected); }

template<class T, int M> void TestConstArrayIndexOperator()
{
    linalg::vec<T,M> v;
    CheckConstness(v[0], false);

    const linalg::vec<T,M> cv;
    CheckConstness(cv[0], true);
}

TEST_CASE( "Array indexing syntax const correctness", "[accessors]" )
{
    TestConstArrayIndexOperator<int,2>();
    TestConstArrayIndexOperator<float,3>();
    TestConstArrayIndexOperator<double,4>();
}
