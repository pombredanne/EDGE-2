// test.cc (test.dll)
#include "dll.hpp"
#include <stdio.h>

EXPORT
int plusFive(int value) {
    return value + 5;
}

EXPORT
void printHello() {
    puts("Hello from C!");
}
