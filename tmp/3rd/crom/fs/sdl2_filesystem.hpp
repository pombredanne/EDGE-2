#ifndef EE_SDL2_FILESYSTEM_HPP
#define EE_SDL2_FILESYSTEM_HPP

#include "filesystem.hpp"

#include <stdio.h>
#include <string>


struct sdl2_filesystem : public ee::filesystem {
    sdl2_filesystem() {}

    // mounted paths
    std::map< std::string, bool > paths;
    // virt2phys table lookup
    std::map< std::string, std::string > table;
    // cache lookup
    std::map< std::string, std::string > cache;

    void mount( const char *filemask ) {
        paths[ filemask ] = true;
    }
    void unmount( const char *filemask ) {
        paths[ filemask ] = false;
    }

    void scan() {
        // clear previous results
        $bench2(
            table.clear();
            cache.clear();
        );
        // create list of masks
        std::vector<std::string> masks;
        $bench2( 
            for( auto &dir : paths ) {
                if( dir.second ) {
                    masks.push_back( dir.first );
                }
            }
        );
        // scan mounted dirs
        std::vector<std::string> fs;
        $bench2( 
            apathy::glob( fs, "", masks, true, true );
        );
        // inscribe results
        $bench2(
            for( auto &file : fs ) {
                bool is_file = file.back() != '/';
                if( is_file ) {
                    table[ unify( file ) ] = file;
                }
            }
        );
    }

    const char *find( const char *virt ) {
        auto uid = unify( virt );
        auto found = table.find( uid );
        if( found == table.end() ) {
            return "";
        }
        return found->second.c_str();
    }

    auto preload( const char *filemask ) {
        // @todo: wildcard
        auto uid = unify( filemask );
        auto phys = table[ uid ];
        auto found = cache.find( uid );
        if( found == cache.end() ) {
            //std::cout << "read from disk... " << phys << std::endl;
            std::pair<std::string,std::string> p { uid, apathy::read( phys ) };
            found = cache.insert( p ).first;
        } else {
            //std::cout << "cached from disk... " << phys << std::endl;
        }
        return found;
    }
    void purge( const char *filemask ) {
        // @todo: wildcard
        // @todo: return number of purged items
        //std::cout << "purged: " << filemask << std::endl;
        auto uid = unify( filemask );
        cache[ uid ];
        cache.erase( uid );
    }

    void *read( const char *filename ) {
        return &preload( filename )->second[0];
    }
    size_t size( const char *filename ) {
        auto phys = table[ unify( filename ) ];
        return apathy::size( phys );
    }
};

#endif
