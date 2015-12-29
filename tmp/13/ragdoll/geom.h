#ifndef __GEOM_H__
#define __GEOM_H__

struct Vec3i
{
    union
    {
        struct { int x, y, z; };
        int v[3];
    };

    Vec3i() {}
    Vec3i(int x, int y, int z) : x(x), y(y), z(z) {}

    int &operator[](int i) { return v[i]; }
    int operator[](int i) const { return v[i]; }

    bool operator==(const Vec3i &o) const { return x == o.x && y == o.y && z == o.z; }
    bool operator!=(const Vec3i &o) const { return x != o.x || y != o.y || z != o.z; }

    Vec3i operator+(const Vec3i &o) const { return Vec3i(x+o.x, y+o.y, z+o.z); }
    Vec3i operator-(const Vec3i &o) const { return Vec3i(x-o.x, y-o.y, z-o.z); }
    Vec3i operator+(int k) const { return Vec3i(x+k, y+k, z+k); }
    Vec3i operator-(int k) const { return Vec3i(x-k, y-k, z-k); }
    Vec3i operator-() const { return Vec3i(-x, -y, -z); }
    Vec3i operator*(const Vec3i &o) const { return Vec3i(x*o.x, y*o.y, z*o.z); }
    Vec3i operator/(const Vec3i &o) const { return Vec3i(x/o.x, y/o.y, z/o.z); }
    Vec3i operator*(int k) const { return Vec3i(x*k, y*k, z*k); }
    Vec3i operator/(int k) const { return Vec3i(x/k, y/k, z/k); }

    Vec3i &operator+=(const Vec3i &o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vec3i &operator-=(const Vec3i &o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    Vec3i &operator+=(int k) { x += k; y += k; z += k; return *this; }
    Vec3i &operator-=(int k) { x -= k; y -= k; z -= k; return *this; }
    Vec3i &operator*=(const Vec3i &o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
    Vec3i &operator/=(const Vec3i &o) { x /= o.x; y /= o.y; z /= o.z; return *this; }
    Vec3i &operator*=(int k) { x *= k; y *= k; z *= k; return *this; }
    Vec3i &operator/=(int k) { x /= k; y /= k; z /= k; return *this; }
};

struct Vec4;

struct Vec3
{
    union
    {
        struct { float x, y, z; };
        float v[3];
    };

    Vec3() {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3(float yaw, float pitch)
    {
        float c = cosf(pitch);
        x = sinf(yaw)*c;
        y = -cosf(yaw)*c;
        z = sinf(pitch);
    }
    explicit Vec3(const float *v) : x(v[0]), y(v[1]), z(v[2]) {}
    explicit Vec3(const Vec4 &v);

    float &operator[](int i) { return v[i]; }
    float operator[](int i) const { return v[i]; }

    bool operator==(const Vec3 &o) const { return x == o.x && y == o.y && z == o.z; }
    bool operator!=(const Vec3 &o) const { return x != o.x || y != o.y || z != o.z; }

    Vec3 operator+(const Vec3 &o) const { return Vec3(x+o.x, y+o.y, z+o.z); }
    Vec3 operator-(const Vec3 &o) const { return Vec3(x-o.x, y-o.y, z-o.z); }
    Vec3 operator+(float k) const { return Vec3(x+k, y+k, z+k); }
    Vec3 operator-(float k) const { return Vec3(x-k, y-k, z-k); }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    Vec3 operator*(const Vec3 &o) const { return Vec3(x*o.x, y*o.y, z*o.z); }
    Vec3 operator/(const Vec3 &o) const { return Vec3(x/o.x, y/o.y, z/o.z); }
    Vec3 operator*(float k) const { return Vec3(x*k, y*k, z*k); }
    Vec3 operator/(float k) const { return Vec3(x/k, y/k, z/k); }

    Vec3 &operator+=(const Vec3 &o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vec3 &operator-=(const Vec3 &o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    Vec3 &operator+=(float k) { x += k; y += k; z += k; return *this; }
    Vec3 &operator-=(float k) { x -= k; y -= k; z -= k; return *this; }
    Vec3 &operator*=(const Vec3 &o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
    Vec3 &operator/=(const Vec3 &o) { x /= o.x; y /= o.y; z /= o.z; return *this; }
    Vec3 &operator*=(float k) { x *= k; y *= k; z *= k; return *this; }
    Vec3 &operator/=(float k) { x /= k; y /= k; z /= k; return *this; }

    float dot(const Vec3 &o) const { return x*o.x + y*o.y + z*o.z; }
    float magnitude() const { return sqrtf(dot(*this)); }
    float squaredlen() const { return dot(*this); }
    float dist(const Vec3 &o) const { return (*this - o).magnitude(); }
    Vec3 normalize() const { return *this * (1.0f / magnitude()); }
    Vec3 cross(const Vec3 &o) const { return Vec3(y*o.z-z*o.y, z*o.x-x*o.z, x*o.y-y*o.x); }
    Vec3 reflect(const Vec3 &n) const { return *this - n*2.0f*dot(n); }
    Vec3 project(const Vec3 &n) const { return *this - n*dot(n); }
    Vec3 rotate(float c, float s, const Vec3 &axis) const
    {
        return Vec3(x*(axis.x*axis.x*(1-c)+c) + y*(axis.x*axis.y*(1-c)-axis.z*s) + z*(axis.x*axis.z*(1-c)+axis.y*s),
                    x*(axis.y*axis.x*(1-c)+axis.z*s) + y*(axis.y*axis.y*(1-c)+c) + z*(axis.y*axis.z*(1-c)-axis.x*s),
                    x*(axis.x*axis.z*(1-c)-axis.y*s) + y*(axis.y*axis.z*(1-c)+axis.x*s) + z*(axis.z*axis.z*(1-c)+c));
    }
    Vec3 rotate(float angle, const Vec3 &axis) const { return rotate(cosf(angle), sinf(angle), axis); }
    Vec3 orthogonal() const
    {
        int i = fabs(x) > fabs(y) ? (fabs(x) > fabs(z) ? 0 : 2) : (fabs(y) > fabs(z) ? 1 : 2);
        Vec3 o;
        o[i] = v[(i+1)%3];
        o[(i+1)%3] = -v[i];
        o[(i+2)%3] = 0;
        return o;
    }
};

struct Vec4
{
    union
    {
        struct { float x, y, z, w; };
        float v[4];
    };

    Vec4() {}
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    explicit Vec4(const Vec3 &p, float w = 0) : x(p.x), y(p.y), z(p.z), w(w) {}
    explicit Vec4(const float *v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

    float &operator[](int i)       { return v[i]; }
    float  operator[](int i) const { return v[i]; }

    bool operator==(const Vec4 &o) const { return x == o.x && y == o.y && z == o.z && w == o.w; }
    bool operator!=(const Vec4 &o) const { return x != o.x || y != o.y || z != o.z || w != o.w; }

    Vec4 operator+(const Vec4 &o) const { return Vec4(x+o.x, y+o.y, z+o.z, w+o.w); }
    Vec4 operator-(const Vec4 &o) const { return Vec4(x-o.x, y-o.y, z-o.z, w-o.w); }
    Vec4 operator+(float k) const { return Vec4(x+k, y+k, z+k, w+k); }
    Vec4 operator-(float k) const { return Vec4(x-k, y-k, z-k, w-k); }
    Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }
    Vec4 operator*(float k) const { return Vec4(x*k, y*k, z*k, w*k); }
    Vec4 operator/(float k) const { return Vec4(x/k, y/k, z/k, w/k); }

    Vec4 &operator+=(const Vec4 &o) { x += o.x; y += o.y; z += o.z; w += o.w; return *this; }
    Vec4 &operator-=(const Vec4 &o) { x -= o.x; y -= o.y; z -= o.z; w -= o.w; return *this; }
    Vec4 &operator+=(float k) { x += k; y += k; z += k; w += k; return *this; }
    Vec4 &operator-=(float k) { x -= k; y -= k; z -= k; w -= k; return *this; }
    Vec4 &operator*=(float k) { x *= k; y *= k; z *= k; w *= k; return *this; }
    Vec4 &operator/=(float k) { x /= k; y /= k; z /= k; w /= k; return *this; }

    float dot3(const Vec4 &o) const { return x*o.x + y*o.y + z*o.z; }
    float dot3(const Vec3 &o) const { return x*o.x + y*o.y + z*o.z; }
    float dot(const Vec4 &o) const { return dot3(o) + w*o.w; }
    float dot(const Vec3 &o) const  { return x*o.x + y*o.y + z*o.z + w; }
    float magnitude() const  { return sqrtf(dot(*this)); }
    float magnitude3() const { return sqrtf(dot3(*this)); }
    Vec4 normalize() const { return *this * (1.0f / magnitude()); }
    Vec3 cross3(const Vec4 &o) const { return Vec3(y*o.z-z*o.y, z*o.x-x*o.z, x*o.y-y*o.x); }
    Vec3 cross3(const Vec3 &o) const { return Vec3(y*o.z-z*o.y, z*o.x-x*o.z, x*o.y-y*o.x); }
};

inline Vec3::Vec3(const Vec4 &v) : x(v.x), y(v.y), z(v.z) {}

struct Matrix3x3;
struct Matrix3x4;

struct Quat : Vec4
{
    Quat() {}
    Quat(float x, float y, float z, float w) : Vec4(x, y, z, w) {}
    Quat(float angle, const Vec3 &axis)
    {
        float s = sinf(0.5f*angle);
        x = s*axis.x;
        y = s*axis.y;
        z = s*axis.z;
        w = cosf(0.5f*angle);
    }
    explicit Quat(const Matrix3x3 &m) { convertmatrix(m); }
    explicit Quat(const Matrix3x4 &m) { convertmatrix(m); }

    void restorew()
    {
        w = 1.0f - dot3(*this);
        w = w < 0 ? 0 : -sqrtf(w);
    }

    Quat normalize() const { return *this * (1.0f / magnitude()); }

    Quat operator*(float k) const { return Quat(x*k, y*k, z*k, w*k); }
    Quat &operator*=(float k) { return (*this = *this * k); }

    Quat operator*(const Quat &o) const
    {
        return Quat(w*o.x + x*o.w + y*o.z - z*o.y,
                    w*o.y - x*o.z + y*o.w + z*o.x,
                    w*o.z + x*o.y - y*o.x + z*o.w,
                    w*o.w - x*o.x - y*o.y - z*o.z);
    }
    Quat &operator*=(const Quat &o) { return (*this = *this * o); }

    Quat operator+(const Quat &o) { return Quat(x+o.x, y+o.y, z+o.z, w+o.w); }
    Quat operator-() const { return Quat(-x, -y, -z, w); }

    void flip() { x = -x; y = -y; z = -z; w = -w; }

    Vec3 transform(const Vec3 &p) const
    {
        Vec3 t1 = cross3(p), t2 = cross3(t1);
        return p + (t1*w + t2)*2.0f;
    }

    template<class M>
    void convertmatrix(const M &m)
    {
        float trace = m.a.x + m.b.y + m.c.z;
        if(trace>0)
        {
            float r = sqrtf(1 + trace), inv = 0.5f/r;
            w = 0.5f*r;
            x = (m.c.y - m.b.z)*inv;
            y = (m.a.z - m.c.x)*inv;
            z = (m.b.x - m.a.y)*inv;
        }
        else if(m.a.x > m.b.y && m.a.x > m.c.z)
        {
            float r = sqrtf(1 + m.a.x - m.b.y - m.c.z), inv = 0.5f/r;
            x = 0.5f*r;
            y = (m.b.x + m.a.y)*inv;
            z = (m.a.z + m.c.x)*inv;
            w = (m.c.y - m.b.z)*inv;
        }
        else if(m.b.y > m.c.z)
        {
            float r = sqrtf(1 + m.b.y - m.a.x - m.c.z), inv = 0.5f/r;
            x = (m.b.x + m.a.y)*inv;
            y = 0.5f*r;
            z = (m.c.y + m.b.z)*inv;
            w = (m.a.z - m.c.x)*inv;
        }
        else
        {
            float r = sqrtf(1 + m.c.z - m.a.x - m.b.y), inv = 0.5f/r;
            x = (m.a.z + m.c.x)*inv;
            y = (m.c.y + m.b.z)*inv;
            z = 0.5f*r;
            w = (m.b.x - m.a.y)*inv;
        }
    }
};

struct DualQuat
{
    Quat real, dual;
    
    DualQuat() {}
    DualQuat(const Quat &rot, const Vec3 &trans) : real(rot)
    {
        dual.x =  0.5f*( trans.x*rot.w + trans.y*rot.z - trans.z*rot.y);
        dual.y =  0.5f*(-trans.x*rot.z + trans.y*rot.w + trans.z*rot.x);
        dual.z =  0.5f*( trans.x*rot.y - trans.y*rot.x + trans.z*rot.w);
        dual.w = -0.5f*( trans.x*rot.x + trans.y*rot.y + trans.z*rot.z);
    }
    DualQuat(const Quat &real, const Quat &dual) : real(real), dual(dual) {}
    explicit DualQuat(const Quat &q) : real(q), dual(0, 0, 0, 0) {}
    
    DualQuat operator*(const DualQuat &o) const
    {
        return DualQuat(real*o.real, real*o.dual + dual*o.real);
    }

    void fixantipodal(const DualQuat &o)
    {
        if(real.dot(o.real) < 0)
        {
            real.flip();
            dual.flip();
        }
    }

    Vec3 transform(const Vec3 &p) const
    {
        Vec3 t1 = real.cross3(p) + p*real.w,
             t2 = real.cross3(t1),
             t3 = real.cross3(dual) + Vec3(dual)*real.w + Vec3(real)*dual.w;
        return p + (t2 + t3)*2.0f;
    }
};

struct Matrix3x3
{
    Vec3 a, b, c;

    Matrix3x3() {}
    Matrix3x3(const Vec3 &a, const Vec3 &b, const Vec3 &c) : a(a), b(b), c(c) {}
    explicit Matrix3x3(const Quat &q)
    {
        float x = q.x, y = q.y, z = q.z, w = q.w,
              ww = w*w, xx = x*x, yy = y*y, zz = z*z,
              xy = x*y, xz = x*z, yz = y*z,
              wx = w*x, wy = w*y, wz = w*z;
        a = Vec3(ww + xx - yy - zz, 2*(xy - wz), 2*(xz + wy));
        b = Vec3(2*(xy + wz), ww + yy - xx - zz, 2*(yz - wx));
        c = Vec3(2*(xz - wy), 2*(yz + wx), ww + zz - xx - yy);

        float invrr = 1.0f/q.dot(q);
        a *= invrr;
        b *= invrr;
        c *= invrr;
    }

    Matrix3x3 operator*(const Matrix3x3 &o) const
    {
        return Matrix3x3(
            Vec3(a.dot(Vec3(o.a.x, o.b.x, o.c.x)),
                 a.dot(Vec3(o.a.y, o.b.y, o.c.y)),
                 a.dot(Vec3(o.a.z, o.b.z, o.c.z))),
            Vec3(b.dot(Vec3(o.a.x, o.b.x, o.c.x)),
                 b.dot(Vec3(o.a.y, o.b.y, o.c.y)),
                 b.dot(Vec3(o.a.z, o.b.z, o.c.z))),
            Vec3(c.dot(Vec3(o.a.x, o.b.x, o.c.x)),
                 c.dot(Vec3(o.a.y, o.b.y, o.c.y)),
                 c.dot(Vec3(o.a.z, o.b.z, o.c.z))));
    }
    Matrix3x3 &operator*=(const Matrix3x3 &o) { return (*this = *this * o); }

    void transpose(const Matrix3x3 &o)
    {
        a = Vec3(o.a.x, o.b.x, o.c.x);
        b = Vec3(o.a.y, o.b.y, o.c.y);
        c = Vec3(o.a.z, o.b.z, o.c.z);
    }

    void rotate(float angle, const Vec3 &axis)
    {
        rotate(cosf(angle), sinf(angle), axis);
    }

    void rotate(float ck, float sk, const Vec3 &axis)
    {
        a = Vec3(axis.x*axis.x*(1-ck)+ck, axis.x*axis.y*(1-ck)-axis.z*sk, axis.x*axis.z*(1-ck)+axis.y*sk);
        b = Vec3(axis.y*axis.x*(1-ck)+axis.z*sk, axis.y*axis.y*(1-ck)+ck, axis.y*axis.z*(1-ck)-axis.x*sk);
        c = Vec3(axis.x*axis.z*(1-ck)-axis.y*sk, axis.y*axis.z*(1-ck)+axis.x*sk, axis.z*axis.z*(1-ck)+ck);
    }

    void calcangleaxis(float &angle, Vec3 &axis)
    {
        angle = acosf(clamp(0.5f*(a.x + b.y + c.z - 1), -1.0f, 1.0f));

        if(angle < M_PI)
            axis = Vec3(c.y - b.z, a.z - c.x, b.x - a.y).normalize();
        else if(a.x >= b.y)
        {
            if(a.x >= c.z)
            {
                axis.x = 0.5f*sqrtf(a.x - b.y - c.z + 1);
                float k = 0.5f/axis.x;
                axis.y = a.y*k;
                axis.z = a.z*k;
            }
            else
            {
                axis.z = 0.5f*sqrtf(c.z - a.x - b.y + 1);
                float k = 0.5f/axis.z;
                axis.x = a.z*k;
                axis.y = b.z*k;
            }
        }
        else
        {
            if(b.y >= c.z)
            {
                axis.y = 0.5f*sqrtf(b.y - a.x - c.z + 1);
                float k = 0.5f/axis.y;
                axis.x = a.y*k;
                axis.z = b.z*k;
            }
            else
            {
                axis.z = 0.5f*sqrtf(c.z - a.x - b.y + 1);        
                float k = 0.5f/axis.z;
                axis.x = a.z*k;
                axis.y = b.z*k;
            }
        }
    }

    Vec3 transform(const Vec3 &o) const { return Vec3(a.dot(o), b.dot(o), c.dot(o)); }
    Vec3 transposedtransform(const Vec3 &o) const
    {
        return Vec3(a.x*o.x + b.x*o.y + c.x*o.z,
                   a.y*o.x + b.y*o.y + c.y*o.z,
                   a.z*o.x + b.z*o.y + c.z*o.z);
    }
};

struct Matrix3x4
{
    Vec4 a, b, c;

    Matrix3x4() {}
    Matrix3x4(const Vec4 &a, const Vec4 &b, const Vec4 &c) : a(a), b(b), c(c) {}
    explicit Matrix3x4(const Matrix3x3 &rot, const Vec3 &trans)
        : a(Vec4(rot.a, trans.x)), b(Vec4(rot.b, trans.y)), c(Vec4(rot.c, trans.z))
    {
    }
    explicit Matrix3x4(const DualQuat &d)
    {
        float x = d.real.x, y = d.real.y, z = d.real.z, w = d.real.w,
              ww = w*w, xx = x*x, yy = y*y, zz = z*z,
              xy = x*y, xz = x*z, yz = y*z,
              wx = w*x, wy = w*y, wz = w*z;
        a = Vec4(ww + xx - yy - zz, 2*(xy - wz), 2*(xz + wy),
            -2*(d.dual.w*x - d.dual.x*w + d.dual.y*z - d.dual.z*y));
        b = Vec4(2*(xy + wz), ww + yy - xx - zz, 2*(yz - wx),
            -2*(d.dual.w*y - d.dual.x*z - d.dual.y*w + d.dual.z*x));
        c = Vec4(2*(xz - wy), 2*(yz + wx), ww + zz - xx - yy,
            -2*(d.dual.w*z + d.dual.x*y - d.dual.y*x - d.dual.z*w));

        float invrr = 1.0f/d.real.dot(d.real);
        a *= invrr;
        b *= invrr;
        c *= invrr;
    }

    Matrix3x4 &operator*=(float k)
    {
        a *= k;
        b *= k;
        c *= k;
        return *this;
    }

    Matrix3x4 &operator+=(const Vec3 &p)
    {
        a.w += p.x;
        b.w += p.y;
        c.w += p.z;
        return *this;
    }

    void identity()
    {
        a = Vec4(1, 0, 0, 0);
        b = Vec4(0, 1, 0, 0);
        c = Vec4(0, 0, 1, 0);
    }

    void rotate(float angle, const Vec3 &axis)
    {
        rotate(cosf(angle), sinf(angle), axis);
    }

    void rotate(float ck, float sk, const Vec3 &axis)
    {
        a = Vec4(axis.x*axis.x*(1-ck)+ck, axis.x*axis.y*(1-ck)-axis.z*sk, axis.x*axis.z*(1-ck)+axis.y*sk, 0);
        b = Vec4(axis.y*axis.x*(1-ck)+axis.z*sk, axis.y*axis.y*(1-ck)+ck, axis.y*axis.z*(1-ck)-axis.x*sk, 0);
        c = Vec4(axis.x*axis.z*(1-ck)-axis.y*sk, axis.y*axis.z*(1-ck)+axis.x*sk, axis.z*axis.z*(1-ck)+ck, 0);
    }

    Matrix3x4 operator*(const Matrix3x4 &o) const
    {
        return Matrix3x4(
            Vec4(a.dot3(Vec3(o.a.x, o.b.x, o.c.x)),
                 a.dot3(Vec3(o.a.y, o.b.y, o.c.y)),
                 a.dot3(Vec3(o.a.z, o.b.z, o.c.z)),
                 a.dot(Vec3(o.a.w, o.b.w, o.c.w))),
            Vec4(b.dot3(Vec3(o.a.x, o.b.x, o.c.x)),
                 b.dot3(Vec3(o.a.y, o.b.y, o.c.y)),
                 b.dot3(Vec3(o.a.z, o.b.z, o.c.z)),
                 b.dot(Vec3(o.a.w, o.b.w, o.c.w))),
            Vec4(c.dot3(Vec3(o.a.x, o.b.x, o.c.x)),
                 c.dot3(Vec3(o.a.y, o.b.y, o.c.y)),
                 c.dot3(Vec3(o.a.z, o.b.z, o.c.z)),
                 c.dot(Vec3(o.a.w, o.b.w, o.c.w))));
    }
    Matrix3x4 &operator*=(const Matrix3x4 &o) { return (*this = *this * o); }

    Vec3 transform(const Vec3 &o) const { return Vec3(a.dot(o), b.dot(o), c.dot(o)); }
    Vec3 transposedtransform(const Vec3 &o) const
    {
        Vec3 p = o;
        p.x -= a.w;
        p.y -= b.w;
        p.z -= c.w;
        return Vec3(a.x*p.x + b.x*p.y + c.x*p.z,
                   a.y*p.x + b.y*p.y + c.y*p.z,
                   a.z*p.x + b.z*p.y + c.z*p.z);
    }
    Vec3 transformnormal(const Vec3 &o) const { return Vec3(a.dot3(o), b.dot3(o), c.dot3(o)); }
    Vec3 transposedtransformnormal(const Vec3 &o) const
    {
        return Vec3(a.x*o.x + b.x*o.y + c.x*o.z,
                   a.y*o.x + b.y*o.y + c.y*o.z,
                   a.z*o.x + b.z*o.y + c.z*o.z);
    }
};

struct Plane : Vec4
{
    Plane() {}
    Plane(float x, float y, float z, float w) : Vec4(x, y, z, w) {}
    Plane(const Vec4 &p) : Vec4(p) {}
    Plane(const Vec3 &n, float w) : Vec4(n, w) {}
    Plane(const Vec3 &n, const Vec3 &p) : Vec4(n, -n.dot(p)) {}
    Plane(const Vec3 &a, const Vec3 &b, const Vec3 &c)
    {
        Vec3 n = (b - a).cross(c - a).normalize();
        x = n.x;
        y = n.y;
        z = n.z;
        w = -n.dot(a);
    }

    float dist(const Vec3 &p) const { return dot(p); }
};

template<class T, class Z>
struct GLMatrix
{
    T v[16];

    T &at(int row, int col) { return v[col*4 + row]; }
    T at(int row, int col) const { return v[col*4 + row]; }

    void zero() { memset(v, 0, sizeof(v)); }

    void rotatetranslate(const Quat &q, const Vec3 &p)
    {
        at(0, 0) = 1 - 2*(q.y*q.y+q.z*q.z);
        at(0, 1) = 2*(q.x*q.y-q.z*q.w);
        at(0, 2) = 2*(q.x*q.z+q.y*q.w);
        at(0, 3) = p.x;

        at(1, 0) = 2*(q.x*q.y+q.z*q.w);
        at(1, 1) = 1 - 2*(q.x*q.x+q.z*q.z);
        at(1, 2) = 2*(q.y*q.z-q.x*q.w);
        at(1, 3) = p.y;

        at(2, 0) = 2*(q.x*q.z-q.y*q.w);
        at(2, 1) = 2*(q.y*q.z+q.x*q.w);
        at(2, 2) = 1-2*(q.x*q.x+q.y*q.y);
        at(2, 3) = p.z;

        loopk(3) at(3, k) = 0;
        at(3, 3) = 1;
    }

    void identity()
    {
        zero();
        loopi(4) at(i, i) = 1;
    }

    void scale(T x, T y, T z)
    {
        zero();
        at(0, 0) = x;
        at(1, 1) = y;
        at(2, 2) = z;
        at(3, 3) = 1;
    }

    void translate(T x, T y, T z)
    {
        identity();
        at(0, 3) = x;
        at(1, 3) = y;
        at(2, 3) = z;
    }

    void scaletranslate(T kx, T ky, T kz, T tx, T ty, T tz)
    {
        scale(kx, ky, kz);
        at(0, 3) = tx;
        at(1, 3) = ty;
        at(2, 3) = tz;
    }

    void rotate(T angle, T x, T y, T z)
    {
        T s = (T)sin(radians(angle)), c = (T)cos(radians(angle));

        at(0, 0) = x*x*(1-c)+c;
        at(0, 1) = x*y*(1-c)-z*s;
        at(0, 2) = x*z*(1-c)+y*s;
        at(0, 3) = 0;

        at(1, 0) = y*x*(1-c)+z*s;
        at(1, 1) = y*y*(1-c)+c;
        at(1, 2) = y*z*(1-c)-x*s;
        at(1, 3) = 0;

        at(2, 0) = x*z*(1-c)-y*s;
        at(2, 1) = y*z*(1-c)+x*s;
        at(2, 2) = z*z*(1-c)+c;
        at(2, 3) = 0;

        loopk(3) at(3, k) = 0;
        at(3, 3) = 1;
    }

    bool invert(const GLMatrix &n, T mindet = 1.0e-10f)
    {
        T det = n.det4x4();
        if(fabs(det)<mindet) return false;
        adjoint(n);
        T invdet = 1/det;
        loopi(16) v[i] *= invdet;
        return true;
    }

    void transpose(const GLMatrix &n)
    {
        loop(row, 4) loop(col, 4) at(row, col) = n.at(col, row);
    }

    bool inversetranspose(const GLMatrix &n)
    {
        GLMatrix t;
        if(!t.invert(n)) return false;
        transpose(t);
        return true;
    }

    void adjoint(const GLMatrix &n)
    {
        T a1 = n.at(0, 0), a2 = n.at(1, 0), a3 = n.at(2, 0), a4 = n.at(3, 0),
          b1 = n.at(0, 1), b2 = n.at(1, 1), b3 = n.at(2, 1), b4 = n.at(3, 1),
          c1 = n.at(0, 2), c2 = n.at(1, 2), c3 = n.at(2, 2), c4 = n.at(3, 2),
          d1 = n.at(0, 3), d2 = n.at(1, 3), d3 = n.at(2, 3), d4 = n.at(3, 3);

        at(0, 0) =  det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4);
        at(1, 0) = -det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4);
        at(2, 0) =  det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4);
        at(3, 0) = -det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);

        at(0, 1) = -det3x3(b1, b3, b4, c1, c3, c4, d1, d3, d4);
        at(1, 1) =  det3x3(a1, a3, a4, c1, c3, c4, d1, d3, d4);
        at(2, 1) = -det3x3(a1, a3, a4, b1, b3, b4, d1, d3, d4);
        at(3, 1) =  det3x3(a1, a3, a4, b1, b3, b4, c1, c3, c4);

        at(0, 2) =  det3x3(b1, b2, b4, c1, c2, c4, d1, d2, d4);
        at(1, 2) = -det3x3(a1, a2, a4, c1, c2, c4, d1, d2, d4);
        at(2, 2) =  det3x3(a1, a2, a4, b1, b2, b4, d1, d2, d4);
        at(3, 2) = -det3x3(a1, a2, a4, b1, b2, b4, c1, c2, c4);

        at(0, 3) = -det3x3(b1, b2, b3, c1, c2, c3, d1, d2, d3);
        at(1, 3) =  det3x3(a1, a2, a3, c1, c2, c3, d1, d2, d3);
        at(2, 3) = -det3x3(a1, a2, a3, b1, b2, b3, d1, d2, d3);
        at(3, 3) =  det3x3(a1, a2, a3, b1, b2, b3, c1, c2, c3);
    }

    T det2x2(T a, T b, T c, T d) const { return a*d - b*c; }
    T det3x3(T a1, T a2, T a3,
             T b1, T b2, T b3,
             T c1, T c2, T c3) const
    {
        return a1 * det2x2(b2, b3, c2, c3)
             - b1 * det2x2(a2, a3, c2, c3)
             + c1 * det2x2(a2, a3, b2, b3);
    }
    T det4x4() const
    {
        T a1 = at(0, 0), a2 = at(1, 0), a3 = at(2, 0), a4 = at(3, 0),
          b1 = at(0, 1), b2 = at(1, 1), b3 = at(2, 1), b4 = at(3, 1),
          c1 = at(0, 2), c2 = at(1, 2), c3 = at(2, 2), c4 = at(3, 2),
          d1 = at(0, 3), d2 = at(1, 3), d3 = at(2, 3), d4 = at(3, 3);

        return a1 * det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4)
             - b1 * det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4)
             + c1 * det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4)
             - d1 * det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);
    }

    Z operator*(const GLMatrix &o) const
    {
        Z t;
        loop(row, 4) loop(col, 4)
        {
            t.at(row, col) = at(row, 0)*o.at(0, col) + at(row, 1)*o.at(1, col) + at(row, 2)*o.at(2, col) + at(row, 3)*o.at(3, col);
        }
        return t;
    }
    Z &operator*=(const GLMatrix &o) { return (*(Z *)this = *this * o); }
        
    template<class U>
    U transform3(const U &v) const
    {
        T w = 1/(at(3, 0)*v.x + at(3, 1)*v.y + at(3, 2)*v.z + at(3, 3));
        return U(at(0, 0)*v.x + at(0, 1)*v.y + at(0, 2)*v.z + at(0, 3),
                 at(1, 0)*v.x + at(1, 1)*v.y + at(1, 2)*v.z + at(1, 3),
                 at(2, 0)*v.x + at(2, 1)*v.y + at(2, 2)*v.z + at(2, 3)) * w;
    }

    template<class U>
    U transform4(const U &v) const
    {
        return U(at(0, 0)*v.x + at(0, 1)*v.y + at(0, 2)*v.z + at(0, 3),
                 at(1, 0)*v.x + at(1, 1)*v.y + at(1, 2)*v.z + at(1, 3),
                 at(2, 0)*v.x + at(2, 1)*v.y + at(2, 2)*v.z + at(2, 3),
                 at(3, 0)*v.x + at(3, 1)*v.y + at(3, 2)*v.z + at(3, 3));
    }
    
    Vec3 transform(const Vec3 &v) const { return transform3(v); }
    Vec4 transform(const Vec4 &v) const { return transform4(v); }
    Plane transform(const Plane &p) const { return transform4(p); }
    
    void unprojectxy(T z)
    {
        T zk = at(2, 2) - z*at(3, 2),
          xk = (z*at(3, 0) - at(2, 0)) / zk,
          yk = (z*at(3, 1) - at(2, 1)) / zk,
          wk = (z*at(3, 3) - at(2, 3)) / zk;

        loopi(4)
        {
            at(i, 0) += at(i, 2)*xk;
            at(i, 1) += at(i, 2)*yk;
            at(i, 3) += at(i, 2)*wk;
            at(i, 2) = 0;
        }
    }

    void mvpmatrix()
    {
        Z mv, p;
        p.projectionmatrix();
        mv.modelviewmatrix();
        *this = p * mv;
    }
};

struct GLMatrixf : GLMatrix<float, GLMatrixf>
{
    void texturematrix() { glGetFloatv(GL_TEXTURE_MATRIX, v); }
    void projectionmatrix() { glGetFloatv(GL_PROJECTION_MATRIX, v); }
    void modelviewmatrix() { glGetFloatv(GL_MODELVIEW_MATRIX, v); }
};

struct GLMatrixd : GLMatrix<double, GLMatrixd>
{
    void texturematrix() { glGetDoublev(GL_TEXTURE_MATRIX, v); }
    void projectionmatrix() { glGetDoublev(GL_PROJECTION_MATRIX, v); }
    void modelviewmatrix() { glGetDoublev(GL_MODELVIEW_MATRIX, v); }
};

#endif

