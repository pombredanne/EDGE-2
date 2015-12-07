// C++ interface for the whereami C library
// - rlyeh, public domain

#pragma once
#include <string>

extern "C" int wai_getExecutablePath(char*, int, int*);
extern "C" int wai_getModulePath(char*, int, int*);

inline std::string get_app_path() {
	int length = wai_getExecutablePath(0, 0, 0);
	std::string path;
	if( length > 0 ) {
		path.resize( length );
		wai_getExecutablePath(&path[0], length, 0);
	}
	return path;
}

inline std::string get_module_path() {
	int length = wai_getModulePath(0, 0, 0);
	std::string path;
	if( length > 0 ) {
		path.resize( length );
		wai_getModulePath(&path[0], length, 0);
	}
	return path;
}

#ifdef MODULE_BUILD_DEMO

#include <iostream>

int main() {
	std::cout << get_app_path() << std::endl;
	std::cout << get_module_path() << std::endl;
}

#endif