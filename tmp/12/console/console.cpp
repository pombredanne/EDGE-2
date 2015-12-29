#include "eve.hpp"

#include <iostream>
#include <functional>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

#include "../string/eval/eval.hpp"

namespace eve {

    eve::vars globals;

    void console( const std::string &title, const std::string &prompt, const std::map< eve::string, std::function<void(const eve::strings &)> > &cb ) {

        // single instance
        static bool once = false;
        if( once ) {
            return;
        } else {
            once = true;
        }

#       ifdef _WIN32
        auto FatalExit0 = [](){
            FreeConsole();
            FatalExit(0);
        };
        // detach console
        FreeConsole();
        // popup console
        AllocConsole();
        // clean exit when std::cin is blocking app from quit
        atexit( FatalExit0 );
#       endif

        // threaded implementation
        using handlers = std::map< eve::string, std::function<void(const eve::strings &)> >;
        std::thread( [=]( handlers callbacks ) {

            // print usage
            //eve::string info( "Welcome to \2! Syntax: {\n\1\n}\n", eve::str1( callbacks, "\1," ), EVE_VERSION_FULL );
            eve::string info( title, eve::str1( callbacks, "\1," ), EVE_VERSION_FULL );
            std::clog << info << std::endl;

            for( ;; ) {
                dr::print( DR_GREEN, prompt );

                eve::string input;
                std::getline( std::cin, input );
                if( input.empty() ) continue;

                eve::strings tokens = input.split("=");
                bool ok = false;
                /**/ if( tokens.size() == 3 ) {
                    globals[ tokens[0] ] = eval( tokens[2] );
                    ok = true;
                }
                else if( tokens.size() == 1 && globals.find(tokens[0]) != globals.end() ) {
                    std::clog << globals.find(tokens[0])->second << std::endl;
                    ok = true;
                }
                if( !ok ) {
                    eve::strings args = eve::string(tokens[0]).tokenize("(),");
                    eve::string function = args.front();
                    args.pop_front();

                    for( auto &arg : args ) {
                        arg = globals.lookup( arg );
                    }

                    auto found = callbacks.find(function);
                    if( found == callbacks.end() ) {
                        std::clog << "? " << function << "(" << args.str() << ")" << std::endl;
                    } else {
                        found->second( args );
                    }
                }
            }
        }, cb ).detach();
    }
}

