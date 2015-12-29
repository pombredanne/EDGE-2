#pragma once

#include <3rd/mini/mini.hpp>
#include <3rd/wire/wire.hpp>

using cvars = mINI_basic<wire::string, wire::string>;

#ifdef CVARS_BUILD_DEMO

cvars cfg;

int main() {
    cfg["console.enabled"] = true;
    cfg["console.history"] = 300;
    std::cout << cfg.save() << std::endl;
}

#endif
