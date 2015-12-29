// benchmark suite
#pragma once
#include <chrono>
static double now() {
    static auto const epoch = std::chrono::steady_clock::now(); // milli ms > micro us > nano ns
    return std::chrono::duration_cast< std::chrono::microseconds >( std::chrono::steady_clock::now() - epoch ).count() / 1000000.0;
}
template<typename T> double bench_s ( const T &t ) { auto t0 = -now(); return (t(), t0+now()); }
template<typename T> double bench_ms( const T &t ) { return bench_s( t ) * 1000.0; }
#define $bench(...) printf("[ OK ] %d %gms %s\n", __LINE__, bench_ms([&]{ __VA_ARGS__ ;}), #__VA_ARGS__ )
