#pragma once
#include <vector>

namespace eve {
    union hexcolor {
        struct { float r, g, b, a; };
        struct { float e[4]; };

        // #abc #abcd #abc,d abc,d abc abcd abc,d
        hexcolor( const char *rgba, char a = 'f' ) {
            if( *rgba == '#' ) rgba++;
            r = (*rgba++) - '0'; if(r > 9) r += '0' - 'a' + 10;
            g = (*rgba++) - '0'; if(g > 9) g += '0' - 'a' + 10;
            b = (*rgba++) - '0'; if(b > 9) b += '0' - 'a' + 10;
            if( !*rgba ) rgba = &a;
            a = (*rgba++) - '0'; if(a > 9) a += '0' - 'a' + 10;
        }

        hexcolor( float r, float g, float b, float a = 1 ) : r(r),g(g),b(b),a(a)
        {}

        hexcolor alpha( float opacity ) const {
            return hexcolor( r, g, b, opacity );
        }
    };

    using hexcolors = std::vector<hexcolor>;
}

#define color static const eve::hexcolor
#define rgb(r,g,b) (r/255.f,g/255.f,b/255.f)
#define hsl(h,s,l) ;

namespace flatui {
    // turquoise
    color cyan
    rgb(26, 188, 156)
    hsl(168, 76%, 42%)

    // emerland
    color green
    rgb(46, 204, 113)
    hsl(145, 63%, 49%)

    // peterriver
    color blue
    rgb(52, 152, 219)
    hsl(204, 70%, 53%)

    // amethyst
    color purple
    rgb(155, 89, 182)
    hsl(283, 39%, 53%)

    // wetasphalt
    color navy
    rgb(52, 73, 94)
    hsl(210, 29%, 29%)

    // greensea
    color dark_cyan
    rgb(22, 160, 133)
    hsl(168, 76%, 36%)

    // nephritis
    color dark_green
    rgb(39, 174, 96)
    hsl(145, 63%, 42%)

    // belizehole
    color dark_blue
    rgb(41, 128, 185)
    hsl(204, 64%, 44%)

    // wisteria
    color dark_purple
    rgb(142, 68, 173)
    hsl(282, 44%, 47%)

    // midnightblue
    color dark_navy
    rgb(44, 62, 80)
    hsl(210, 29%, 24%)

    // sunflower
    color yellow
    rgb(241, 196, 15)
    hsl(48, 89%, 50%)

    // carrot
    color orange
    rgb(230, 126, 34)
    hsl(28, 80%, 52%)

    // alizarin
    color red
    rgb(231, 76, 60)
    hsl(6, 78%, 57%)

    // clouds
    color white
    rgb(236, 240, 241)
    hsl(192, 15%, 94%)

    // concrete
    color gray
    rgb(149, 165, 166)
    hsl(184, 9%, 62%)

    // orange
    color dark_yellow
    rgb(243, 156, 18)
    hsl(37, 90%, 51%)

    // pumpkin
    color dark_orange
    rgb(211, 84, 0)
    hsl(24, 100%, 41%)

    // pomegranate
    color dark_red
    rgb(192, 57, 43)
    hsl(6, 63%, 46%)

    // silver
    color dark_white
    rgb(189, 195, 199)
    hsl(204, 8%, 76%)

    // asbestos
    color dark_gray
    rgb(127, 140, 141)
    hsl(184, 6%, 53%)
}

#undef color
#undef rgb
#undef hsl
