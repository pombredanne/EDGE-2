// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "linalg-tests.h"
#include "catch.hpp"

template<class T, class U> void MatchTypes(const T &, const U &)
{
    REQUIRE(typeid(T) == typeid(U));
}

template<class T> void CheckMatrixVectorMultiply()
{
    // Check matrix by vector multiplication, M-element vectors act like M x 1 matrices
    MatchTypes(mul(linalg::mat<T,2,2>(), linalg::vec<T,2>()), linalg::vec<T,2>());
    MatchTypes(mul(linalg::mat<T,2,3>(), linalg::vec<T,3>()), linalg::vec<T,2>());
    MatchTypes(mul(linalg::mat<T,2,4>(), linalg::vec<T,4>()), linalg::vec<T,2>());

    MatchTypes(mul(linalg::mat<T,3,2>(), linalg::vec<T,2>()), linalg::vec<T,3>());
    MatchTypes(mul(linalg::mat<T,3,3>(), linalg::vec<T,3>()), linalg::vec<T,3>());
    MatchTypes(mul(linalg::mat<T,3,4>(), linalg::vec<T,4>()), linalg::vec<T,3>());

    MatchTypes(mul(linalg::mat<T,4,2>(), linalg::vec<T,2>()), linalg::vec<T,4>());
    MatchTypes(mul(linalg::mat<T,4,3>(), linalg::vec<T,3>()), linalg::vec<T,4>());
    MatchTypes(mul(linalg::mat<T,4,4>(), linalg::vec<T,4>()), linalg::vec<T,4>());
}

TEST_CASE( "Matrix x vector multiplication obeys size rules", "[matrix]" )
{
    CheckMatrixVectorMultiply<int>();
    CheckMatrixVectorMultiply<float>();
}

template<class T> void CheckMatrixMatrixMultiply()
{
    // Check matrix by matrix multiplication, M x N matrices multiplied with N x O matrices form M x O matrices
    MatchTypes(mul(linalg::mat<T,2,2>(), linalg::mat<T,2,2>()), linalg::mat<T,2,2>());
    MatchTypes(mul(linalg::mat<T,2,3>(), linalg::mat<T,3,2>()), linalg::mat<T,2,2>());
    MatchTypes(mul(linalg::mat<T,2,4>(), linalg::mat<T,4,2>()), linalg::mat<T,2,2>());

    MatchTypes(mul(linalg::mat<T,2,2>(), linalg::mat<T,2,3>()), linalg::mat<T,2,3>());
    MatchTypes(mul(linalg::mat<T,2,3>(), linalg::mat<T,3,3>()), linalg::mat<T,2,3>());
    MatchTypes(mul(linalg::mat<T,2,4>(), linalg::mat<T,4,3>()), linalg::mat<T,2,3>());

    MatchTypes(mul(linalg::mat<T,2,2>(), linalg::mat<T,2,4>()), linalg::mat<T,2,4>());
    MatchTypes(mul(linalg::mat<T,2,3>(), linalg::mat<T,3,4>()), linalg::mat<T,2,4>());
    MatchTypes(mul(linalg::mat<T,2,4>(), linalg::mat<T,4,4>()), linalg::mat<T,2,4>());

    MatchTypes(mul(linalg::mat<T,3,2>(), linalg::mat<T,2,2>()), linalg::mat<T,3,2>());
    MatchTypes(mul(linalg::mat<T,3,3>(), linalg::mat<T,3,2>()), linalg::mat<T,3,2>());
    MatchTypes(mul(linalg::mat<T,3,4>(), linalg::mat<T,4,2>()), linalg::mat<T,3,2>());

    MatchTypes(mul(linalg::mat<T,3,2>(), linalg::mat<T,2,3>()), linalg::mat<T,3,3>());
    MatchTypes(mul(linalg::mat<T,3,3>(), linalg::mat<T,3,3>()), linalg::mat<T,3,3>());
    MatchTypes(mul(linalg::mat<T,3,4>(), linalg::mat<T,4,3>()), linalg::mat<T,3,3>());

    MatchTypes(mul(linalg::mat<T,3,2>(), linalg::mat<T,2,4>()), linalg::mat<T,3,4>());
    MatchTypes(mul(linalg::mat<T,3,3>(), linalg::mat<T,3,4>()), linalg::mat<T,3,4>());
    MatchTypes(mul(linalg::mat<T,3,4>(), linalg::mat<T,4,4>()), linalg::mat<T,3,4>());

    MatchTypes(mul(linalg::mat<T,4,2>(), linalg::mat<T,2,2>()), linalg::mat<T,4,2>());
    MatchTypes(mul(linalg::mat<T,4,3>(), linalg::mat<T,3,2>()), linalg::mat<T,4,2>());
    MatchTypes(mul(linalg::mat<T,4,4>(), linalg::mat<T,4,2>()), linalg::mat<T,4,2>());

    MatchTypes(mul(linalg::mat<T,4,2>(), linalg::mat<T,2,3>()), linalg::mat<T,4,3>());
    MatchTypes(mul(linalg::mat<T,4,3>(), linalg::mat<T,3,3>()), linalg::mat<T,4,3>());
    MatchTypes(mul(linalg::mat<T,4,4>(), linalg::mat<T,4,3>()), linalg::mat<T,4,3>());

    MatchTypes(mul(linalg::mat<T,4,2>(), linalg::mat<T,2,4>()), linalg::mat<T,4,4>());
    MatchTypes(mul(linalg::mat<T,4,3>(), linalg::mat<T,3,4>()), linalg::mat<T,4,4>());
    MatchTypes(mul(linalg::mat<T,4,4>(), linalg::mat<T,4,4>()), linalg::mat<T,4,4>());
}

TEST_CASE( "Matrix x matrix multiplication obeys size rules", "[matrix]" )
{
    CheckMatrixMatrixMultiply<int>();
    CheckMatrixMatrixMultiply<float>();
}

template<class T> void CheckRowRetrieval()
{
    // An M x N matrix has M rows each containing N elements
    MatchTypes(linalg::mat<T,2,2>().row(0), linalg::vec<T,2>());
    MatchTypes(linalg::mat<T,2,3>().row(0), linalg::vec<T,3>());
    MatchTypes(linalg::mat<T,2,4>().row(0), linalg::vec<T,4>());

    MatchTypes(linalg::mat<T,3,2>().row(0), linalg::vec<T,2>());
    MatchTypes(linalg::mat<T,3,3>().row(0), linalg::vec<T,3>());
    MatchTypes(linalg::mat<T,3,4>().row(0), linalg::vec<T,4>());

    MatchTypes(linalg::mat<T,4,2>().row(0), linalg::vec<T,2>());
    MatchTypes(linalg::mat<T,4,3>().row(0), linalg::vec<T,3>());
    MatchTypes(linalg::mat<T,4,4>().row(0), linalg::vec<T,4>());
}

TEST_CASE( "Matrix rows are the correct size", "[matrix]" )
{
    CheckRowRetrieval<int>();
    CheckRowRetrieval<float>();
}

template<class T> void CheckTranspose()
{
    // The transpose of an M x N matrix is an N x M matrix
    MatchTypes(transpose(linalg::mat<T,2,2>()), linalg::mat<T,2,2>());
    MatchTypes(transpose(linalg::mat<T,2,3>()), linalg::mat<T,3,2>());
    MatchTypes(transpose(linalg::mat<T,2,4>()), linalg::mat<T,4,2>());

    MatchTypes(transpose(linalg::mat<T,3,2>()), linalg::mat<T,2,3>());
    MatchTypes(transpose(linalg::mat<T,3,3>()), linalg::mat<T,3,3>());
    MatchTypes(transpose(linalg::mat<T,3,4>()), linalg::mat<T,4,3>());

    MatchTypes(transpose(linalg::mat<T,4,2>()), linalg::mat<T,2,4>());
    MatchTypes(transpose(linalg::mat<T,4,3>()), linalg::mat<T,3,4>());
    MatchTypes(transpose(linalg::mat<T,4,4>()), linalg::mat<T,4,4>());
}

TEST_CASE( "Transposed matrices are the correct size", "[matrix]" )
{
    CheckTranspose<int>();
    CheckTranspose<float>();
}