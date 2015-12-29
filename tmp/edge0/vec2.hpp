#ifndef EE_VEC2_HPP
#define EE_VEC2_HPP

template <typename T>
struct vec2 {
    T x, y;

    vec2() : x(T()), y(T()) {}
    vec2(T _v) : x(_v), y(_v) {}
    vec2(T _x, T _y) : x(_x), y(_y) {}

    template<typename T2>
    const vec2<T2> as() const {
        return vec2<T2>(T2(x), T2(y));
    }

    vec2<T>& operator -=(const vec2<T>& o) {
        this->x -= o.x;
        this->y -= o.y;

        return *this;
    }

    vec2<T>& operator +=(const vec2<T>& o) {
        this->x += o.x;
        this->y += o.y;

        return *this;
    }

    const vec2<T> operator -(const vec2<T>& o) const {
        return vec2<T>(x - o.x, y - o.y);
    }

    const vec2<T> operator +(const vec2<T>& o) const {
        return vec2<T>(x + o.x, y + o.y);
    }

    const vec2<T> operator /(const vec2<T>& o) const {
        return vec2<T>(x / o.x, y / o.y);
    }

    const vec2<T> operator /(T o) const {
        return vec2<T>(x / o, y / o);
    }

    const vec2<T> operator *(const vec2<T>& o) const {
        return vec2<T>(x * o.x, y * o.y);
    }

    const vec2<T> operator *(T o) const {
        return vec2<T>(x * o, y * o);
    }

    bool operator ==(const vec2<T>& o) const {
        return x == o.x && y == o.y;
    }

    bool operator !=(const vec2<T>& o) const {
        return x != o.x || y != o.y;
    }
};

typedef vec2<float> vec2f;
typedef vec2<int>   vec2i;

#endif
