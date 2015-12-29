extern int appmain( int argc, const char *argv[] );

#ifdef main
#undef main
#endif

#if 0

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <stdio.h>

#if defined(main)
#   undef main
#   if _MSC_VER >= 1900
        FILE _iob[] = { *stdin, *stdout, *stderr };
        extern "C" FILE * __cdecl __iob_func(void) {
            return _iob;
        }
#   endif
extern "C"
#endif

#endif

int main(int argc, char *argv[]) {
    return appmain( argc, (const char **)argv );
}
