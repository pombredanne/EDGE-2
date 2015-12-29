// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "linalg-tests.h"
#include "catch.hpp"



// Ensure that basic arithmetic operations exist and are implemented componentwise
struct UnaryOperatorTest
{
    template<class T, int M> void Run(Random & r, linalg::vec<T,M> *) const
    {
        const auto vec = r.Get<linalg::vec<T,M>>(), pos = +vec, neg = -vec;
        for(int i=0; i<M; ++i)
        {
            REQUIRE(pos[i] == +vec[i]);
            REQUIRE(neg[i] == -vec[i]);
        }
    }

    template<class T, int M, int N> void Run(Random & r, linalg::mat<T,M,N> *) const
    {
        const auto mat = r.Get<linalg::mat<T,M,N>>(), pos = +mat, neg = -mat;
        for(int j=0; j<N; ++j) for(int i=0; i<M; ++i)
        {
            REQUIRE(pos(i,j) == +mat(i,j));
            REQUIRE(neg(i,j) == -mat(i,j));
        }
    }
};

TEST_CASE( "Unary operators", "[operators]" )
{
    RunTest(UnaryOperatorTest(), 10);
}



struct EqualityTest
{
    template<class T, int M> void Run(Random & r, linalg::vec<T,M> *) const
    {
        // Any vector must be equal to itself, and to copies of itself (unless it is a float vector containing NaNs...)
        const auto original = r.Get<linalg::vec<T,M>>(), copy = original;
        REQUIRE(original == original);
        REQUIRE_FALSE(original != original);
        REQUIRE(copy == original);
        REQUIRE_FALSE(copy != original);

        // If any component is modified, the modified vector must not be equal to the original
        for(int i=0; i<M; ++i)
        {
            auto modified = original;
            modified[i] += T(1);
            REQUIRE_FALSE(modified == original);
            REQUIRE(modified != original);

            REQUIRE(any(modified > original) == true);
            REQUIRE(all(modified > original) == false);
        }
    }

    template<class T, int M, int N> void Run(Random & r, linalg::mat<T,M,N> *) const
    {
        // Any matrix must be equal to itself, and to copies of itself (unless it is a float vector containing NaNs...)
        const auto original = r.Get<linalg::mat<T,M,N>>(), copy = original;
        REQUIRE(original == original);
        REQUIRE_FALSE(original != original);
        REQUIRE(copy == original);
        REQUIRE_FALSE(copy != original);

        // If any component is modified, the modified matrix must not be equal to the original
        for(int j=0; j<N; ++j) for(int i=0; i<M; ++i)
        {
            auto modified = original;
            modified(i,j) += T(1);
            REQUIRE_FALSE(modified == original);
            REQUIRE(modified != original);

            REQUIRE(any(modified > original) == true);
            REQUIRE(all(modified > original) == false);
        }
    }
};

TEST_CASE( "Equality and inequality operators", "[operators]" )
{
    RunTest(EqualityTest(), 10);
}



struct VectorVectorComparisonTest
{
    template<class T, int M> void Run(Random & r, linalg::vec<T,M> *) const
    {
        const auto left = r.Get<linalg::vec<T,M>>(), right = r.Get<linalg::vec<T,M>>();
        const auto less = left < right, lequal = left <= right, greater = left > right, gequal = left >= right;
        for(int i=0; i<M; ++i)
        {
            REQUIRE(less[i]     == left[i] <  right[i]);
            REQUIRE(lequal[i]   == left[i] <= right[i]);
            REQUIRE(greater[i]  == left[i] >  right[i]);
            REQUIRE(gequal[i]   == left[i] >= right[i]);
        }
    }

    template<class T, int M, int N> void Run(Random & r, linalg::mat<T,M,N> *) const
    {
        const auto left = r.Get<linalg::mat<T,M,N>>(), right = r.Get<linalg::mat<T,M,N>>();
        const auto less = left < right, lequal = left <= right, greater = left > right, gequal = left >= right;
        for(int j=0; j<N; ++j) for(int i=0; i<M; ++i)
        {
            REQUIRE(less(i,j)     == left(i,j) <  right(i,j));
            REQUIRE(lequal(i,j)   == left(i,j) <= right(i,j));
            REQUIRE(greater(i,j)  == left(i,j) >  right(i,j));
            REQUIRE(gequal(i,j)   == left(i,j) >= right(i,j));
        }
    }
};

TEST_CASE( "Comparison operators (vector x vector)", "[operators]" )
{
    RunTest(VectorVectorComparisonTest(), 10);
}



struct VectorVectorArithmeticTest
{
    template<class T, int M> void Run(Random & r, linalg::vec<T,M> *) const
    {
        const auto left = r.Get<linalg::vec<T,M>>(), right = r.GetDivisor<linalg::vec<T,M>>();
        const auto sum = left + right, difference = left - right, product = left * right, quotient = left / right;
        for(int i=0; i<M; ++i)
        {
            REQUIRE(sum[i]        == left[i] + right[i]);
            REQUIRE(difference[i] == left[i] - right[i]);
            REQUIRE(product[i]    == left[i] * right[i]);
            REQUIRE(quotient[i]   == left[i] / right[i]);
        }
    }

    template<class T, int M, int N> void Run(Random & r, linalg::mat<T,M,N> *) const
    {
        const auto left = r.Get<linalg::mat<T,M,N>>(), right = r.GetDivisor<linalg::mat<T,M,N>>();
        const auto sum = left + right, difference = left - right, product = left * right, quotient = left / right;
        for(int j=0; j<N; ++j) for(int i=0; i<M; ++i)
        {
            REQUIRE(sum(i,j)        == left(i,j) + right(i,j));
            REQUIRE(difference(i,j) == left(i,j) - right(i,j));
            REQUIRE(product(i,j)    == left(i,j) * right(i,j));
            REQUIRE(quotient(i,j)   == left(i,j) / right(i,j));
        }
    }
};

TEST_CASE( "Arithmetic operators (vector x vector)", "[operators]" )
{
    RunTest(VectorVectorArithmeticTest(), 10);
}



struct VectorScalarArithmeticTest
{
    template<class T, int M> void Run(Random & r, linalg::vec<T,M> *) const
    {
        const auto left = r.Get<linalg::vec<T,M>>();
        const auto right = r.GetDivisor<T>();
        const auto sum = left + right, difference = left - right, product = left * right, quotient = left / right;
        for(int i=0; i<M; ++i)
        {
            REQUIRE(sum[i]        == left[i] + right);
            REQUIRE(difference[i] == left[i] - right);
            REQUIRE(product[i]    == left[i] * right);
            REQUIRE(quotient[i]   == left[i] / right);
        }
    }

    template<class T, int M, int N> void Run(Random & r, linalg::mat<T,M,N> *) const
    {
        const auto left = r.Get<linalg::mat<T,M,N>>();
        const auto right = r.GetDivisor<T>();
        const auto sum = left + right, difference = left - right, product = left * right, quotient = left / right;
        for(int j=0; j<N; ++j) for(int i=0; i<M; ++i)
        {
            REQUIRE(sum(i,j)        == left(i,j) + right);
            REQUIRE(difference(i,j) == left(i,j) - right);
            REQUIRE(product(i,j)    == left(i,j) * right);
            REQUIRE(quotient(i,j)   == left(i,j) / right);
        }
    }
};

TEST_CASE( "Arithmetic operators (vector x scalar)", "[operators]" )
{
    RunTest(VectorScalarArithmeticTest(), 10);
}



struct ScalarVectorArithmeticTest
{
    template<class T, int M> void Run(Random & r, linalg::vec<T,M> *) const
    {
        const auto left = r.Get<T>();
        const auto right = r.GetDivisor<linalg::vec<T,M>>();
        const auto sum = left + right, difference = left - right, product = left * right, quotient = left / right;
        for(int i=0; i<M; ++i)
        {
            REQUIRE(sum[i]        == left + right[i]);
            REQUIRE(difference[i] == left - right[i]);
            REQUIRE(product[i]    == left * right[i]);
            REQUIRE(quotient[i]   == left / right[i]);
        }
    }

    template<class T, int M, int N> void Run(Random & r, linalg::mat<T,M,N> *) const
    {
        const auto left = r.Get<T>();
        const auto right = r.GetDivisor<linalg::mat<T,M,N>>();
        const auto sum = left + right, difference = left - right, product = left * right, quotient = left / right;
        for(int j=0; j<N; ++j) for(int i=0; i<M; ++i)
        {
            REQUIRE(sum(i,j)        == left + right(i,j));
            REQUIRE(difference(i,j) == left - right(i,j));
            REQUIRE(product(i,j)    == left * right(i,j));
            REQUIRE(quotient(i,j)   == left / right(i,j));
        }
    }
};

TEST_CASE( "Arithmetic operators (scalar x vector)", "[operators]" )
{
    RunTest(ScalarVectorArithmeticTest(), 10);
}



struct VectorVectorAssignmentArithmeticTest
{
    template<class T, int M> void Run(Random & r, linalg::vec<T,M> *) const
    {
        const auto left = r.Get<linalg::vec<T,M>>(), right = r.GetDivisor<linalg::vec<T,M>>();
        auto sum = left, difference = left, product = left, quotient = left;
        sum += right; difference -= right; product *= right; quotient /= right;
        for(int i=0; i<M; ++i)
        {
            REQUIRE(sum[i]        == left[i] + right[i]);
            REQUIRE(difference[i] == left[i] - right[i]);
            REQUIRE(product[i]    == left[i] * right[i]);
            REQUIRE(quotient[i]   == left[i] / right[i]);
        }
    }

    template<class T, int M, int N> void Run(Random & r, linalg::mat<T,M,N> *) const
    {
        const auto left = r.Get<linalg::mat<T,M,N>>(), right = r.GetDivisor<linalg::mat<T,M,N>>();
        auto sum = left, difference = left, product = left, quotient = left;
        sum += right; difference -= right; product *= right; quotient /= right;
        for(int j=0; j<N; ++j) for(int i=0; i<M; ++i)
        {
            REQUIRE(sum(i,j)        == left(i,j) + right(i,j));
            REQUIRE(difference(i,j) == left(i,j) - right(i,j));
            REQUIRE(product(i,j)    == left(i,j) * right(i,j));
            REQUIRE(quotient(i,j)   == left(i,j) / right(i,j));
        }
    }
};

TEST_CASE( "Arithmetic operators (vector x= vector)", "[operators]" )
{
    RunTest(VectorVectorAssignmentArithmeticTest(), 10);
}



struct VectorScalarAssignmentArithmeticTest
{
    template<class T, int M> void Run(Random & r, linalg::vec<T,M> *) const
    {
        const auto left = r.Get<linalg::vec<T,M>>();
        const auto right = r.GetDivisor<T>();
        auto sum = left, difference = left, product = left, quotient = left;
        sum += right; difference -= right; product *= right; quotient /= right;
        for(int i=0; i<M; ++i)
        {
            REQUIRE(sum[i]        == left[i] + right);
            REQUIRE(difference[i] == left[i] - right);
            REQUIRE(product[i]    == left[i] * right);
            REQUIRE(quotient[i]   == left[i] / right);
        }
    }

    template<class T, int M, int N> void Run(Random & r, linalg::mat<T,M,N> *) const
    {
        const auto left = r.Get<linalg::mat<T,M,N>>();
        const auto right = r.GetDivisor<T>();
        auto sum = left, difference = left, product = left, quotient = left;
        sum += right; difference -= right; product *= right; quotient /= right;
        for(int j=0; j<N; ++j) for(int i=0; i<M; ++i)
        {
            REQUIRE(sum(i,j)        == left(i,j) + right);
            REQUIRE(difference(i,j) == left(i,j) - right);
            REQUIRE(product(i,j)    == left(i,j) * right);
            REQUIRE(quotient(i,j)   == left(i,j) / right);
        }
    }
};

TEST_CASE( "Arithmetic operators (vector x= scalar)", "[operators]" )
{
    RunTest(VectorVectorAssignmentArithmeticTest(), 10);
}