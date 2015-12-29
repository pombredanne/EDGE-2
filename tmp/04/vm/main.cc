#include "vm.hpp"

int main() {
    lua_vm vm;
    vm.eval("print \"Hello from Lua\"");
}

