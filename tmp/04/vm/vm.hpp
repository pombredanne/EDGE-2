#pragma once
#include <lua.hpp>
#include <string>

struct lua_vm {
    lua_State *L;

     lua_vm();
    ~lua_vm();

    std::string eval( const std::string &script );

    template<typename T>
    void extend( const T* func, const std::string &as_name ) {
        lua_pushcfunction( L, func );
        lua_setglobal( L, as_name.c_str() );
    }
};
