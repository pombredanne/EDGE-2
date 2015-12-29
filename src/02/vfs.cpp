#include <3rd/journey/journey.hpp>
#include <3rd/apathy/apathy.hpp>
#include <3rd/unify/unify.hpp>
#include "../api.hpp"
#include <iostream>
#include <fstream>
#include <string>

namespace vfs {
    journey j;
    bool init() {
        std::cout << "[ OK ] setup: virtual filesystem" << std::endl;
        j = journey();
        std::string journal_app;
        std::string journal_alt;
        eeModuleGetApplicationPath(journal_app);
        journal_alt = journal_app.substr( 0, journal_app.find_last_of('.') ) + ".joy";
        j.init( journal_app );
        bool scan_app = 1, scan_res = 1, scan_usr = 1;
        if( j.load() ) {
            std::cout << "[ OK ] appended journal found " << std::endl;
            scan_app = scan_res = 0;
        } else {
            std::cout << "[ OK ] appended journal not found, using '" << journal_alt << "' instead..." << std::endl;
            j.init( journal_alt );
            j.load();
        }
        if( scan_app || scan_res || scan_usr ) {
            auto scan = [&]( const std::string &path ) {
                auto glob = apathy::lsr( path );
                for( auto &pathfile : glob ) {
                    bool is_file = pathfile.back() != '/';
                    bool is_dot = apathy::name(pathfile).front() == '.';
                    if( is_file && !is_dot ) {
                        auto stamp = apathy::mdate(pathfile);
                        j.append( unify(pathfile), apathy::read(pathfile).c_str(), apathy::size(pathfile), stamp );
                    }
                }
            };
            if( scan_app ) {
                scan( "app/" );
            }
            if( scan_res ) {
                scan( "res/" );
            }
            if( scan_usr ) {
                scan( "usr/" );
            }
            // reload whole toc
            j.load();

        }
#ifndef SHIPPING
        auto toc = j.get_toc();
        for( auto &item : toc ) {
            const auto &name = item.first;
            const auto &entry = item.second;
            std::cout << "[ OK ] journal found: " << name << " (" << entry.size << " bytes)" << std::endl;
        }
#endif
        // } vfs

        // test vfs
        auto welcome = j.read(unify("txt/welcome"));
        if( welcome.empty() ) {
            std::cout << "[FAIL] vfs" << std::endl;
        } else {
            std::cout << "[ OK ] vfs read: " << welcome << std::endl;
        }

        return welcome.empty() ? false : true;
    }
    bool quit() {
        std::cout << "[ OK ] teardown: virtual filesystem" << std::endl;
        return true;
    }
}
