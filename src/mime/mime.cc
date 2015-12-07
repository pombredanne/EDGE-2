#include "mime.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

int main( int argc, const char **argv ) {
	if( argc > 1 ) {
		std::ifstream ifs( argv[1], std::ios::binary );
		std::stringstream ss;
		ss << ifs.rdbuf();
		std::string data = ss.str();
		const unsigned char *data8 = (const unsigned char*)data.c_str();
		std::string ext, mime;
		bool ok = detect_mime( data8, data.size(), ext, mime );
		std::cout << ok << ',' << ext << ',' << mime << std::endl;
	}
}
