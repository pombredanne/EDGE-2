// file/folder monitoring
// - rlyeh, public domain

#pragma once
#include <apathy/apathy.hpp>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <functional>

// public api

struct watchdog {
    double seconds;
    std::string folder;
    std::function<void(watchdog &)> callback;
    std::vector<std::string> previous, add, del, modif;

    void operator()();
    void reload( double seconds );
};

void watch( const std::string &uri, const std::function<void(watchdog &)> &callback );

// api details below

inline void watch( const std::string &uri, const std::function<void(watchdog &)> &callback ) {
    watchdog wt;
    wt.folder = uri;
    wt.callback = callback;
    wt.reload( 0 );
}

inline void watchdog::operator()() {
    std::chrono::microseconds duration( (int)(seconds * 1000000) );
    std::this_thread::sleep_for( duration );

    add = del = modif = std::vector<std::string>();
    std::vector<std::string> copy = previous;

    for( auto &file : previous ) {
        if( apathy::exists(file) ) {
            if( apathy::touched(file) ) {
                modif.push_back( file );
            }
        } else {
            del.push_back( file );
            copy.erase( std::find( copy.begin(), copy.end(), file ) );
        }
    }

    std::vector<std::string> now;
    if( apathy::lsr( now, folder, "*" ) )
    for( auto &file : now ) {
        if( std::find( copy.begin(), copy.end(), file ) == copy.end() ) {
            add.push_back( file );
            copy.push_back( file );
        }
    }

    previous = copy;
    callback( *this );
}

inline void watchdog::reload( double seconds ) {
    this->seconds = seconds;
    std::thread( *this ).detach();
}

#ifdef WATCHDOG_BUILD_TESTS
#include <iostream>

int main() {
    watch( "./", [&]( watchdog &wd ) {
        for( auto &item : wd.add ) {
            std::cout << "add: \t" << item << std::endl;
        }
        for( auto &item : wd.del ) {
            std::cout << "del: \t" << item << std::endl;
        }
        for( auto &item : wd.modif ) {
            std::cout << "modif: \t" << item << std::endl;
        }
        wd.reload( 2.0 );
    } );
    for(;;) {
        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
}

#endif
