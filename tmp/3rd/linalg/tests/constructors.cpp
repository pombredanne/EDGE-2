// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "linalg-tests.h"
#include "catch.hpp"



struct DefaultConstructorTest
{
    template<class T, int M> void Run(Random &, linalg::vec<T,M> *) const
    {
        const linalg::vec<T,M> v;
        for(int i=0; i<M; ++i)
        {
            REQUIRE(v[i] == T());
        }
    }

    template<class T, int M, int N> void Run(Random &, linalg::mat<T,M,N> *) const
    {
        const linalg::mat<T,M,N> m;
        for(int j=0; j<N; ++j) for(int i=0; i<M; ++i)
        {
            REQUIRE(m(i,j) == T());
        }
    }
};

TEST_CASE( "Default construct to zero", "[constructors]" )
{
    RunTest(DefaultConstructorTest(), 1);
}



struct ScalarConstructorTest
{
    template<class T, int M> void Run(Random & r, linalg::vec<T,M> *) const
    {
        T scalar = r.Get<T>();
        const linalg::vec<T,M> v { scalar };
        for(int i=0; i<M; ++i)
        {
            REQUIRE(v[i] == scalar);
        }
    }

    template<class T, int M, int N> void Run(Random & r, linalg::mat<T,M,N> *) const
    {
        T scalar = r.Get<T>();
        const linalg::mat<T,M,N> m { scalar };
        for(int j=0; j<N; ++j) for(int i=0; i<M; ++i)
        {
            REQUIRE(m(i,j) == scalar);
        }
    }
};

TEST_CASE( "Construct with a scalar", "[constructors]" )
{
    RunTest(ScalarConstructorTest(), 10);
}



struct ComponentConstructorTest
{
    template<class T> void Run(Random & r, linalg::vec<T,2> *) const
    {
        auto nums = r.GetNumbers<T>(2);
        const linalg::vec<T,2> v { nums[0], nums[1] };
        REQUIRE(v.x == nums[0]);
        REQUIRE(v.y == nums[1]);
    }

    template<class T> void Run(Random & r, linalg::vec<T,3> *) const
    {
        auto nums = r.GetNumbers<T>(3);
        const linalg::vec<T,3> v { nums[0], nums[1], nums[2] };
        REQUIRE(v.x == nums[0]);
        REQUIRE(v.y == nums[1]);
        REQUIRE(v.z == nums[2]);
    }

    template<class T> void Run(Random & r, linalg::vec<T,4> *) const
    {
        auto nums = r.GetNumbers<T>(4);
        const linalg::vec<T,4> v { nums[0], nums[1], nums[2], nums[3] };
        REQUIRE(v.x == nums[0]);
        REQUIRE(v.y == nums[1]);
        REQUIRE(v.z == nums[2]);
        REQUIRE(v.w == nums[3]);
    }

    template<class T, int M> void Run(Random & r, linalg::mat<T,M,2> *) const
    {
        const auto x = r.Get<linalg::vec<T,M>>(), y = r.Get<linalg::vec<T,M>>();
        const linalg::mat<T,M,2> m { x, y };
        REQUIRE(m.x == x);
        REQUIRE(m.y == y);
    }

    template<class T, int M> void Run(Random & r, linalg::mat<T,M,3> *) const
    {
        const auto x = r.Get<linalg::vec<T,M>>(), y = r.Get<linalg::vec<T,M>>(), z = r.Get<linalg::vec<T,M>>();
        const linalg::mat<T,M,3> m { x, y, z };
        REQUIRE(m.x == x);
        REQUIRE(m.y == y);
        REQUIRE(m.z == z);
    }

    template<class T, int M> void Run(Random & r, linalg::mat<T,M,4> *) const
    {
        const auto x = r.Get<linalg::vec<T,M>>(), y = r.Get<linalg::vec<T,M>>(), z = r.Get<linalg::vec<T,M>>(), w = r.Get<linalg::vec<T,M>>();
        const linalg::mat<T,M,4> m { x, y, z, w };
        REQUIRE(m.x == x);
        REQUIRE(m.y == y);
        REQUIRE(m.z == z);
        REQUIRE(m.w == w);
    }
};

TEST_CASE( "Construct with components", "[constructors]" )
{
    RunTest(ComponentConstructorTest(), 10);
}
