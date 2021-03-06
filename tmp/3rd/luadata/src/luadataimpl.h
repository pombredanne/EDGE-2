#ifndef _LUADATAIMPL_H_
#define _LUADATAIMPL_H_

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sys/types.h>
#include <sys/stat.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "luadata.h"

namespace luadata { namespace impl {;

/**
 * Implementation of the luadata library.
 */
class luadataimpl {
	/** Lua state pointer. */
	lua_State *L;

	/** A vector of everything defined in the global table before anything was loaded. */
	std::vector<luakey> _globalKeys;

	/** Lists of loaded files. */
	std::vector<std::pair<std::string, time_t>> _loadedFiles;

	/** Gets incremented every time a script gets executed. */
	int _cacheStateIndex;

	/** Cache for the global data tree keys. */
	std::shared_ptr<std::vector<std::pair<luakey, luavalue>>> _globalKeysCache;

	/** Validity state for the global data tree keys cache. */
	int _globalKeysCacheState;

public:
	/** The constructor creates a new Lua state. */
	luadataimpl();

	/** The destructor releases the Lua state. */
	~luadataimpl();

	/** Loads a Lua file (source code or precompiled). */
	bool loadfile(const std::string &path);

	/** Loads a string containing Lua code. */
	bool loadcode(const std::string &code);

	/** Do the hotreloading if needed. */
	void hotreload(bool force);

	/** Gets the value of a Lua variable. */
	double retrievedouble(const luapath &valuepath, const double &defaultValue);
	double retrievedouble(const luapath &valuepath);
	std::ptrdiff_t retrieveint(const luapath &valuepath, const std::ptrdiff_t &defaultValue);
	std::ptrdiff_t retrieveint(const luapath &valuepath);
	std::string retrievestring(const luapath &valuepath, const std::string &defaultValue);
	std::string retrievestring(const luapath &valuepath);
	bool retrievebool(const luapath &valuepath, const bool &defaultValue);
	bool retrievebool(const luapath &valuepath);

	/** Gets the type of a Lua variable. */
	luatype type(const luapath &valuepath);

	/** Get the length of a Lua table, or 0 if it's not a table. */
	std::ptrdiff_t tablelen(const luapath &valuepath);

	/** Gets a list of keys in the global table. */
	std::vector<luakey> tablekeys();

	/** Gets a list of keys of a Lua associative table. */
	std::vector<luakey> tablekeys(const luapath &valuepath);

	/** Iterator access to keys of a Lua associative table. */
	std::vector<std::pair<luakey, luavalue>>::const_iterator tablebegin(luapath &valuepath);
	std::vector<std::pair<luakey, luavalue>>::const_iterator tableend(luapath &valuepath);

	/** Iterator access to keys in the global table. */
	std::vector<std::pair<luakey, luavalue>>::const_iterator tablebegin();
	std::vector<std::pair<luakey, luavalue>>::const_iterator tableend();

private:
	/** Process a loaded chunk. */
	bool processloadedchunk();

	/** Puts a value from a path at the front of the stack, returns the number of pushed elements. */
	inline void getpath(const luapath &valuepath);

	/** Calls the function at the top of the stack with the given arguments. */
	inline void callfunction(const std::vector<luaarg> &args);

	/** Returns the keys of the table on the top of the stack. */
	inline std::vector<luakey> fetchtablekeys();

	/** Updates keys cache. */
	inline void updatekeyscache(luapath &valuepath);

	/** Updates the global keys cache. */
	inline void updateglobalkeyscache();

	/** Gets a value from the top of the stack. */
	double getdoublefromstack(const double &defaultValue);
	std::ptrdiff_t getintfromstack(const std::ptrdiff_t &defaultValue);
	std::string getstringfromstack(const std::string &defaultValue);
	bool getboolfromstack(const bool &defaultValue);
};

/** Last modification date of the file. */
time_t getmodtime(const std::string &filepath);

}} // namespaces

#endif // _LUADATAIMPL_H_
