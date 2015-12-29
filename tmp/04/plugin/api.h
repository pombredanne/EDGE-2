#ifndef YOURLIB_H_INCLUDED
#define YOURLIB_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

/* code goes here */

#ifdef __cplusplus
}
#endif
#endif

#ifndef YL_API
#  ifdef _WIN32
#     if defined(YL_BUILD_SHARED) /* build dll */
#         define YL_API __declspec(dllexport)
#     elif !defined(YL_BUILD_STATIC) /* use dll */
#         define YL_API __declspec(dllimport)
#     else /* static library */
#         define YL_API
#     endif
#  else
#     if __GNUC__ >= 4
#         define YL_API __attribute__((visibility("default")))
#     else
#         define YL_API
#     endif
#  endif
#endif

#define YL_VERSION_MAJOR 1
#define YL_VERSION_MINOR 0
#define YL_VERSION ((YL_VERSION_MAJOR << 16) | YL_VERSION_MINOR)

unsigned int yl_get_version(void);
int yl_is_compatible_dll(void);

unsigned int yl_get_version(void)
{
    return YL_VERSION;
}

int yl_is_compatible_dll(void)
{
    unsigned int major = yl_get_version() >> 16;
    return major == YL_VERSION_MAJOR;
}


YL_API void yl_set_allocators(void *(*f_malloc)(size_t),
                              void *(*f_realloc)(void *, size_t),
                              void (*f_free)(void *));
YL_API void *yl_malloc(size_t size);
YL_API void *yl_realloc(void *ptr, size_t size);
YL_API void *yl_calloc(size_t count, size_t size);
YL_API void yl_free(void *ptr);
YL_API char *yl_strdup(const char *str);

struct yl_allocators_s {
    void *(*f_malloc)(size_t);
    void *(*f_realloc)(void *, size_t);
    void (*f_free)(void *);
};
extern struct yl_allocators_s _yl_allocators;

inline void *yl_malloc(size_t size)
{
    return _yl_allocators.f_malloc(size);
}

inline void *yl_realloc(void *ptr, size_t size)
{
    return _yl_allocators.f_realloc(ptr, size);
}

inline void yl_free(void *ptr)
{
    _yl_allocators.f_free(ptr);
}

inline void *yl_calloc(size_t count, size_t size)
{
    void *ptr = _yl_allocators.f_malloc(count * size);
    memset(ptr, 0, count * size);
    return ptr;
}

inline char *yl_strdup(const char *str)
{
    size_t length = strlen(str) + 1;
    char *rv = (char *)yl_malloc(length);
    memcpy(rv, str, length);
    return rv;
}

struct yl_allocators_s _yl_allocators = {
    malloc,
    realloc,
    free
};

void yl_set_allocators(void *(*f_malloc)(size_t),
                       void *(*f_realloc)(void *, size_t),
                       void (*f_free)(void *))
{
    _yl_allocators.f_malloc = f_malloc;
    _yl_allocators.f_realloc = f_realloc;
    _yl_allocators.f_free = f_free;
}

#define YL_IMPLEMENTS_ALLOCATORS \
public: \
    void *operator new(size_t size) { return yl_malloc(size); } \
    void operator delete(void *ptr) { yl_free(ptr); } \
    void *operator new[](size_t size) { return yl_malloc(size); } \
    void operator delete[](void *ptr) { yl_free(ptr); } \
    void *operator new(size_t, void *ptr) { return ptr; } \
    void operator delete(void *, void *) {} \
    void *operator new[](size_t, void *ptr) { return ptr; } \
    void operator delete[](void *, void *) {} \
private:

