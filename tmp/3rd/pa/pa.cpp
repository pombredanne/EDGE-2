#include "pa.hpp"
#include <iostream>

int main() {
	pa<int17_t> p( 1000 );

	p[1] = 4;
	p[2] = 5;
	( p[3] = 6 ) = 7;

	std::cout << p[0] << ',' << p[1] << ',' << p[2] << ',' << p[3] << std::endl;
}