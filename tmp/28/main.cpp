#include <iostream>
#include "fakebar.hpp"

#include <hyde/hyde.hpp>

int main() {
	hyde::windows::keyboard k(0);
	fakebar bar;

	while( bar.progress() < 100 ) {
		std::cout << bar.progress() << "%               \r";

		k.update();
		if( k.space.trigger() )  bar.push();
		if( k.enter.trigger() )  bar.progress( bar.progress() + 10 );
		if( k.escape.trigger() ) bar.finish();
	}

	std::cout << bar.progress() << "%             " << std::endl;
}
