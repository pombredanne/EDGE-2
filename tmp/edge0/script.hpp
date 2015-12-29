#ifndef EE_SCRIPT_HPP
#define EE_SCRIPT_HPP

#include <string>
#include <map>
#include <lua.hpp>

namespace ee {

struct script {
    script();
    ~script();

    void bind_function(const std::string &name, lua_CFunction function);
    void bind_class(const std::string &class_name, const std::map<std::string, lua_CFunction> &functions);
    bool load_file(const std::string &scriptFile);
    void update();
    void render();
    void debug();

    // private
    lua_State *state;
};

}

#endif
