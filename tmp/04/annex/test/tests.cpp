///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include "plugin.hpp"
#include <assert.h>

void plugin_load_unload()
{
    annex::plugin<my_plugin_interface> p1("plugin1.dll");
}

void multiple_plugin_load_unload()
{
    annex::plugin<my_plugin_interface> p1("plugin1.dll");
    annex::plugin<my_plugin_interface> p2("plugin2.dll");
}

void multiple_plugin_calls()
{
    annex::plugin<my_plugin_interface> p1("plugin1.dll");
    annex::plugin<my_plugin_interface> p2("plugin2.dll");

    assert( p1->function_1() == plugin1::function_1_result );
    assert( p1->function_2() == plugin1::function_2_result );

    assert( p2->function_1() == plugin2::function_1_result );
    assert( p2->function_2() == plugin2::function_2_result );
}

int main() {
    plugin_load_unload();
    multiple_plugin_load_unload();
    multiple_plugin_calls();
}
