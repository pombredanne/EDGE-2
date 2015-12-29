#ifndef EE_COLOR_HPP
#define EE_COLOR_HPP

struct Color {
    unsigned char r,g,b,a;

    Color() : r(0), g(0), b(0), a(255) {}
    Color(char _r, char _g, char _b, char _a = 0) : r(_r), g(_g), b(_b), a(_a) {}
    Color(int c) :
        r((c >> 24) & 0xff), 
        g((c >> 16) & 0xff),
        b((c >>  8) & 0xff),
        a((c >>  0) & 0xff)
    {}
};

#endif
