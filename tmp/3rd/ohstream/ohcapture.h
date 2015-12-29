#include <iostream>
#include <vector>
#include <string>

#include <stdio.h>
#ifdef _MSC_VER
#include <io.h>
#else
#define _dup dup
#define _dup2 dup2
#define _close close
#define _fileno fileno
#endif

namespace {
    // taken from https://github.com/r-lyeh/wire {
    std::vector< std::string > tokenize( const std::string &self, const std::string &delimiters ) {
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
    // }
    struct capture {
        int fd, old, ch;
        capture( int ch, FILE *fp ) : ch(ch) {
            old = _dup( ch );
            fd = _fileno( fp );
            _dup2( fd, ch );
        }
        ~capture() {
            _dup2( old, ch );
        }
    };
}

#define CAPTURE(log, ...) do { \
    { \
        FILE *fp1 = tmpfile(), *fp2 = tmpfile(); \
        { \
            capture c1( 1, fp1 ), c2( 2, fp2 ); \
            __VA_ARGS__; \
        } \
        { \
            rewind(fp1); rewind(fp2); \
            std::string ss1, ss2; \
            do { ss1 += fgetc(fp1); } while (!feof(fp1)); \
            do { ss2 += fgetc(fp2); } while (!feof(fp2)); \
            ss1.resize( ss1.size() - 1 ); \
            ss2.resize( ss2.size() - 1 ); \
            std::string clean; \
            for( auto &line : tokenize( log + ss1 + ss2, "\r\n") ) { \
                clean += std::string("       ") + line + "\n"; \
            } \
            log = clean; \
        } \
        fclose( fp1 ); \
        fclose( fp2 ); \
    } \
    if( log.size() ) { \
    std::clog << " log following:"; \
    } \
    std::clog << (ok ? "\r[ OK" : "\r[FAIL"); \
    std::clog << std::endl; \
    std::clog << log; \
} while(0)

/*
// Usage:
std::clog << "[    ] setup '" << ring.name << "' ...";
std::string log;
CAPTURE(log,
    if( ring.init ) {
        ok &= ring.init();
        if( ok ) {
            unmap.push_back( ring );
        }
    }
);
*/
