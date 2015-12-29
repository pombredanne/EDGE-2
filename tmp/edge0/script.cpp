#include "script.hpp"
#include "logger.hpp"
#include "engine.hpp"

#include <iostream>
#include <sstream>
#include <cassert>


extern "C" int luaopen_lpeg(lua_State *L);
extern "C" int luaopen_lfs (lua_State *L);

extern "C" {

static
int traceback(lua_State *L)
{
  if (!lua_isstring(L, 1)) { /* Non-string error object? Try metamethod. */
    if (lua_isnoneornil(L, 1) ||
    !luaL_callmeta(L, 1, "__tostring") ||
    !lua_isstring(L, -1))
      return 1;  /* Return non-string error object. */
    lua_remove(L, 1);  /* Replace object by result of __tostring metamethod. */
  }
  luaL_traceback(L, L, lua_tostring(L, 1), 1);
  return 1;
}

static
void report_errors(lua_State *L, int status) {
  if (status != 0) {
    // print error message (including callstack)
    ee::log->error( "-- %s", lua_tostring(L, -1) );
    // remove error message
    lua_pop(L, 1); 
  }
}

static int safecall(lua_State *L, int narg, int clear)
{
  int status;
  int base = lua_gettop(L) - narg;  /* function index */
  lua_pushcfunction(L, traceback);  /* push traceback function */
  lua_insert(L, base);  /* put it under chunk and args */
#if !LJ_TARGET_CONSOLE
  //signal(SIGINT, laction);
#endif
  status = lua_pcall(L, narg, (clear ? 0 : LUA_MULTRET), base);
  report_errors(L, status);
#if !LJ_TARGET_CONSOLE
  //signal(SIGINT, SIG_DFL);
#endif
  lua_remove(L, base);  /* remove traceback function */
  /* force a complete garbage collection in case of errors */
  if (status != 0) lua_gc(L, LUA_GCCOLLECT, 0);
  return status;
}

static void fastcall(lua_State *L, int narg, int clear) {
    lua_call(L, 0, 0);
}

}


namespace ee {

script::script() {
    state = lua_newstate( [](void *ud, void *ptr, size_t osize, size_t nsize) -> void* {
        if( ptr == 0 ) {
            return malloc( (osize+1) * nsize );
        } else {
            ptr = realloc( ptr, (osize+1) * nsize );
        }
        return ptr;
    }, 0 );

    luaL_openlibs( state );
    //luaopen_lpeg( state );
    //luaopen_lfs( state );
}

script::~script() {
    lua_close( state );
}

void script::bind_function(const std::string &name, lua_CFunction function ) {
    lua_pushcfunction( state, function );
    lua_setglobal( state, name.c_str() );
}

void script::bind_class(const std::string &class_name, const std::map<std::string, lua_CFunction> &functions) {
    lua_newtable( state );

    for( auto &f : functions ) {
        lua_pushcfunction( state, f.second );
        lua_setfield( state, 1, f.first.c_str() );
    }

    lua_setglobal( state, class_name.c_str() );
}

bool script::load_file(const std::string &file) {
    ee::log->dev( "Loading: %s ...", file.c_str() );
    int loadResult = luaL_loadfile( state, file.c_str() );

    /**/ if( loadResult == LUA_OK ) {
        safecall( state, 0, 1 );
        return true;
    }
    else if( loadResult == LUA_ERRSYNTAX ) {
        ee::log->error("Failed to load (LUA_ERRSYNTAX, '%s'): %s", lua_tostring( state, 1 ), file );
    }
    else if( loadResult == LUA_ERRMEM ) {
        ee::log->error("Failed to load (LUA_ERRMEM): %s", file);
    }
    else {
        ee::log->error("Failed to load (%d): %s", file );
    }

    return false;
}

void script::update() {
    lua_getglobal( state, "update" );
    safecall( state, 0, 1 );
}
void script::render() {
    lua_getglobal( state, "render" );
    safecall( state, 0, 1 );
}
void script::debug() {
    lua_getglobal( state, "debug" );
    safecall( state, 0, 1 );
}

}
