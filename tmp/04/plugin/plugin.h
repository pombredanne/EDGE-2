// [ref] http://stackoverflow.com/questions/15249465/c-dynamically-load-arbitrary-function-from-dll-into-stdfunction
// g++ -shared -o test.dll test.cc -std=c++11
// cl test.cc /D_USRDLL /D_WINDLL /MD /link /DLL /OUT:test.dll

#ifndef DLL_HEADER
#define DLL_HEADER

#ifdef __cplusplus
#  define PLUGIN_EXTERN extern "C"
#else
#  define PLUGIN_EXTERN
#endif

#ifdef _WIN32
#  ifdef __cplusplus
#    define PLUGIN_EXPORT __declspec(dllexport)
#  else
#    define PLUGIN_EXPORT __declspec(dllexport)
#  endif
#else
#  define   PLUGIN_EXPORT
#endif

#ifdef __cplusplus
#include <functional>
#include <string>
#include <iostream>
#include <stdlib.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <dlfcn.h>
#endif

template <typename T>
struct TypeParser {};

template <typename Ret, typename... Args>
struct TypeParser<Ret(Args...)> {
    template<typename FARPROC>
    static std::function<Ret(Args...)> createFunction(const FARPROC lpfnGetProcessID) {
        return std::function<Ret(Args...)>(reinterpret_cast<Ret (/*__stdcall*/ *)(Args...)>(lpfnGetProcessID));
    }
};

template <typename T>
T sym( const std::string &dll_name, const std::string &symbol ) {
    // Try to load DLL
#ifdef _WIN32
    auto dll = LoadLibraryA( dll_name.c_str() );
#else
    auto dll = dlopen( dll_name.c_str(), RTLD_LAZY );
#endif

    if( !dll ) {
        return (T)0;
    }

    // Locate function in DLL.
#ifdef _WIN32
    auto pid = GetProcAddress( dll, symbol.c_str() );
#else
    auto pid = dlsym( dll, symbol.c_str() );
#endif

    if( !pid ) {
        return (T)0;
    }

    // Cast address to type
    return reinterpret_cast<T>(pid);
}

template <typename T>
std::function<T> dll( const std::string &dll_name, const std::string &func_name ) {

#ifdef _WIN32
    using rettype = decltype( GetProcAddress( LoadLibraryA( 0 ), 0 ) );
#else
    using rettype = decltype( dlsym( dlopen( 0, RTLD_LAZY ), 0 ) );
#endif

    // Create function object from function pointer.
    auto ptr = sym<rettype>( dll_name, func_name );
    return ptr ? TypeParser<T>::createFunction(ptr) : std::function<T>();
}

namespace plugin {

// Forward declare the plugin class which must be defined externally.
class IPlugin;

// Define the API version.
// This value is incremented whenever there are ABI breaking changes.
#define PLUGIN_API_VERSION 100

// Define a type for the static function pointer.
PLUGIN_EXTERN typedef IPlugin* (*GetPluginFunc)();

// Plugin details structure that's exposed to the application.
struct PluginDetails {
    int apiVersion;
    const char* fileName;
    const char* className;
    const char* classBase;
    const char* pluginVersion;
    const char* pluginAuthor;
    const char* pluginLicense;
    const char* pluginDependencies;
    GetPluginFunc initializeFunc;
};

#define PLUGIN_REGISTER(className, classBase, version, author, license, deps)     \
extern "C" {                                               \
    PLUGIN_EXPORT plugin::IPlugin* getPlugin() {           \
        static className singleton;                        \
        return &singleton;                                 \
    }                                                      \
    PLUGIN_EXPORT plugin::PluginDetails exports = {        \
        PLUGIN_API_VERSION,                                \
        __FILE__,                                          \
        #className,                                        \
        #classBase,                                        \
        version,                                           \
        author,                                            \
        license,                                           \
        deps,                                              \
        getPlugin,                                         \
    };                                                     \
}

}

#endif

#endif

/* usage:
// test.cc (test.dll)
#include <iostream>
#include "dll.hpp"
EXPORT
int plusFive(int value) {
    return value + 5;
}
EXPORT
void printHello() {
    std::cout << "Hello!" << std::endl;
}
// main.cc
#include <iostream>
#include "dll.hpp"
int main() {
    auto func1 = dll<int(int)>("test.dll", "plusFive");
    auto func2 = dll<void()>("test.dll", "printHello");
    std::cout << "Result of func1: " << func1(1) << std::endl;
    func2();
}
*/
