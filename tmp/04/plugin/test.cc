#include "test.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string.h>

//
// Test Plugin
//

PLUGIN_REGISTER(TestPlugin, plugin::IPlugin, "0.1.1", "r-lyeh", "unlicense", "")

TestPlugin::TestPlugin() {
	std::cout << "TestPlugin: Create" << std::endl;
}

TestPlugin::~TestPlugin() {
	std::cout << "TestPlugin: Destroy" << std::endl;
}

bool TestPlugin::rpc( int argc, const void **argv ) {
    const char *opcode = (const char *)argv[0]; argc--; argv++;

	std::cout << "TestPlugin: Command: " << opcode << std::endl;

    // Handle a JSON encoded options hash
    if ( argc == 1 && 0 == strcmp(opcode, "options:set") ) {
    }
    // Handle raw file data
    else if (argc == 2 && 0 == strcmp(opcode, "file:write") ) {
        const char *data = (const char *)argv[0];
        int size = (int)argv[1];
        std::string path("test.bin");
        std::ofstream ofs(path, std::ios::out|std::ios::binary);
        if (!ofs.is_open()) {
            _error = "Cannot write to output file: " + path;
            return false;
        }
        ofs.write(data, size);
        ofs.close();
    }
    else {
        // Handle unknown commands
        _error = "Unknown command";
        return false;
    }

	return true;
}

void TestPlugin::setValue(const char* value) {
	std::cout << "TestPlugin: Set value: " << value << std::endl;
	_value = value;
}

const char* TestPlugin::cValue() const {
	return _value.c_str();
}

const char* TestPlugin::error() const {
	return _error.empty() ? nullptr : _error.c_str();
}

//
// Static Methods
//

PLUGIN_EXTERN PLUGIN_EXPORT
int gimmeFive();

int gimmeFive() {
	return 5;
}
