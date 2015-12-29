// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "linalg.h"

#include <cstdint>
#include <vector>
#include <random>
#include <type_traits>

class Random
{
    std::mt19937 engine;

    int Generate(int *, bool isDivisor);
    float Generate(float *, bool isDivisor);
    double Generate(double *, bool isDivisor);
    template<class T, int M> linalg::vec<T,M> Generate(linalg::vec<T,M> *, bool isDivisor) { linalg::vec<T,M> v; for(int i=0; i<M; ++i) v[i] = Generate((T*)nullptr, isDivisor); return v; }
    template<class T, int M, int N> linalg::mat<T,M,N> Generate(linalg::mat<T,M,N> *, bool isDivisor) { linalg::mat<T,M,N> m; for(int j=0; j<N; ++j) m[j] = Generate((linalg::vec<T,M>*)nullptr, isDivisor); return m; }
public:
    Random() {}
    Random(const Random & r) = delete;

    template<class T> T Get() { return Generate((T*)nullptr, false); }
    template<class T> T GetDivisor() { return Generate((T*)nullptr, true); }
    template<class T> std::vector<T> GetNumbers(int count) 
    { 
        std::vector<T> results(count);
        for(int i=0; i<count; ++i) results[i] = Get<T>();
        return results;
    }
};

template<class T> void RunTest(const T & test, int count)
{
    Random r;
    for(int i=0; i<count; ++i)
    {
        test.Run(r, (linalg::vec<int,2> *)nullptr);
        test.Run(r, (linalg::vec<int,3> *)nullptr);
        test.Run(r, (linalg::vec<int,4> *)nullptr);
        test.Run(r, (linalg::vec<float,2> *)nullptr);
        test.Run(r, (linalg::vec<float,3> *)nullptr);
        test.Run(r, (linalg::vec<float,4> *)nullptr);
        test.Run(r, (linalg::vec<double,2> *)nullptr);
        test.Run(r, (linalg::vec<double,3> *)nullptr);
        test.Run(r, (linalg::vec<double,4> *)nullptr);
        test.Run(r, (linalg::mat<int,4,4> *)nullptr);
        test.Run(r, (linalg::mat<float,2,2> *)nullptr);
        test.Run(r, (linalg::mat<float,2,3> *)nullptr);
        test.Run(r, (linalg::mat<float,2,4> *)nullptr);
        test.Run(r, (linalg::mat<float,3,2> *)nullptr);
        test.Run(r, (linalg::mat<float,3,3> *)nullptr);
        test.Run(r, (linalg::mat<float,3,4> *)nullptr);
        test.Run(r, (linalg::mat<float,4,2> *)nullptr);
        test.Run(r, (linalg::mat<float,4,3> *)nullptr);
        test.Run(r, (linalg::mat<float,4,4> *)nullptr);
        test.Run(r, (linalg::mat<double,4,4> *)nullptr);
    }
}