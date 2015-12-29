// main.cc
#include <iostream>
#include "dll.hpp"

int main() {
    auto func1 = dll<int(int)>("test.dll", "plusFive");
    auto func2 = dll<void()>("test.dll", "printHello");
    std::cout << "Result of func1: " << func1(1) << std::endl;
    func2();
}
