#ifndef EE_FILESYSTEM_HPP
#define EE_FILESYSTEM_HPP

#include <stdio.h>

// disk
// mmap
// http
// zipf
// tarf

namespace ee {
    typedef const char cstr;

    struct filesystem {
        virtual ~filesystem() {}

        virtual void   mount( const char *filemask ) = 0;
        virtual void   unmount( const char *filemask ) = 0;

        virtual void   scan() = 0;
        virtual cstr*  find( const char *filename ) = 0;

        //virtual void   preload( const char *filemask ) = 0;
        virtual void   purge( const char *filemask ) = 0;

        virtual void*  read( const char *filename ) = 0;
        virtual size_t size( const char *filename ) = 0;
    };
}

#endif
