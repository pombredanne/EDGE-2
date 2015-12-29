// [ref] http://stackoverflow.com/questions/15249465/c-dynamically-load-arbitrary-function-from-dll-into-stdfunction
// g++ -shared -o test.dll test.cc -std=c++11
// cl test.cc /D_USRDLL /D_WINDLL /MD /link /DLL /OUT:test.dll

#ifndef DLL_HEADER
#define DLL_HEADER

#ifdef _WIN32
#  ifdef __cplusplus
#    define EXPORT extern "C" __declspec(dllexport)
#  else
#    define EXPORT            __declspec(dllexport)
#  endif
#else
#  define EXPORT
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
std::function<T> dll(const std::string &dllName, const std::string &funcName) {
    // Load DLL.
#ifdef _WIN32
    auto dll = LoadLibraryA( dllName.c_str() );
#else
    auto dll = dlopen( dllName.c_str(), RTLD_LAZY );
#endif

    // Check if DLL is loaded.
    if (0 == dll) {
        std::cerr << "Could not load DLL \"" << dllName << "\"" << std::endl;
        return std::function<T>();
    }

    // Locate function in DLL.
#ifdef _WIN32
    auto pid = GetProcAddress( dll, funcName.c_str() );
#else
    auto pid = dlsym( dll, funcName.c_str() );
#endif

    // Check if function was located.
    if (0 == pid) {
        std::cerr << "Could not locate the function \"" << funcName << "\" in DLL \"" << dllName << "\"" << std::endl;
        return std::function<T>();
    }

    // Create function object from function pointer.
    return TypeParser<T>::createFunction(pid);
}

template <typename T>
T sym(const std::string &dllName, const std::string &funcName) {
    // Load DLL.
#ifdef _WIN32
    auto dll = LoadLibraryA( dllName.c_str() );
#else
    auto dll = dlopen( dllName.c_str(), RTLD_LAZY );
#endif

    // Check if DLL is loaded.
    if (0 == dll) {
        std::cerr << "Could not load DLL \"" << dllName << "\"" << std::endl;
        return (T)0;
    }

    // Locate function in DLL.
#ifdef _WIN32
    auto pid = GetProcAddress( dll, funcName.c_str() );
#else
    auto pid = dlsym( dll, funcName.c_str() );
#endif

    // Check if function was located.
    if (0 == pid) {
        std::cerr << "Could not locate the function \"" << funcName << "\" in DLL \"" << dllName << "\"" << std::endl;
        return (T)0;
    }

    // Create function object from function pointer.
    return reinterpret_cast<T>(pid);
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
