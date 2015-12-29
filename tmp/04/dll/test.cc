// test.cc (test.dll)
#include "dll.hpp"
#include <iostream>

EXPORT
int plusFive(int value) {
    return value + 5;
}

EXPORT
void printHello() {
    std::cout << "Hello from C++!" << std::endl;
}
