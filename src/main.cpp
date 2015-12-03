#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

// directives

// Check windows
#if _WIN32 || _WIN64
#   if _WIN64
#       define EE_BITS 64
#   else
#       define EE_BITS 32
#   endif
#endif

// Check GCC
#if __GNUC__
#   if __x86_64__ || __ppc64__
#       define EE_BITS 64
#   else
#       define EE_BITS 32
#   endif
#endif

static_assert( EE_BITS >= 64, "You should be compiling on x64 arch");

#define EE_VERSION "0.0.0" /* (2015/12/05) Initial commit */

#define EE_STR2(x) #x
#define EE_STR(x) EE_STR2(x)

#include "../prj/git.hpp"

#define EE_BUILD "DEBUG"
#define EE_URL "https://github.com/r-lyeh/EDGE"
#define EE_TEXT "EDGE " EE_VERSION " (" EE_STR(EE_BITS) "-bit " EE_BUILD ") (git:" GIT_BRANCH " r" GIT_REVISION ")"

#if defined(DEBUGOPT)
#undef  EE_BUILD
#define EE_BUILD "DEBUGOPT"
#endif

#if defined(RELEASE) || defined(NDEBUG) || defined(_NDEBUG)
#undef  EE_BUILD
#define EE_BUILD "RELEASE"
#endif

#if defined(SHIPPING)
#undef  EE_BUILD
#define EE_BUILD "SHIPPING"
#endif

// core interface

int eeInit();
int eeUpdate();
int eeRender();
int eeQuit();

int eeLoadModule();
int eeQuitModule();

int main( int argc, const char **argv ) {
    printf("%s\n", EE_TEXT);
}
