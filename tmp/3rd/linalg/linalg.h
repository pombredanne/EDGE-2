// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef LINALG_H_INCLUDE_GUARD
#define LINALG_H_INCLUDE_GUARD

#include <cstdint>
#include <cmath>

namespace linalg
{
    /////////////////////
    // Data structures //
    /////////////////////
    
    // A vector with exactly M elements, each of which is an instance of type T. Can also be thought of as an M x 1 matrix.
    template<class T, int M> struct vec;
    template<class T> struct vec<T,2>
    {
        T                           x, y;

                                    vec()                               : x(), y() {}
                                    vec(T x, T y)                       : x(x), y(y) {}
        explicit                    vec(T s)                            : x(s), y(s) {}
        template<class U> explicit  vec(const vec<U,2> & r)             : x(T(r.x)), y(T(r.y)) {}

        const T &                   operator [] (int i) const           { return (&x)[i]; }
        T &                         operator [] (int i)                 { return (&x)[i]; }
    };
    template<class T> struct vec<T,3>
    {
        T                           x, y, z;

                                    vec()                               : x(), y(), z() {}
                                    vec(T x, T y, T z)                  : x(x), y(y), z(z) {}
                                    vec(vec<T,2> xy, T z)               : x(xy.x), y(xy.y), z(z) {}
        explicit                    vec(T s)                            : x(s), y(s), z(s) {}
        template<class U> explicit  vec(const vec<U,3> & r)             : x(T(r.x)), y(T(r.y)), z(T(r.z)) {}

        const vec<T,2> &            xy() const                          { return reinterpret_cast<const vec<T,2> &>(x); }
        const T &                   operator [] (int i) const           { return (&x)[i]; }
        T &                         operator [] (int i)                 { return (&x)[i]; }
    };
    template<class T> struct vec<T,4>
    {
        T                           x, y, z, w;

                                    vec()                               : x(), y(), z(), w() {}
                                    vec(T x, T y, T z, T w)             : x(x), y(y), z(z), w(w) {}
                                    vec(vec<T,3> xyz, T w)              : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
        explicit                    vec(T s)                            : x(s), y(s), z(s), w(s) {}
        template<class U> explicit  vec(const vec<U,4> & r)             : x(T(r.x)), y(T(r.y)), z(T(r.z)), w(T(r.w)) {}

        const vec<T,3> &            xyz() const                          { return reinterpret_cast<const vec<T,3> &>(x); }
        const T &                   operator [] (int i) const           { return (&x)[i]; }
        T &                         operator [] (int i)                 { return (&x)[i]; }
    };

    // A matrix with exactly M rows and N columns, where each element is an instance of type T
    template<class T, int M, int N> struct mat;
    template<class T, int M> struct mat<T,M,2>
    {
        typedef                     vec<T,M> C;
        C                           x, y;

                                    mat()                               {}
                                    mat(C x, C y)                       : x(x), y(y) {}
        explicit                    mat(T s)                            : x(s), y(s) {}
        template<class U> explicit  mat(const mat<U,M,2> & r)           : x(C(r.x)), y(C(r.y)) {}

        vec<T,2>                    row(int i) const                    { return {x[i], y[i]}; } 
        const C &                   operator [] (int j) const           { return (&x)[j]; }
        const T &                   operator () (int i, int j) const    { return (*this)[j][i]; }
        C &                         operator [] (int j)                 { return (&x)[j]; }
        T &                         operator () (int i, int j)          { return (*this)[j][i]; }
    };

    // Specialization for an M x 3 matrix, with exactly three columns, each of which is an M-element vector
    template<class T, int M> struct mat<T,M,3>
    {
        typedef                     vec<T,M> C;
        C                           x, y, z;

                                    mat()                               {}
                                    mat(C x, C y, C z)                  : x(x), y(y), z(z) {}
                                    mat(mat<T,M,2> xy, C z)             : x(xy.x), y(xy.y), z(z) {}
        explicit                    mat(T s)                            : x(s), y(s), z(s) {}
        template<class U> explicit  mat(const mat<U,M,3> & r)           : x(C(r.x)), y(C(r.y)), z(C(r.z)) {}

        const mat<T,M,2> &          xy() const                          { return reinterpret_cast<const mat<T,M,2> &>(x); }
        vec<T,3>                    row(int i) const                    { return {x[i], y[i], z[i]}; }
        const C &                   operator [] (int j) const           { return (&x)[j]; }
        const T &                   operator () (int i, int j) const    { return (*this)[j][i]; }
        C &                         operator [] (int j)                 { return (&x)[j]; }
        T &                         operator () (int i, int j)          { return (*this)[j][i]; }
    };

    // Specialization for an M x 4 matrix, with exactly four columns, each of which is an M-element vector
    template<class T, int M> struct mat<T,M,4>
    {
        typedef                     vec<T,M> C;
        C                           x, y, z, w;

                                    mat()                               {}
                                    mat(C x, C y, C z, C w)             : x(x), y(y), z(z), w(w) {}
                                    mat(mat<T,M,3> xyz, C w)            : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
        explicit                    mat(T s)                            : x(s), y(s), z(s), w(s) {}
        template<class U> explicit  mat(const mat<U,M,4> & r)           : x(C(r.x)), y(C(r.y)), z(C(r.z)), w(C(r.w)) {}

        const mat<T,M,3> &          xyz() const                         { return reinterpret_cast<const mat<T,M,3> &>(x); }
        vec<T,4>                    row(int i) const                    { return {x[i], y[i], z[i], w[i]}; }
        const C &                   operator [] (int j) const           { return (&x)[j]; }
        const T &                   operator () (int i, int j) const    { return (*this)[j][i]; }
        C &                         operator [] (int j)                 { return (&x)[j]; }
        T &                         operator () (int i, int j)          { return (*this)[j][i]; }
    };
   
    ////////////////////////
    // Functional helpers //
    ////////////////////////

    // Form a vector or matrix by applying function f to each element of vector or matrix v
    template<class T, class F> T map(const T & v, F f) { return zip(v, v, [f](auto a, auto) { return f(a); }); }

    // Form a scalar by applying function f to adjacent components of vector or matrix v
    template<class T, class F> T reduce(const vec<T,2> & v, F f) { return f(v.x, v.y); }
    template<class T, class F> T reduce(const vec<T,3> & v, F f) { return f(f(v.x, v.y), v.z); }
    template<class T, class F> T reduce(const vec<T,4> & v, F f) { return f(f(f(v.x, v.y), v.z), v.w); }
    template<class T, int M, class F> T reduce(const mat<T,M,2> & m, F f) { return f(reduce(m.x, f), reduce(m.y, f)); }
    template<class T, int M, class F> T reduce(const mat<T,M,3> & m, F f) { return f(f(reduce(m.x, f), reduce(m.y, f)), reduce(m.z, f)); }
    template<class T, int M, class F> T reduce(const mat<T,M,4> & m, F f) { return f(f(f(reduce(m.x, f), reduce(m.y, f)), reduce(m.z, f)), reduce(m.w, f)); }

    // Form a vector or matrix by applying function f to corresponding pairs of components from l and r
    template<class T, class F> auto zip(const vec<T,2> & l, const vec<T,2> & r, F f) -> vec<decltype(f(T(),T())),2> { return {f(l.x,r.x), f(l.y,r.y)}; }
    template<class T, class F> auto zip(const vec<T,3> & l, const vec<T,3> & r, F f) -> vec<decltype(f(T(),T())),3> { return {f(l.x,r.x), f(l.y,r.y), f(l.z,r.z)}; }
    template<class T, class F> auto zip(const vec<T,4> & l, const vec<T,4> & r, F f) -> vec<decltype(f(T(),T())),4> { return {f(l.x,r.x), f(l.y,r.y), f(l.z,r.z), f(l.w,r.w)}; }
    template<class T, class F> auto zip(const vec<T,2> & l, const T & r, F f) -> vec<decltype(f(T(),r)),2> { return {f(l.x,r), f(l.y,r)}; }
    template<class T, class F> auto zip(const vec<T,3> & l, const T & r, F f) -> vec<decltype(f(T(),r)),3> { return {f(l.x,r), f(l.y,r), f(l.z,r)}; }
    template<class T, class F> auto zip(const vec<T,4> & l, const T & r, F f) -> vec<decltype(f(T(),r)),4> { return {f(l.x,r), f(l.y,r), f(l.z,r), f(l.w,r)}; }
    template<class T, class F> auto zip(const T & l, const vec<T,2> & r, F f) -> vec<decltype(f(l,T())),2> { return {f(l,r.x), f(l,r.y)}; }
    template<class T, class F> auto zip(const T & l, const vec<T,3> & r, F f) -> vec<decltype(f(l,T())),3> { return {f(l,r.x), f(l,r.y), f(l,r.z)}; }
    template<class T, class F> auto zip(const T & l, const vec<T,4> & r, F f) -> vec<decltype(f(l,T())),4> { return {f(l,r.x), f(l,r.y), f(l,r.z), f(l,r.w)}; }
    template<class T, int M, class F> auto zip(const mat<T,M,2> & l, const mat<T,M,2> & r, F f) -> mat<decltype(f(T(),T())),M,2> { return {zip(l.x,r.x,f), zip(l.y,r.y,f)}; }
    template<class T, int M, class F> auto zip(const mat<T,M,3> & l, const mat<T,M,3> & r, F f) -> mat<decltype(f(T(),T())),M,3> { return {zip(l.x,r.x,f), zip(l.y,r.y,f), zip(l.z,r.z,f)}; }
    template<class T, int M, class F> auto zip(const mat<T,M,4> & l, const mat<T,M,4> & r, F f) -> mat<decltype(f(T(),T())),M,4> { return {zip(l.x,r.x,f), zip(l.y,r.y,f), zip(l.z,r.z,f), zip(l.w,r.w,f)}; }
    template<class T, int M, class F> auto zip(const mat<T,M,2> & l, const T & r, F f) -> mat<decltype(f(T(),r)),M,2> { return {zip(l.x,r,f), zip(l.y,r,f)}; }
    template<class T, int M, class F> auto zip(const mat<T,M,3> & l, const T & r, F f) -> mat<decltype(f(T(),r)),M,3> { return {zip(l.x,r,f), zip(l.y,r,f), zip(l.z,r,f)}; }
    template<class T, int M, class F> auto zip(const mat<T,M,4> & l, const T & r, F f) -> mat<decltype(f(T(),r)),M,4> { return {zip(l.x,r,f), zip(l.y,r,f), zip(l.z,r,f), zip(l.w,r,f)}; }
    template<class T, int M, class F> auto zip(const T & l, const mat<T,M,2> & r, F f) -> mat<decltype(f(l,T())),M,2> { return {zip(l,r.x,f), zip(l,r.y,f)}; }
    template<class T, int M, class F> auto zip(const T & l, const mat<T,M,3> & r, F f) -> mat<decltype(f(l,T())),M,3> { return {zip(l,r.x,f), zip(l,r.y,f), zip(l,r.z,f)}; }
    template<class T, int M, class F> auto zip(const T & l, const mat<T,M,4> & r, F f) -> mat<decltype(f(l,T())),M,4> { return {zip(l,r.x,f), zip(l,r.y,f), zip(l,r.z,f), zip(l,r.w,f)}; }

    ////////////////////////
    // Operator overloads //
    ////////////////////////

    // Unary operators return a vector or matrix of the original type, by applying the operator to each element
    template<class T> T operator + (const T & v) { return map(v, [](auto a) { return +a; }); }
    template<class T> T operator - (const T & v) { return map(v, [](auto a) { return -a; }); }
    template<class T> T operator ! (const T & v) { return map(v, [](auto a) { return !a; }); }
    template<class T> T operator ~ (const T & v) { return map(v, [](auto a) { return ~a; }); }

    // Equality operators return a bool indicating exact equality or inequality of two values
    template<class T, int M> bool operator == (const vec<T,M> & l, const vec<T,M> & r) { return all(zip(l, r, [](T a, T b) { return a == b; })); }
    template<class T, int M> bool operator != (const vec<T,M> & l, const vec<T,M> & r) { return any(zip(l, r, [](T a, T b) { return a != b; })); }
    template<class T, int M, int N> bool operator == (const mat<T,M,N> & l, const mat<T,M,N> & r) { return all(zip(l, r, [](T a, T b) { return a == b; })); }
    template<class T, int M, int N> bool operator != (const mat<T,M,N> & l, const mat<T,M,N> & r) { return any(zip(l, r, [](T a, T b) { return a != b; })); }

    // Ordering operators return a vector or matrix of bools, by comparing componentwise pairs
    template<class L, class R> auto operator <  (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a <  b; }); }
    template<class L, class R> auto operator <= (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a <= b; }); }
    template<class L, class R> auto operator >  (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a >  b; }); }
    template<class L, class R> auto operator >= (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a >= b; }); }

    // Arithmetic and bitwise binary operators return a vector or matrix of the original type, by applying the operator to componentwise pairs
    template<class L, class R> auto operator + (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a+b; }); }
    template<class L, class R> auto operator - (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a-b; }); }
    template<class L, class R> auto operator * (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a*b; }); }
    template<class L, class R> auto operator / (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a/b; }); }
    template<class L, class R> auto operator % (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a%b; }); }
    template<class L, class R> auto operator | (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a|b; }); }
    template<class L, class R> auto operator & (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a&b; }); }
    template<class L, class R> auto operator ^ (const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a^b; }); }

    // Assignment operators are defined trivially
    template<class L, class R> L & operator += (L & l, const R & r) { return l=l+r; }
    template<class L, class R> L & operator -= (L & l, const R & r) { return l=l-r; }
    template<class L, class R> L & operator *= (L & l, const R & r) { return l=l*r; }
    template<class L, class R> L & operator /= (L & l, const R & r) { return l=l/r; }
    template<class L, class R> L & operator %= (L & l, const R & r) { return l=l%r; }
    template<class L, class R> L & operator |= (L & l, const R & r) { return l=l|r; }
    template<class L, class R> L & operator &= (L & l, const R & r) { return l=l&r; }
    template<class L, class R> L & operator ^= (L & l, const R & r) { return l=l^r; }

    /////////////////////////////////
    // General aggregate functions //
    /////////////////////////////////

    // Return true if any/all elements of vector or matrix v would evaluate to true in a boolean context
    template<class T> bool any(const T & v) { return reduce(v, [](bool a, bool b) { return a || b; }); }
    template<class T> bool all(const T & v) { return reduce(v, [](bool a, bool b) { return a && b; }); }

    // Compute the sum of all elements in vector or matrix v
    template<class T> auto sum(const T & v) { return reduce(v, [](auto a, auto b) { return a + b; }); }
    template<class T> auto product(const T & v) { return reduce(v, [](auto a, auto b) { return a * b; }); }

    // Form a vector or matrix by applying std::abs/ceil/floor to each component of vector or matrix v
    template<class T> T abs(const T & v) { return map(v, [](auto a) { return std::abs(a); }); }
    template<class T> T ceil(const T & v) { return map(v, [](auto a) { return std::ceil(a); }); }
    template<class T> T floor(const T & v) { return map(v, [](auto a) { return std::floor(a); }); }

    // Form a vector or matrix by taking the componentwise max/min of two vectors or matrices
    template<class L, class R> auto max(const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a > b ? a : b; }); }
    template<class L, class R> auto min(const L & l, const R & r) { return zip(l, r, [](auto a, auto b) { return a < b ? a : b; }); }

    // Return a when t = 0, b when t = 1, and linearly interpolate between a and b when t is in the interval (0,1)
    template<class V, class T> V lerp(const V & a, const V & b, T t) { return a*(1-t) + b*t; }

    //////////////////////////////
    // Vector algebra functions //
    //////////////////////////////

    // Compute the dot/cross/outer product of two vectors
    template<class T, int N> T dot(const vec<T,N> & l, const vec<T,N> & r) { return sum(l*r); }
    template<class T> vec<T,3> cross(const vec<T,3> & l, const vec<T,3> & r) { return {l.y*r.z-l.z*r.y, l.z*r.x-l.x*r.z, l.x*r.y-l.y*r.x}; }

    // Compute the length/square length of a vector
    template<class T, int N> T length(const vec<T,N> & v) { return std::sqrt(length_sq(v)); }    
    template<class T, int N> T length_sq(const vec<T,N> & v) { return dot(v,v); }

    // Compute the distance/square distance between two points, expressed as vectors
    template<class T, int N> T distance(const vec<T,N> & l, const vec<T,N> & r) { return length(r-l); }
    template<class T, int N> T distance_sq(const vec<T,N> & l, const vec<T,N> & r) { return length_sq(r-l); }

    // Compute a normalized vector with the same direction as the original vector
    template<class T, int N> vec<T,N> normalize(const vec<T,N> & v) { return v/length(v); }

    //////////////////////////////////
    // Quaternion algebra functions //
    //////////////////////////////////

    // General quaternion algebra for quaternions of the form xi + yj + zk + w
    template<class T> vec<T,4> quat_conjugate(const vec<T,4> & q) { return {-q.x,-q.y,-q.z,q.w}; }
    template<class T> vec<T,4> quat_inverse(const vec<T,4> & q) { return quat_conjugate(q)/length_sq(q); }
    template<class T> vec<T,4> quat_product(const vec<T,4> & a, const vec<T,4> & b) { return {a.x*b.w+a.w*b.x+a.y*b.z-a.z*b.y, a.y*b.w+a.w*b.y+a.z*b.x-a.x*b.z, a.z*b.w+a.w*b.z+a.x*b.y-a.y*b.x, a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z}; }

    // Compute basis vectors of spatial rotation quaternions
    template<class T> vec<T,3> quat_basis_x(const vec<T,4> & q) { return {q.w*q.w+q.x*q.x-q.y*q.y-q.z*q.z, (q.x*q.y+q.z*q.w)*2, (q.z*q.x-q.y*q.w)*2}; }
    template<class T> vec<T,3> quat_basis_y(const vec<T,4> & q) { return {(q.x*q.y-q.z*q.w)*2, q.w*q.w-q.x*q.x+q.y*q.y-q.z*q.z, (q.y*q.z+q.x*q.w)*2}; }
    template<class T> vec<T,3> quat_basis_z(const vec<T,4> & q) { return {(q.z*q.x+q.y*q.w)*2, (q.y*q.z-q.x*q.w)*2, q.w*q.w-q.x*q.x-q.y*q.y+q.z*q.z}; }

    // Factory functions to produce spatial rotation quaternions
    template<class T> vec<T,4> make_rotation_quaternion(const vec<T,3> & axis, T angle) { return {axis*std::sin(angle/2), std::cos(angle/2)}; }
    template<class T> vec<T,4> make_rotation_quaternion(const vec<T,3> & from, const vec<T,3> & to) { return make_rotation_quaternion(normalize(cross(from, to)), std::acos(dot(from, to))); }

    // Rotate a vector quantity by an angle or quaternion
    template<class T> vec<T,2> rotate_vector(T angle, const vec<T,2> & vector) { return mul(make_rotation_matrix(angle), vector); }
    template<class T> vec<T,3> rotate_vector(const vec<T,4> & quaternion, const vec<T,3> & vector) { return mul(make_rotation_matrix(quaternion), vector); }

    //////////////////////////////
    // Matrix algebra functions //
    //////////////////////////////

    // Compute the adjugate of a square matrix (equivalent to the transpose of the cofactor matrix)
    template<class T> mat<T,2,2> adjugate(const mat<T,2,2> & a) { return {{a.y.y, -a.x.y}, {-a.y.x, a.x.x}}; }
    template<class T> mat<T,3,3> adjugate(const mat<T,3,3> & a) { return {{a.y.y*a.z.z - a.z.y*a.y.z, a.z.y*a.x.z - a.x.y*a.z.z, a.x.y*a.y.z - a.y.y*a.x.z},
                                                                          {a.y.z*a.z.x - a.z.z*a.y.x, a.z.z*a.x.x - a.x.z*a.z.x, a.x.z*a.y.x - a.y.z*a.x.x},
                                                                          {a.y.x*a.z.y - a.z.x*a.y.y, a.z.x*a.x.y - a.x.x*a.z.y, a.x.x*a.y.y - a.y.x*a.x.y}}; }
    template<class T> mat<T,4,4> adjugate(const mat<T,4,4> & a) { return {{a.y.y*a.z.z*a.w.w + a.w.y*a.y.z*a.z.w + a.z.y*a.w.z*a.y.w - a.y.y*a.w.z*a.z.w - a.z.y*a.y.z*a.w.w - a.w.y*a.z.z*a.y.w,
                                                                           a.x.y*a.w.z*a.z.w + a.z.y*a.x.z*a.w.w + a.w.y*a.z.z*a.x.w - a.w.y*a.x.z*a.z.w - a.z.y*a.w.z*a.x.w - a.x.y*a.z.z*a.w.w,
                                                                           a.x.y*a.y.z*a.w.w + a.w.y*a.x.z*a.y.w + a.y.y*a.w.z*a.x.w - a.x.y*a.w.z*a.y.w - a.y.y*a.x.z*a.w.w - a.w.y*a.y.z*a.x.w,
                                                                           a.x.y*a.z.z*a.y.w + a.y.y*a.x.z*a.z.w + a.z.y*a.y.z*a.x.w - a.x.y*a.y.z*a.z.w - a.z.y*a.x.z*a.y.w - a.y.y*a.z.z*a.x.w},
                                                                          {a.y.z*a.w.w*a.z.x + a.z.z*a.y.w*a.w.x + a.w.z*a.z.w*a.y.x - a.y.z*a.z.w*a.w.x - a.w.z*a.y.w*a.z.x - a.z.z*a.w.w*a.y.x,
                                                                           a.x.z*a.z.w*a.w.x + a.w.z*a.x.w*a.z.x + a.z.z*a.w.w*a.x.x - a.x.z*a.w.w*a.z.x - a.z.z*a.x.w*a.w.x - a.w.z*a.z.w*a.x.x,
                                                                           a.x.z*a.w.w*a.y.x + a.y.z*a.x.w*a.w.x + a.w.z*a.y.w*a.x.x - a.x.z*a.y.w*a.w.x - a.w.z*a.x.w*a.y.x - a.y.z*a.w.w*a.x.x,
                                                                           a.x.z*a.y.w*a.z.x + a.z.z*a.x.w*a.y.x + a.y.z*a.z.w*a.x.x - a.x.z*a.z.w*a.y.x - a.y.z*a.x.w*a.z.x - a.z.z*a.y.w*a.x.x},
                                                                          {a.y.w*a.z.x*a.w.y + a.w.w*a.y.x*a.z.y + a.z.w*a.w.x*a.y.y - a.y.w*a.w.x*a.z.y - a.z.w*a.y.x*a.w.y - a.w.w*a.z.x*a.y.y,
                                                                           a.x.w*a.w.x*a.z.y + a.z.w*a.x.x*a.w.y + a.w.w*a.z.x*a.x.y - a.x.w*a.z.x*a.w.y - a.w.w*a.x.x*a.z.y - a.z.w*a.w.x*a.x.y,
                                                                           a.x.w*a.y.x*a.w.y + a.w.w*a.x.x*a.y.y + a.y.w*a.w.x*a.x.y - a.x.w*a.w.x*a.y.y - a.y.w*a.x.x*a.w.y - a.w.w*a.y.x*a.x.y,
                                                                           a.x.w*a.z.x*a.y.y + a.y.w*a.x.x*a.z.y + a.z.w*a.y.x*a.x.y - a.x.w*a.y.x*a.z.y - a.z.w*a.x.x*a.y.y - a.y.w*a.z.x*a.x.y},
                                                                          {a.y.x*a.w.y*a.z.z + a.z.x*a.y.y*a.w.z + a.w.x*a.z.y*a.y.z - a.y.x*a.z.y*a.w.z - a.w.x*a.y.y*a.z.z - a.z.x*a.w.y*a.y.z,
                                                                           a.x.x*a.z.y*a.w.z + a.w.x*a.x.y*a.z.z + a.z.x*a.w.y*a.x.z - a.x.x*a.w.y*a.z.z - a.z.x*a.x.y*a.w.z - a.w.x*a.z.y*a.x.z,
                                                                           a.x.x*a.w.y*a.y.z + a.y.x*a.x.y*a.w.z + a.w.x*a.y.y*a.x.z - a.x.x*a.y.y*a.w.z - a.w.x*a.x.y*a.y.z - a.y.x*a.w.y*a.x.z,
                                                                           a.x.x*a.y.y*a.z.z + a.z.x*a.x.y*a.y.z + a.y.x*a.z.y*a.x.z - a.x.x*a.z.y*a.y.z - a.y.x*a.x.y*a.z.z - a.z.x*a.y.y*a.x.z}}; }

    // Compute the determinant of a square matrix
    template<class T> T determinant(const mat<T,2,2> & a) { return a.x.x*a.y.y - a.x.y*a.y.x; }
    template<class T> T determinant(const mat<T,3,3> & a) { return a.x.x*(a.y.y*a.z.z - a.z.y*a.y.z) + a.x.y*(a.y.z*a.z.x - a.z.z*a.y.x) + a.x.z*(a.y.x*a.z.y - a.z.x*a.y.y); }
    template<class T> T determinant(const mat<T,4,4> & a) { return a.x.x*(a.y.y*a.z.z*a.w.w + a.w.y*a.y.z*a.z.w + a.z.y*a.w.z*a.y.w - a.y.y*a.w.z*a.z.w - a.z.y*a.y.z*a.w.w - a.w.y*a.z.z*a.y.w)
                                                                 + a.x.y*(a.y.z*a.w.w*a.z.x + a.z.z*a.y.w*a.w.x + a.w.z*a.z.w*a.y.x - a.y.z*a.z.w*a.w.x - a.w.z*a.y.w*a.z.x - a.z.z*a.w.w*a.y.x)
                                                                 + a.x.z*(a.y.w*a.z.x*a.w.y + a.w.w*a.y.x*a.z.y + a.z.w*a.w.x*a.y.y - a.y.w*a.w.x*a.z.y - a.z.w*a.y.x*a.w.y - a.w.w*a.z.x*a.y.y)
                                                                 + a.x.w*(a.y.x*a.w.y*a.z.z + a.z.x*a.y.y*a.w.z + a.w.x*a.z.y*a.y.z - a.y.x*a.z.y*a.w.z - a.w.x*a.y.y*a.z.z - a.z.x*a.w.y*a.y.z); }

    // Compute the inverse of a square matrix
    template<class T, int N> mat<T,N,N> inverse(const mat<T,N,N> & a) { return adjugate(a)/determinant(a); }

    // Compute the product of a matrix postmultiplied by a column vector
    template<class T, int M> vec<T,M> mul(const mat<T,M,2> & a, const vec<T,2> & b) { return a.x*b.x + a.y*b.y; }
    template<class T, int M> vec<T,M> mul(const mat<T,M,3> & a, const vec<T,3> & b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
    template<class T, int M> vec<T,M> mul(const mat<T,M,4> & a, const vec<T,4> & b) { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }

    // Compute the product of two matrices
    template<class T, int M, int N> mat<T,M,2> mul(const mat<T,M,N> & a, const mat<T,N,2> & b) { return {mul(a,b.x), mul(a,b.y)}; }
    template<class T, int M, int N> mat<T,M,3> mul(const mat<T,M,N> & a, const mat<T,N,3> & b) { return {mul(a,b.x), mul(a,b.y), mul(a,b.z)}; }
    template<class T, int M, int N> mat<T,M,4> mul(const mat<T,M,N> & a, const mat<T,N,4> & b) { return {mul(a,b.x), mul(a,b.y), mul(a,b.z), mul(a,b.w)}; }

    // Compute the tranpose of a matrix
    template<class T, int M> mat<T,M,2> transpose(const mat<T,2,M> & m) { return {m.row(0), m.row(1)}; }
    template<class T, int M> mat<T,M,3> transpose(const mat<T,3,M> & m) { return {m.row(0), m.row(1), m.row(2)}; }
    template<class T, int M> mat<T,M,4> transpose(const mat<T,4,M> & m) { return {m.row(0), m.row(1), m.row(2), m.row(3)}; }
  
    // Compute the outer product of two vectors (the product of an Mx1 matrix with a 1xN matrix)
    template<class T, int M> mat<T,M,2> outer_product(const vec<T,M> & a, const vec<T,2> & b) { return {a*b.x, a*b.y}; }
    template<class T, int M> mat<T,M,3> outer_product(const vec<T,M> & a, const vec<T,3> & b) { return {a*b.x, a*b.y, a*b.z}; }
    template<class T, int M> mat<T,M,4> outer_product(const vec<T,M> & a, const vec<T,4> & b) { return {a*b.x, a*b.y, a*b.z, a*b.w}; }

    // Factory functions to produce linear transformation matrices (which preserve addition and scalar multiplication)
    template<class T> mat<T,2,2>        make_identity_matrix(const mat<T,2,2> *)                         { return {{1,0},{0,1}}; }
    template<class T> mat<T,3,3>        make_identity_matrix(const mat<T,3,3> *)                         { return {{1,0,0},{0,1,0},{0,0,1}}; }
    template<class T> mat<T,4,4>        make_identity_matrix(const mat<T,4,4> *)                         { return {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}; }
    template<class T> T                 make_identity_matrix()                                           { return make_identity_matrix((T*)nullptr); }
    template<class T> mat<T,2,2>        make_rotation_matrix(T angle)                                    { return {{std::cos(angle), std::sin(angle)}, {-std::sin(angle), std::cos(angle)}}; }
    template<class T> mat<T,3,3>        make_rotation_matrix(const vec<T,4> & quaternion)                { return {quat_basis_x(quaternion), quat_basis_y(quaternion), quat_basis_z(quaternion)}; }
    template<class T> mat<T,3,3>        make_rotation_matrix(const vec<T,3> & axis, T angle)             { return make_rotation_matrix(make_rotation_quaternion(axis, angle)); }
    template<class T> mat<T,3,3>        make_rotation_matrix(const vec<T,3> & from, const vec<T,3> & to) { return make_rotation_matrix(make_rotation_quaternion(from, to)); }
    template<class T, int M> mat<T,M,M> make_scaling_matrix(const vec<T,M> & axis, float scale)          { return make_identity_matrix<mat<T,M,M>>() + outer_product(axis, axis) * (scale-1); }

    /////////////////////////////////////
    // Affine matrix algebra functions //
    /////////////////////////////////////

    // Compute the affine transformations of vectors, coordinates, and other affine transformation matrices
    template<class T, int M> vec<T,M> affine_transform_vector(const mat<T,M,M+1> & affine, const vec<T,M> & vector) { return mul(affine, vec<T,M+1>(vector,0)); }
    template<class T, int M> vec<T,M> affine_transform_coord(const mat<T,M,M+1> & affine, const vec<T,M> & coord) { return mul(affine, vec<T,M+1>(coord,1)); }
    template<class T, int M> mat<T,M,M+1> affine_product(const mat<T,M,M+1> & a, const mat<T,M,M+1> & b) { return mul(a, make_homogeneous(b)); }
    template<class T> mat<T,2,3> affine_inverse(const mat<T,2,3> & a) { auto r = inverse(a.xy()); return {r, -mul(r,a.z)}; }
    template<class T> mat<T,3,4> affine_inverse(const mat<T,3,4> & a) { auto r = inverse(a.xyz()); return {r, -mul(r,a.w)}; }

    // Factory functions to produce affine transformation matrices (which preserve points, lines, and planes)
    template<class T> mat<T,2,3>  make_affine(const mat<T,2,2> & linear)                                  { return {{linear.x}, {linear.y}, {0,0}}; }
    template<class T> mat<T,3,4>  make_affine(const mat<T,3,3> & linear)                                  { return {{linear.x}, {linear.y}, {linear.z}, {0,0,0}}; }
    template<class T> mat<T,2,3>  make_affine_translation_matrix(const vec<T,2> & translation)            { return {{1,0}, {0,1}, {translation}}; }
    template<class T> mat<T,3,4>  make_affine_translation_matrix(const vec<T,3> & translation)            { return {{1,0,0}, {0,1,0}, {0,0,1}, {translation}}; }
    template<class T> auto        make_affine_rotation_matrix(T angle)                                    { return make_affine(make_rotation_matrix(angle)); }
    template<class T> auto        make_affine_rotation_matrix(const vec<T,4> & quaternion)                { return make_affine(make_rotation_matrix(quaternion)); }
    template<class T> auto        make_affine_rotation_matrix(const vec<T,3> & axis, T angle)             { return make_affine(make_rotation_matrix(axis, angle)); }
    template<class T> auto        make_affine_rotation_matrix(const vec<T,3> & from, const vec<T,3> & to) { return make_affine(make_rotation_matrix(from, to)); }
    template<class T, int M> auto make_affine_scaling_matrix(const vec<T,M> & axis, float scale)          { return make_affine(make_scaling_matrix(axis, scale)); }

    //////////////////////////////////////////
    // Homogeneous matrix algebra functions //
    //////////////////////////////////////////

    // Compute the homogeneous transformations of vectors and coordinates
    template<class T> vec<T,2> homogeneous_projection(const vec<T,3> & coord) { return {coord.x/coord.z, coord.y/coord.z}; }
    template<class T> vec<T,3> homogeneous_projection(const vec<T,4> & coord) { return {coord.x/coord.w, coord.y/coord.w, coord.z/coord.w}; }
    template<class T, int M> vec<T,M> homogeneous_transform_vector(const mat<T,M+1,M+1> & homogeneous, const vec<T,M> & vector) { return homogeneous_projection(mul(homogeneous, vec<T,M+1>(vector,0))); }
    template<class T, int M> vec<T,M> homogeneous_transform_coord(const mat<T,M+1,M+1> & homogeneous, const vec<T,M> & coord) { return homogeneous_projection(mul(homogeneous, vec<T,M+1>(coord,1))); }

    // Factory functions to produce homogeneous transformation matrices (which allow both affine and projective transformations)
    template<class T> mat<T,3,3>  make_homogeneous(const mat<T,2,2> & linear)                                  { return {{linear.x,0}, {linear.y,0}, {0,0,1}}; }
    template<class T> mat<T,3,3>  make_homogeneous(const mat<T,2,3> & affine)                                  { return {{affine.x,0}, {affine.y,0}, {affine.z,1}}; }
    template<class T> mat<T,4,4>  make_homogeneous(const mat<T,3,3> & linear)                                  { return {{linear.x,0}, {linear.y,0}, {linear.z,0}, {0,0,0,1}}; }
    template<class T> mat<T,4,4>  make_homogeneous(const mat<T,3,4> & affine)                                  { return {{affine.x,0}, {affine.y,0}, {affine.z,0}, {affine.w,1}}; }
    template<class T, int M> auto make_homogeneous_translation_matrix(const vec<T,M> & translation)            { return make_homogeneous(make_affine_translation_matrix(translation)); }
    template<class T> auto        make_homogeneous_rotation_matrix(T angle)                                    { return make_homogeneous(make_rotation_matrix(angle)); }
    template<class T> auto        make_homogeneous_rotation_matrix(const vec<T,4> & quaternion)                { return make_homogeneous(make_rotation_matrix(quaternion)); }
    template<class T> auto        make_homogeneous_rotation_matrix(const vec<T,3> & axis, T angle)             { return make_homogeneous(make_rotation_matrix(axis, angle)); }
    template<class T> auto        make_homogeneous_rotation_matrix(const vec<T,3> & from, const vec<T,3> & to) { return make_homogeneous(make_rotation_matrix(from, to)); }
    template<class T, int M> auto make_homogeneous_scaling_matrix(const vec<T,M> & axis, float scale)          { return make_homogeneous(make_scaling_matrix(axis, scale)); }
    template<class T> mat<T,4,4>  make_homogeneous_frustum_matrix(T l, T r, T b, T t, T n, T f)                { return {{2*n/(r-l),0,0,0}, {0,2*n/(t-b),0,0}, {(r+l)/(r-l),(t+b)/(t-b),-(f+n)/(f-n),-1}, {0,0,-2*f*n/(f-n),0}}; }
    template<class T> mat<T,4,4>  make_homogeneous_perspective_matrix(T fovy, T aspect, T n, T f)              { T y = n*std::tan(fovy/2), x=y*aspect; return make_homogeneous_frustum_matrix(-x,x,-y,y,n,f); }

    /////////////////////////
    // Convenience aliases //
    /////////////////////////

    // These aliases resemble those used in HLSL, Cg, OpenCL, etc.
    namespace aliases
    {
        typedef vec<int32_t,2> int2; typedef vec<uint32_t,2> uint2; typedef vec<bool,2> bool2; typedef vec<float,2> float2; typedef vec<double,2> double2;
        typedef vec<int32_t,3> int3; typedef vec<uint32_t,3> uint3; typedef vec<bool,3> bool3; typedef vec<float,3> float3; typedef vec<double,3> double3;
        typedef vec<int32_t,4> int4; typedef vec<uint32_t,4> uint4; typedef vec<bool,4> bool4; typedef vec<float,4> float4; typedef vec<double,4> double4;
        typedef mat<float,2,2> float2x2; typedef mat<float,2,3> float2x3; typedef mat<float,2,4> float2x4;
        typedef mat<float,3,2> float3x2; typedef mat<float,3,3> float3x3; typedef mat<float,3,4> float3x4;
        typedef mat<float,4,2> float4x2; typedef mat<float,4,3> float4x3; typedef mat<float,4,4> float4x4;
        typedef mat<double,2,2> double2x2; typedef mat<double,2,3> double2x3; typedef mat<double,2,4> double2x4;
        typedef mat<double,3,2> double3x2; typedef mat<double,3,3> double3x3; typedef mat<double,3,4> double3x4;
        typedef mat<double,4,2> double4x2; typedef mat<double,4,3> double4x3; typedef mat<double,4,4> double4x4;
    }
}

#endif
