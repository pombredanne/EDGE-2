#ifndef EE_MACROS_HPP
#define EE_MACROS_HPP

#include <stdint.h>

#define $yes(...)       __VA_ARGS__
#define $no(...)

// usage: const char *txt = $quote(This is a string)
// usage: $comment( puts("skipped"); )
// usage: $uncomment( puts("printed"); )

#define $quote(...)     #__VA_ARGS__
#define $comment(...)   $no
#define $uncomment(...) $yes

// usage: #if $defined($msvc)
// usage: #if $has(cxx_exceptions)

#define $defined(v)     (0 v(+1))
#define $has(...)       $clang(__has_feature(__VA_ARGS__)) $celse(__VA_ARGS__) 

// Platform utils

#if defined(_WIN32)
#   define $windows   $yes
#   define $welse     $no
#else
#   define $windows   $no
#   define $welse     $yes
#endif

#ifdef __APPLE__
#   define $apple     $yes
#   define $aelse     $no
#else
#   define $apple     $no
#   define $aelse     $yes
#endif

#ifdef __linux__
#   define $linux     $yes
#   define $lelse     $no
#else
#   define $linux     $no
#   define $lelse     $yes
#endif

#if $defined($windows) || $defined($linux) || $defined($apple)
#   define $supported_os $yes
#else
#   define $supported_os $no
#endif

// Compiler utils

#if ( defined(_MSC_VER) && _MSC_VER >= 1800 ) || __cplusplus >= 201103L
#   define $cpp11     $yes
#   define $cpp03     $no
#else
#   define $cpp11     $no
#   define $cpp03     $yes
#endif

#if ULONG_MAX == 4294967295
#   define $bits64    $yes
#   define $bits32    $no
#else
#   define $bits64    $no
#   define $bits32    $yes
#endif

#if defined(NDEBUG) || defined(_NDEBUG) || defined(RELEASE)
#   define $release   $yes
#   define $debug     $no
#else
#   define $release   $no
#   define $debug     $yes
#endif

#if defined(NDEVEL) || defined(_NDEVEL) || defined(PUBLIC)
#   define $public    $yes
#   define $devel     $no
#else
#   define $public    $no
#   define $devel     $yes
#endif

#if defined(__GNUC__) || defined(__MINGW32__)
#   define $gnuc      $yes
#   define $gelse     $no
#else
#   define $gnuc      $no
#   define $gelse     $yes
#endif

#ifdef _MSC_VER
#   define $msvc      $yes
#   define $melse     $no
#else
#   define $msvc      $no
#   define $melse     $yes
#endif

#ifdef __clang__
#   define $clang     $yes
#   define $celse     $no
#else
#   define $clang     $no
#   define $celse     $yes
#endif

#if $defined($gnuc) || $defined($clang)
#   define $likely(expr)    (__builtin_expect(!!(expr), 1))
#   define $unlikely(expr)  (__builtin_expect(!!(expr), 0))
#else
#   define $likely(expr)    ((expr))
#   define $unlikely(expr)  ((expr))
#endif

#if $defined($msvc) || $defined($gnuc) || $defined($clang)
#   define $supported_compiler $yes
#else
#   define $supported_compiler $no
#endif

// Try to guess if exceptions are enabled...

#if (defined(_HAS_EXCEPTIONS) && (_HAS_EXCEPTIONS > 0)) || \
    (defined(_STLP_USE_EXCEPTIONS) && (_STLP_USE_EXCEPTIONS > 0)) || \
    (defined(HAVE_EXCEPTIONS)) || \
    (defined(__EXCEPTIONS)) || \
    (defined(_CPPUNWIND)) || \
    ($has(cxx_exceptions))
#   define $throw     $yes
#   define $telse     $no
#else
#   define $throw     $no
#   define $telse     $yes
#endif

// Thread Local Storage

// if   MingW, Solaris Studio C/C++, IBM XL C/C++, GNU C, Clang and Intel C++ Compiler (Linux systems)
// else Visual C++, Intel C/C++ (Windows systems), C++Builder and Digital Mars C++
#if defined(__MINGW32__) || defined(__SUNPRO_C) || defined(__xlc__) || defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
#    define $tls(x) __thread x
#else
#    define $tls(x) __declspec(thread) x
#endif

#endif
