#include "medea.hpp"
#include "medea.cpp"

struct A {
int a = 0;
};

MEDEA_DEFINE( A &it, (it.a));

struct B : A {
int b = 0;
};

MEDEA_DEFINE( B &it, (it.a, it.b)); //should be (it.b) only!!

#include <iostream>

int main() {
B b, c;
b.a = 1;
b.b = 2;
std::cout << medea::to_json( b ) << std::endl;
medea::from_json( c, medea::to_json( b ) );
b = B();
std::cout << medea::to_json( b ) << std::endl;
std::cout << medea::to_json( c ) << std::endl;
}