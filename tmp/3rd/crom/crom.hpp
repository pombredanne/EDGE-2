// yob

#pragma once
#include <stdio.h>
#include <vector>
#include <map>
#include <set>

#include "../unify/unify.hpp"
#include "../apathy/apathy.hpp"

// disk
// mmap
// http
// zipf
// tarf

namespace crom {
    typedef const char cstr;

    struct entry {
        size_t offset, size; // entry in archive
    };

    struct archive {
        int id;
        int fields; // 2
        std::vector< entry > name;
        std::vector< entry > data;
    };

    struct filesystem {
        virtual ~filesystem() {}

        virtual cstr*  id() = 0;
        virtual int    semver() = 0;

        virtual void   toggle( const char *pathmask, bool onoff ) = 0;
        virtual void   scan() = 0;

        virtual void   preload( const char *pathmask ) = 0;
        virtual void   purge( const char *pathmask ) = 0;

        virtual cstr*  find( const char *filename ) = 0;
        virtual void*  read( const char *filename ) = 0;
        virtual size_t size( const char *filename ) = 0;

        // brothers(filename)
        // parent(filename)
        virtual std::string toc() = 0;
    };

    struct vfs : public std::vector<filesystem *> {

        void scan();
        std::string toc();

        void preload( const char *pathmask );
        void   purge( const char *pathmask );

        filesystem* find( const char *filename );

        void*  read( const char *filename );
        size_t size( const char *filename );
    };
}

// utils 

bool pathmask(const std::string &pm, std::string &path, std::string &mask);
std::string get_protocol(const std::string &virt);
std::string get_pathfile(const std::string &virt);

#ifdef CROM_BUILD_TESTS

#include "../apathy/apathy.hpp"

struct stdio_filesystem : public crom::filesystem {
    stdio_filesystem() {}

    // mounted paths
    std::map< std::string, bool > paths;
    std::set< std::string >       mirror;
    // virt2phys table lookup
    std::map< std::string, std::string > alias;
    // cache lookup
    std::map< std::string, std::string > cache;

    const char *id() {
        return "stdio-filesystem";
    }
    int semver() {
        return 100;
    }

    void toggle( const char *pathmask, bool onoff ) {
        paths[ pathmask ] = onoff;
    }

    void scan() {

        // clear previous results
        alias.clear();
        cache.clear();
        // iterate mounted dirs
        for( auto &dir : paths ) {
            bool enabled = dir.second;
            if( enabled ) {
                // scan active dirs
                std::string path, mask;
                bool recurse = pathmask( dir.first, path, mask );

                std::vector<std::string> globbed;
                apathy::glob( globbed, path, mask, recurse, false );

                // inscribe results
                for( auto &file : globbed ) {
                    bool is_file = file.back() != '/';
                    if( is_file ) {
                        alias[ unify( file ) ] = file;
                    }
                }
            }
        }
    }

    std::string toc() {
        std::string out;
        for( auto &entry : alias ) { // phys -> virt
            out += "\t" + entry.second + " -> " + entry.first + "\n";
        }
        return out;
    }

    const char *find( const char *virt ) {
        auto uid = unify( virt );
        auto found = alias.find( uid );
        if( found == alias.end() ) {
            return "";
        }
        return found->second.c_str();
    }

    auto preload( const char *filemask ) {
        // @todo: wildcard
        auto uid = unify( filemask );
        auto phys = alias[ uid ];
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
        auto phys = alias[ unify( filename ) ];
        return apathy::size( phys );
    }
};

#include <iostream>

int main() {
    crom::vfs vfs;

    auto stdfs = new stdio_filesystem;
    stdfs->toggle( "**.*pp", true );

    vfs.push_back( stdfs );
    vfs.scan();

    std::cout << vfs.toc() << std::endl;
}

#endif

// vfs

inline void crom::vfs::scan() {
    for( auto &fs : *this ) {
        fs->scan();
    }
}
inline std::string crom::vfs::toc() {
    std::string t;
    for( auto &fs : *this ) {
        t = t + fs->id() + " v" + std::to_string(fs->semver()) + " {\n";
        t = t + fs->toc() + "}\n";
    }
    return t;
}

inline void crom::vfs::preload( const char *pathmask ) {
    for( auto &fs : *this ) {
        fs->preload( pathmask );
    }
}

inline void crom::vfs::purge( const char *pathmask ) {
    for( auto &fs : *this ) {
        fs->purge( pathmask );
    }
}

inline crom::filesystem* crom::vfs::find( const char *filename ) {
    for( auto &fs : *this ) {
        if( fs->find( filename ) ) {
            return fs;
        }
    }
    return 0;
}

inline void* crom::vfs::read( const char *filename ) {
    auto *fs = find( filename );
    return fs ? fs->read( filename ) : 0;
}
inline size_t crom::vfs::size( const char *filename ) {
    auto *fs = find( filename );
    return fs ? fs->size( filename ) : 0;
}

// utils

inline bool pathmask(const std::string &pm, std::string &path, std::string &mask) {
    auto pos = pm.find_first_of('*');
    mask = pos == std::string::npos ? std::string() : pm.substr( pos );
    path = apathy::replace(pm, mask, "");
    pos = path.find_last_of('/');
    if (pos != std::string::npos) {
        mask = path.substr(pos + 1) + mask;
        path.resize(pos + 1);
    }
    if (mask.empty()) mask = "*";
    if (path.empty()) path = ".";
    if (path.back() != '/') path += "/";
    bool recurse_subdirs = (mask.find("**") != std::string::npos);
    return recurse_subdirs;
}

inline std::string get_protocol(const std::string &virt) {
    std::string proto = virt.substr(0, apathy::replace(virt, "://", "\1").find_first_of('\1'));
    return proto != virt ? proto + "://" : std::string();
}

inline std::string get_pathfile(const std::string &virt) {
    auto proto = get_protocol(virt);
    std::string pf = virt.substr(proto.size());
    return pf;
}
