#include <assert.h>
#include <stdint.h>
#include <vector>
#include <map>
#include <tuple>

using vec1 = std::tuple<double>;                      //double[1];
using vec2 = std::tuple<double,double>;               //double[2];
using vec3 = std::tuple<double,double,double>;        //double[3];
using vec4 = std::tuple<double,double,double,double>; //double[4];

bool low( const vec1 &t ) {
    return std::get<0>(t) < 0.5f;
}
bool high( const vec1 &t ) {
    return !low(t);
}

template<typename T>
bool off( const T &t0, const T &t1 ) {
    return low(t0) && low(t1);
}
template<typename T>
bool trigger( const T &t0, const T &t1 ) {
    return low(t0) && high(t1);
}
template<typename T>
bool on( const T &t0, const T &t1 ) {
    return high(t0) && high(t1);
}
template<typename T>
bool release( const T &t0, const T &t1 ) {
    return high(t0) && low(t1);
}

template<typename T>
using history = std::map< uint64_t, T >;

#include <algorithm>

template<typename T>
auto at( const history<T> &t, int pos ) -> decltype(t.begin()) {
    if( pos == 0 && t.size() > 0 ) return t.begin();
    if( pos == 1 && t.size() > 1 ) return ++t.begin();
    if( pos == 2 && t.size() > 2 ) return ++++t.begin();
    return t.end();
}

enum { second = 1000 /*ms*/ };

template<typename T>
bool avail( const history<T> &t, uint64_t delta = 1000 ) {
    if( t.size() >= 2 ) {
        auto it = t.begin();
        auto t0 = it->first;
        auto v0 = it->second;
        ++it;
        auto t1 = it->first;
        auto v1 = it->second;
        if( (t1 - t0) <= delta ) {
            return true;
        }
    }
    return false;
}

template<typename T>
bool off( const history<T> &t, uint64_t delta = 1000 ) {
    if( !avail(t, delta) ) {
        return false;
    }
    auto i0 = at( t, 0 );
    auto i1 = at( t, 1 );
    return low(i0->second) && low(i1->second);
}

template<typename T>
bool trigger( const history<T> &t, uint64_t delta = 1000 ) {
    if( !avail(t, delta) ) {
        return false;
    }
    auto i0 = at( t, 0 );
    auto i1 = at( t, 1 );
    return low(i0->second) && high(i1->second);
}

template<typename T>
bool on( const history<T> &t, uint64_t delta = 1000 ) {
    if( !avail(t, delta) ) {
        return false;
    }
    auto i0 = at( t, 0 );
    auto i1 = at( t, 1 );
    return high(i0->second) && high(i1->second);
}

template<typename T>
bool release( const history<T> &t, uint64_t delta = 1000 ) {
    if( !avail(t, delta) ) {
        return false;
    }
    auto i0 = at( t, 0 );
    auto i1 = at( t, 1 );
    return high(i0->second) && low(i1->second);
}











// unittest suite {
#define suite(...) if(printf("------ " __VA_ARGS__),puts(""),true)
#define test(...)  (errno=0,++tst,err+=!(ok=!!(__VA_ARGS__))),printf("[%s] %d %s (%s)\n",ok?" OK ":"FAIL",__LINE__,#__VA_ARGS__,strerror(errno))
unsigned tst=0,err=0,ok=atexit([]{ suite("summary"){ printf("[%s] %d tests = %d passed + %d errors\n",err?"FAIL":" OK ",tst,tst-err,err); }});
// }



int main() {
    suite( "pair diff 0,0" ) {
        vec1 then { 0 }, now { 0 };

        test( low(then) );
        test( low(now) );
        test( !high(then) );
        test( !high(now) );

        test( off(then, now) );
        test( !trigger(then, now) );
        test( !on(then, now) );
        test( !release(then, now) );
    }

    suite( "pair diff 0,1" ) {
        vec1 then { 0 }, now { 1 };

        test( low(then) );
        test( !low(now) );
        test( !high(then) );
        test( high(now) );

        test( !off(then, now) );
        test( trigger(then, now) );
        test( !on(then, now) );
        test( !release(then, now) );
    }

    suite( "pair diff 1,1" ) {
        vec1 then { 1 }, now { 1 };

        test( !low(then) );
        test( !low(now) );
        test( high(then) );
        test( high(now) );

        test( !off(then, now) );
        test( !trigger(then, now) );
        test( on(then, now) );
        test( !release(then, now) );
    }

    suite( "pair diff 1,0" ) {
        vec1 then { 1 }, now { 0 };

        test( !low(then) );
        test( low(now) );
        test( high(then) );
        test( !high(now) );

        test( !off(then, now) );
        test( !trigger(then, now) );
        test( !on(then, now) );
        test( release(then, now) );
    }

    suite( "history 0,0" ) {
        history<vec1> h;
        h[   0 ] = vec1 { 0 }; // at   0ms
        h[ 500 ] = vec1 { 0 }; // at 500ms

        test( off(h) ); 
        test( !trigger(h) ); 
        test( !on(h) ); 
        test( !release(h) ); 
    }

    suite( "history 0,1" ) {
        history<vec1> h;
        h[   0 ] = vec1 { 0 }; // at   0ms
        h[ 500 ] = vec1 { 1 }; // at 500ms

        test( !off(h) ); 
        test( trigger(h) ); 
        test( !on(h) ); 
        test( !release(h) ); 
    }

    suite( "history 1,1" ) {
        history<vec1> h;
        h[   0 ] = vec1 { 1 }; // at   0ms
        h[ 500 ] = vec1 { 1 }; // at 500ms

        test( !off(h) ); 
        test( !trigger(h) ); 
        test( on(h) ); 
        test( !release(h) ); 
    }

    suite( "history 1,0" ) {
        history<vec1> h;
        h[   0 ] = vec1 { 1 }; // at   0ms
        h[ 500 ] = vec1 { 0 }; // at 500ms

        test( !off(h) ); 
        test( !trigger(h) ); 
        test( !on(h) ); 
        test( release(h) ); 
    }

    // malformed

    suite( "history 0" ) {
        history<vec1> h;
        h[   0 ] = vec1 { 1 }; // at   0ms

        test( !off(h) ); 
        test( !trigger(h) ); 
        test( !on(h) ); 
        test( !release(h) ); 
    }

    suite( "history 0,1 timeout" ) {
        history<vec1> h;
        h[    0 ] = vec1 { 1 }; // at 0s
        h[ 2000 ] = vec1 { 1 }; // at 2s

        test( !off(h) ); 
        test( !trigger(h) ); 
        test( !on(h) ); 
        test( !release(h) ); 
    }

}
