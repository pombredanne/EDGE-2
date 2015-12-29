#pragma once
#include <map>
#include <string>
#include <stdio.h>
#include <vector>
#include <deque>
#include "../../apathy/apathy.hpp"

bool pathmask( const std::string &pm, std::string &path, std::string &mask ) {
    mask = pm.substr(pm.find_first_of("*"));
    path = apathy::replace(pm, mask, "");
    if( path.find_last_of('/') != std::string::npos ) path.resize( path.find_last_of('/') );
    if( mask.empty() ) mask = "*";
    if (path.empty()) path = "."; path += "/";
    bool recurse_subdirs = (mask.find("**") != std::string::npos);
    return recurse_subdirs;
}

inline std::string get_protocol(const std::string &virt) {
    std::string proto = virt.substr(0, apathy::replace(virt, "://", "\1").find_first_of('\1'));
    return proto != virt ? proto + "://" : std::string();
}

inline std::string get_pathfile(const std::string &virt) {
    auto proto = get_protocol(virt);
    std::string pf = virt.substr( proto.size() );
    return pf;
}

// table of contents

struct entry {
    size_t size, offset; // size and offset in the container (if any)
};

typedef std::map< std::string, std::string > mapping;
typedef std::map< std::string /* phys */, entry > table_of_contents;

struct fs {
    // virt -> phys table
    mapping table;
    // phys -> size, offset
    table_of_contents toc;
    // supported protocols
    std::vector<std::string> protocols;

    virtual ~fs() {}
    virtual std::string name() const = 0;
    virtual std::string read( const std::string &phys, size_t size = ~0, size_t offset = 0 ) const = 0;
    virtual table_of_contents scan() = 0;
    virtual void touch( const std::string &phys ) = 0;
    virtual void refresh() = 0;

    virtual bool mount( const std::string &phys, const std::string &virt );
    virtual bool supported( const std::string &virt ) const;

    virtual std::string translate( const std::string &virt ) const;

    virtual bool exists( const std::string &virt ) const;
    virtual bool symlink( const std::string &from_virt, const std::string &to_virt );
    virtual bool rename( const std::string &virt_mask, const std::string &as_virt );

    virtual std::string head() const;
    virtual std::string print() const;
};

struct std_io_fs : public fs {
    std::string path, mask;
    std_io_fs( const std::string &pathmask ) {
        ::pathmask( pathmask, path, mask );
        refresh();
    }
    std::string name() const {
        return "stdio";
    }
    std::vector<std::string> protocols() const {
        std::vector<std::string> p;
        p.push_back("file://");
        p.push_back("");
        return p;
    }
    std::string read( const std::string &phys, size_t size = ~0, size_t offset = 0 ) const {
        return apathy::read( phys );
    }
    table_of_contents scan() {
        return toc;
    }
    void touch( const std::string &phys ) {
        apathy::touch( phys );
    }
    void refresh() {
        auto files = apathy::lsr( path, mask );
        toc.clear();
        for( auto &file : files ) {
            toc[ file ].offset = 0;
            toc[ file ].size = apathy::size( file );
        }
    }
};

//

class vfs {
    vfs( const vfs & );
    vfs &operator=( const vfs & );

    std::deque< fs * > all;
    fs *locate( const std::string &virt ) const;

public:

     vfs();
    ~vfs();

    bool add_fs( fs *f, const std::vector<std::string> &protocols ) {
        all.push_front( f );
        all.front()->protocols = protocols;
        return true;
    }

    bool mount( const std::string &phys, const std::string &virt );

    bool exists( const std::string &virt ) const;

    std::string read( const std::string &virt ) const;

    // construct a valid resource, or a placeholder at least
    template<typename T>
    bool make( T &t, const std::string &virt ) const {
        // construct a valid resource, or a placeholder at least
        std::string data = read( virt );
        if( !data.empty() ) {
            // T() construct
            t = T( (const void *)data.c_str(), (size_t)data.size() );
            return true;
        } else {
            // T() might construct a placeholder here
            t = T( (const void *)0, (size_t)0 );
            return false;
        }
    }

    template<typename T>
    T make( const std::string &virt ) const {
        // construct a valid resource, or a placeholder at least
        T t;
        make( t, virt );
        return t;
    }

    std::string toc() const;
};

 // table of contents

inline bool fs::mount( const std::string &phys, const std::string &virt ) {
    std::string mask, path;
    pathmask( phys, path, mask );

    auto mount = [&]( const std::string &phys, const std::string &virt_, const std::string &mask ) {
        std::string virt(virt_);
        while( virt.size() && virt.front() == '/' ) virt = virt.substr(1);
        bool recurse_subdirs = (mask.find("**") != std::string::npos);
        auto found = recurse_subdirs ? apathy::lsr( path, mask ) : apathy::ls( path, mask );
        for( const auto &uri : found ) {
            std::string virt2( virt + apathy::replace( apathy::replace(uri, apathy::cwd(), "" ), phys, "") );
            table[ virt2 ] = uri;
        }
        return found.size() ? true : false;
    };

    return mount( path, virt, mask );
}

inline bool fs::supported( const std::string &virt ) const {
    auto proto = get_protocol(virt);
    for( auto &protocol : protocols ) {
        if( protocol == proto || ( protocol.empty() && proto.empty() ) ) {
            return true;
        }
    }
    return false;
}

inline std::string fs::translate( const std::string &virt ) const {
    if( supported( virt ) ) {
        auto found = table.find(get_pathfile(virt));
        if( found != table.end() ) {
            return found->second;
        }
    }
    return std::string();
}

inline bool fs::exists( const std::string &virt ) const {
    std::string phys = translate( virt );
    return !phys.empty();
}

inline bool fs::symlink( const std::string &from_virt, const std::string &to_virt ) {
    return false;
}

inline bool fs::rename( const std::string &virt_mask, const std::string &as_virt ) {
    auto copy = table;
    unsigned replaced = 0;
    for( auto &it : table ) {
        if( apathy::match(it.first.c_str(), virt_mask.c_str()) ) {
            copy.erase( it.first );
            copy[ as_virt ] = it.second;
            replaced++;
        }
    }
    return replaced > 0 ? ( table = copy, true ) : false;
}

inline std::string fs::head() const {
    std::string head, sep;
    for( auto &protocol : protocols ) {
        head += sep + protocol, sep = '|';
    }
    return head;
}

inline std::string fs::print() const {
    std::string toc, head = "[" + this->head() + "]";
    toc += ";; fs: " + name() + " {\n";
    for( auto &entry : table ) {
        toc += "\t" + head + entry.first + " -> " + entry.second + "\n";
    }
    toc += ";; }\n";
    return toc;
}

// virtual fs

inline vfs::vfs()
{}

inline vfs::~vfs()  {
    for( auto &fs : all ) {
        delete fs, fs = 0;
    }
}

inline bool vfs::mount( const std::string &phys, const std::string &virt ) {
    bool found = false;
    for( auto &fs : all ) {
        found |= fs->mount( phys, virt );
    }
    return found;
}

inline bool vfs::exists( const std::string &virt ) const {
    return locate( virt ) ? true : false;
}

inline std::string vfs::read( const std::string &virt ) const {
    fs *fs = locate( virt );
    if( fs ) {
        std::string phys = fs->translate( virt );
        if( !phys.empty() ) {
            std::string data = fs->read( phys );
            return data;
        }
    }
    return std::string();
}

inline std::string vfs::toc() const {
    std::string toc;
    for( auto &fs : all ) {
        toc += fs->print();
        /*
        for( auto &toc : fs.scan() ) {
            toc += toc.print();
        }
        */
    }
    return toc;
}

inline fs *vfs::locate( const std::string &virt ) const {
    std::string found;
    for( auto &fs : all ) {
        found = fs->translate( virt );
        if( !found.empty() ) {
            return fs;
        }
    }
    return 0;
}

#ifdef VFS_BUILD_TESTS
// unittest suite {
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#define suite(...) if(printf("------ " __VA_ARGS__),puts(""),true)
#define test(...)  (errno=0,++tst,err+=!(ok=!!(__VA_ARGS__))),printf("[%s] %d %s (%s)\n",ok?" OK ":"FAIL",__LINE__,#__VA_ARGS__,strerror(errno))
unsigned tst=0,err=0,ok=atexit([]{ suite("summary"){ printf("[%s] %d tests = %d passed + %d errors\n",err?"FAIL":" OK ",tst,tst-err,err); }});
// }

namespace game {
    struct texture {
        int id;

        texture()
        {}

        texture( const void *ptr, size_t len ) : id(0) {
            if( ptr ) {
                // do whatever
                // glBindTexture
            } else {
                id = 0; // placeholder
            }
        }
    };
};

int main() {
    vfs vfs;

    // register std_io_fs to handle "disk://", "game://" and "" empty protocol urls
    vfs.add_fs( new std_io_fs("./*"), {"disk://", "game://", ""} );

    // test 
    game::texture tex;
    tex = vfs.make<game::texture>( "test.tga" );
    vfs.make( tex, "test.tga" );

    vfs.mount("tests/*", "my_tests/");
    vfs.mount("music.zip/*", "music/");
    vfs.mount("textures/*", "icons/");
    vfs.mount("*.c*", "code/");
    vfs.mount("**.hpp", "code-r/");

    // flat paths
    vfs.mount("dlc-watchman/*.txt", "/" );

    std::cout << vfs.toc() << std::endl;

    test( vfs.exists("code-r/vfs.hpp") );
    test( vfs.exists("disk://code-r/vfs.hpp") );
    test( vfs.exists("game://code-r/vfs.hpp") );

    std::cout << vfs.toc() << std::endl;

    test( vfs.read("disk://code-r/vfs.hpp") == apathy::read(apathy::normalize(__FILE__)) );
}

#endif
