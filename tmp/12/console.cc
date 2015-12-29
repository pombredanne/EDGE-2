#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> tokenize( const std::string &self, const std::string &delimiters ) {
    std::string map( 256, '\0' );
    for( const unsigned char &ch : delimiters )
    map[ ch ] = '\1';
    std::vector< std::string > tokens(1);
    for( const unsigned char &ch : self ) {
        /**/ if( !map.at(ch)          ) tokens.back().push_back( char(ch) );
        else if( tokens.back().size() ) tokens.push_back( std::string() );
    }
    while( tokens.size() && !tokens.back().size() ) tokens.pop_back();
    return tokens;
}

#include <sstream>
template<typename T>
std::string str( const T &t, const std::string &delim ) {
    std::string sep;
    std::stringstream ss;
    for(auto &it : t ) {
        ss << sep << it;
        sep = delim;
    }
    return ss.str();
}

#include <map>
#include <functional>

#include "eval/eval.hpp"
#include "eval/eval.cpp"
#include "eval/calculate.cpp"

std::map< std::string, std::string > globals;

#ifdef _WIN32
#include <windows.h>
#endif

#include <iomanip>

int main() {

#   ifdef _WIN32
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
#   endif

    config_eval();

    std::string input;
    while (std::cout << "> ", getline(std::cin, input))
    {
        if( input.empty() ) {
            continue;
        }

        std::string err;
        auto value = eval(input, &err);
        if( err.empty() ) {
            std::cout << std::setprecision(std::numeric_limits<decltype(value)>::digits10) << value << std::endl;
        } else {

            auto tokens = tokenize(input, "=");
            bool ok = false;
            /**/ if( tokens.size() == 2 ) {
                globals[ tokens[0] ] = std::to_string( eval( tokens[1] ) );
                ok = true;
            }
            else if( tokens.size() == 1 && globals.find(tokens[0]) != globals.end() ) {
                std::cout << globals.find(tokens[0])->second << std::endl;
                ok = true;
            }

            if(!ok)
            std::cout << err << std::endl;
        }

    }
}
