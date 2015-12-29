// simple lua evaluator
// - rlyeh
// to look: http://code.google.com/p/slb/

// original article by: fabrice lete 
// http://altdevblogaday.com/2011/02/19/lua-elevator-pitch-for-very-high-buildings/

#include <iostream>
#include <string>
#include <lua.hpp>
#include <string>
#include "vm.hpp"

extern "C" {
}

#include <fstream>
#include <sstream>

std::string load( const std::string &file ) {
    std::ifstream ifs(file.c_str(), std::ios::binary);
    std::stringstream ss; ss << ifs.rdbuf();
    return ss.str();
}

lua_vm::lua_vm() {
    L = luaL_newstate();

    luaL_openlibs( L );
}

lua_vm::~lua_vm() {
    lua_close( L );
}

std::string lua_vm::eval( const std::string &script ) {
    if( script.size() ) {
        int errors = luaL_dostring( L, script.c_str() );
        if( errors && !lua_isnil(L, -1)) {
            const char *msg = lua_tostring(L, -1);
            if (msg == NULL) msg = "(error object is not a string)";
            lua_pop(L, 1);
            return msg;
        }
    }
    return std::string();
}
