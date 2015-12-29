// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "linalg-tests.h"
#include <climits>

std::uniform_int_distribution<int> int_dist(INT_MIN, INT_MAX);
std::uniform_real_distribution<float> float_dist(-1000000.0f, +1000000.0f);
std::uniform_real_distribution<double> double_dist(-1000000.0, +1000000.0);

template<class T> auto generate(std::mt19937 & engine, T & dist, bool isDivisor) -> decltype(dist(engine))
{
    while(true)
    {
        auto r = dist(engine);
        if(std::abs(r) != 0) return r;
    }
}

int Random::Generate(int *, bool isDivisor) { return generate(engine, int_dist, isDivisor); }
float Random::Generate(float *, bool isDivisor) { return generate(engine, float_dist, isDivisor); }
double Random::Generate(double *, bool isDivisor) { return generate(engine, double_dist, isDivisor); }

#define CATCH_CONFIG_MAIN
#include "catch.hpp"