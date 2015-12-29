// doc layout, oop, aos
// items v \ attribs -> | file        |  data        | size
//----------------------+-------------+--------------+---------
// #0                   | "hello"     | hello world  | 11
// #1                   | "hi"        | (null)       | 0
// ...

// fly layout, ecs, soa
// attribs v \ items -> |     #0      |      #1      |   ...
//----------------------+-------------+--------------+---------
// file                 | "hello"     | "hi"         |   ...
// data                 | hello world | (null)       |   ...
// size                 |    11       |  0           |   ...

#pragma once
#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include "portable_endian.h"
#include "lzjb/lzjb2010.h"
#include "lzjb/lzjb2010.c"
#include "crush/crush.hpp"
#include "crush/crush.cpp"
#include "shrinker/shrinker.h"
#include "shrinker/shrinker.c"
#define ZSTD_LEGACY_SUPPORT 0
#undef ERROR
#include "zstd/zstd_buffered.c"
#undef MIN
#undef MAX
#undef BLOCKSIZE
#include "zstd/zstd.h"
#include "zstd/zstd_compress.c"
#include "zstd/zstd_decompress.c"
#include "zstd/huff0.c"
#include "zstd/fse.c"

#define EMMETT_ENABLE_COMPRESSION 1

// evaluated compression alternatives {
// lzjb, yappy, shrinker, bsc_lzp, fastlz, crush, pithy, yalz77
// }

#include <chrono>
#include <sstream>
struct presto : public std::stringstream {
    std::ostream &os;
    double t;
    int line;
    presto( int line, std::ostream &os = std::cout ) : os(os), t(0), line(line) {
    }
    presto( std::ostream &os ) : os(os), t(0), line(0) {
    }
    ~presto() {
        auto str = this->str();
        if( !str.empty() ) {
            if( line ) {
                os << "[ OK ] " << line << " " << this->str() << " (" << t << "s)" << std::endl;
            } else {
                os << "[ OK ] " << this->str() << " (" << t << "s)" << std::endl;
            }
        }
    }
    static double now() {
        static auto const epoch = std::chrono::steady_clock::now(); // milli ms > micro us > nano ns
        return std::chrono::duration_cast< std::chrono::microseconds >( std::chrono::steady_clock::now() - epoch ).count() / 1000000.0;
    }
    operator bool () {
        return t == 0 ? (t -= now(), true) : ( t += now(), false );
    }
    void operator()( int line ) {
        this->line = line;
    }
    template<typename T>
    bool operator()( const T &t ) {
        return operator bool();
    }
};
#define presto(...) for( presto __VA_ARGS__(__LINE__); __VA_ARGS__; )


#ifndef EMMETT_PREALLOCATED_BLOCK
#define EMMETT_PREALLOCATED_BLOCK (256 * 1024) // 256 KiB
#endif


namespace {
#   define VLE_API inline
    VLE_API uint64_t vle_encode_u( uint8_t *buffer, uint64_t value ) {
        /* 7-bit packing. MSB terminates stream */
        const uint8_t *buffer0 = buffer;
        do {
            *buffer++ = (uint8_t)( 0x80 | (value & 0x7f) );
            value >>= 7;
        } while( value > 0 );
        *(buffer-1) ^= 0x80;
        return buffer - buffer0;
    }
    VLE_API uint64_t vle_decode_u( uint64_t *value, const uint8_t *buffer ) {
        /* 7-bit unpacking. MSB terminates stream */
        const uint8_t *buffer0 = buffer;
        uint64_t out = 0, j = -7;
        do {
            out |= (( ((uint64_t)(*buffer)) & 0x7f) << (j += 7) );
        } while( ((uint64_t)(*buffer++)) & 0x80 );
        *value = out;
        return buffer - buffer0;
    }
}

class emmett {
    public:
    // id: pkg1 (pkg file format, v1)
    // id: 1gkp (pkg file format, v1) (needs endian swap)

    struct entry {
        uint64_t offset, size; // entry in archive
    };
    union attribute {
        uint32_t x, y;
    };

    uint64_t id;
    std::vector< uint64_t > attribs;
    std::vector< entry    > entries;
    std::vector< uint8_t  > blob;

    enum {
        doc_v100 =  'doc\0',
        fly_v100 =  'fly\0',
        bitmask  =  '\3',
        padmask  =  '\4',
        bits8    =  '\0',
        bits16   =  '\1',
        bits32   =  '\2',
        bits64   =  '\3',
    };

    bool is_doc( uint64_t fourcc ) const {
        return (fourcc >> 8) == (doc_v100 >> 8);
    }
    bool is_fly( uint64_t fourcc ) const {
        return (fourcc >> 8) == (fly_v100 >> 8);
    }
    bool padded( uint64_t fourcc ) const {
        return ( fourcc & padmask ) > 0;
    }
    int  bitsize( uint64_t fourcc ) const {
        return 1 << (( fourcc & bitmask ) + 3);
    }

    bool is_supported_id( uint64_t fourcc ) const {
        return ( is_doc(fourcc) || is_fly(fourcc) ) && ((fourcc & 0x000000ff) <= 7 );
    }

    enum fourccs { 
         flag = 'flg-', 
         chr8 = 'chr-', 
         utf8 = 'utf-',
         int8 = 'i08-',  int16 = 'i16-',  int32 = 'i32-',  int64 = 'i64-', 
        uint8 = 'u08-', uint16 = 'u16-', uint32 = 'u32-', uint64 = 'u64-', 
                         flt16 = 'f16-',  flt32 = 'f32-',  flt64 = 'f64-',
    };

    bool is_supported_type( uint64_t fourcc ) const {
        switch( fourcc ) {
            case flag:
            case chr8:
            case utf8:
            case int8:
            case int16:
            case int32:
            case int64:
            case uint8:
            case uint16:
            case uint32:
            case uint64:
            case flt16:
            case flt32:
            case flt64: return true;

            default: return false;
        }
    }

    bool is_supported_attrib( uint64_t fourcc ) const {
        for( auto &attrib : attribs ) {
            if( attrib == fourcc ) {
                return true;
            }
        }
        return false;
    }

    // constructor

    emmett() {
        blob.reserve( EMMETT_PREALLOCATED_BLOCK );
    }

    bool operator ==( const emmett &doc ) const {
        return id == doc.id
        && entries.size() == doc.entries.size()
        && attribs.size() == doc.attribs.size()
        && blob.size() == doc.blob.size()
        && 0 == memcmp( entries.data(), doc.entries.data(), entries.size() * sizeof(entry) )
        && 0 == memcmp( attribs.data(), doc.attribs.data(), attribs.size() * sizeof(uint64_t) )
        && 0 == memcmp(    blob.data(),    doc.blob.data(),    blob.size() * sizeof(unsigned char) );
    }
    bool operator !=( const emmett &doc ) const {
        return !operator==(doc);
    }

    // info api

    size_t num_items() const {
        return attribs.empty() ? 0 : entries.size() / attribs.size();
    }
    size_t num_attribs() const {
        return attribs.size();
    }

    void toc( std::ostream &os ) const {
        for( auto end = num_items(), it = end - end; it != end; ++it ) {
            for( auto jend = num_attribs(), jt = jend - jend; jt != jend; ++jt ) {
                uint64_t x = attribs[jt];
                uint64_t fmt = (x >> 32) & 0xffffffff;
                x = htobe64(x);
                os << "item #" << it << " attrib #" << jt << " (";
                os.write( (const char *)&x, sizeof(x)) << ") ";
                bool is_ascii = chr8 == fmt;
                bool is_utf8 = utf8 == fmt;
                if( is_ascii || is_utf8 ) {
                    os.write( (const char *)data(it,jt), size(it,jt) );
                } else {
                    switch( fmt ) {
                        break; default: os << "??";
                        break; case   flag : os << (get<    bool>(it,jt) ? "true" : "false");
                        break; case   int8 : os <<  get<  int8_t>(it,jt);
                        break; case  uint8 : os <<  get< uint8_t>(it,jt);
                        break; case  int16 : os <<  get< int16_t>(it,jt);
                        break; case uint16 : os <<  get<uint16_t>(it,jt);
                        break; case  int32 : os <<  get< int32_t>(it,jt);
                        break; case uint32 : os <<  get<uint32_t>(it,jt);
                        break; case  int64 : os <<  get< int64_t>(it,jt);
                        break; case uint64 : os <<  get<uint64_t>(it,jt);
                        break; case  flt16 : os << "??";
                        break; case  flt32 : os <<  get<float>(it,jt);
                        break; case  flt64 : os <<  get<double>(it,jt);
                    }
                }
                os << std::endl;
            }
        }
    }

    // write api

    void push_attr( uint32_t type, uint32_t content ) {
        attribs.push_back( ( uint64_t(type) << 32 ) | content );
    }

    void push_data( const void *ptr, int len ) {
        auto offs = entries.size() ? entries.back().offset + entries.back().size : uint64_t(0);
        entries.push_back( entry { offs, uint64_t(len) } );

        blob.resize( offs + len );
        memcpy( &blob[offs], ptr, len );
    }

    template<typename T>
    void push_data( const T &t ) {
        push_data( &t, sizeof(t) );
    }

    void push_data( const char *text ) {
        push_data( text, strlen(text) + 1 );
    }

    uint64_t precalc() const {
        uint64_t count = 0;
        auto bs = bitsize(id);
        auto write = [&]( int bits, uint64_t t ) {
            return bits >> 3;
        };
        count += write(32, id);
        count += write(bs, attribs.size()); // 32
        for( auto &attrib : attribs ) {
            count += write(64, attrib); // 64
        }
        count += write(bs, entries.size()); // 32
        for( auto &entry : entries ) {
            count += write(bs, entry.size); // 32
        }
        count += write(bs, blob.size()); // 64
        count += blob.size();
        return count;
    }

    void write_blob( std::ostream &os, const void *ptr, size_t len ) const {
        auto bs = bitsize(id);
        auto write = [&]( int bits, uint64_t t ) {
            switch( bits ) {
                break; case 64: t = htobe64(t);
                break; case 32: t = htobe32(t);
                break; case 16: t = htobe16(t);
                default: ;
            }
            os.write( (const char *)&t, bits >> 3 );
        };
#if EMMETT_ENABLE_COMPRESSION
            char *zptr = (char *)malloc( len * 2 );
            int zlen;

            if(1) presto( t ) {
                zlen = shrinker_compress( (void *)ptr, zptr, len );
                t << "shrinker: " << len << " -> " << zlen;
            }

            if(1) presto( t ) {
                zlen = lzjb_compress2010( (uint8_t*)ptr, (uint8_t*)zptr, len, len * 2, 9); 
                t << "lzjb:     " << len << " -> " << zlen;
            }
            if(1) presto( t ) {
                zlen = crush::compress( 0, (uint8_t*)ptr, len, (uint8_t*)zptr); 
                t << "crush:    " << len << " -> " << zlen;
            }
            if(1) presto( t ) {
                zlen = ZSTD_compress( zptr, len * 2, ptr, len, 1 /*1..26*/ ); if( ZSTD_isError(zlen) ) zlen = 0;
                t << "zstdf:    " << len << " -> " << zlen;
            }

            if( zlen > 0 ) {
                write( bs, zlen );
                os.write( zptr, zlen );
            } else {
                write( bs, 0 );
                os.write( (const char *)ptr, len );
            }
            free( zptr );
#else
            write( bs, 0 );
            os.write( (const char *)ptr, len );
#endif
    }

    bool write( std::ostream &os ) const {
        auto bs = bitsize(id);
        auto write = [&]( int bits, uint64_t t ) {
            switch( bits ) {
                break; case 64: t = htobe64(t);
                break; case 32: t = htobe32(t);
                break; case 16: t = htobe16(t);
                default: ;
            }
            os.write( (const char *)&t, bits >> 3 );
        };
        if( os.good() ) {
            write(32, id);
            write(bs, attribs.size()); // 32
            for( auto &attrib : attribs ) {
                write(64, attrib); // 64
            }
#if EMMETT_ENABLE_COMPRESSION
            {
                std::vector<uint64_t> flat;
                flat.reserve( entries.size() );
                for( auto &entry : entries ) {
                    flat.push_back( entry.size );
                }
                write(bs, flat.size()); // 32
                write_blob( os, &flat[0], sizeof(flat[0]) * flat.size() );
            }
#else
            write(bs, entries.size()); // 32
            for( auto &entry : entries ) {
                write(bs, entry.size); // 32, bs
            }
#endif
            write(bs, blob.size()); // 64
            write_blob( os, &blob[0], blob.size() );
        }
        return os.good();
    }

    // read api

    const void *data( int item, int attrib ) const {
        auto &entry = entries[ attrib + item * attribs.size() ];
        return &blob[ entry.offset ];
    }
    size_t size( int item, int attrib ) const {
        auto &entry = entries[ attrib + item * attribs.size() ];
        return entry.size;
    }
    template<typename T>
    const T *cast( int item, int attrib ) const {
        return (const T*)data( item, attrib );
    }
    template<typename T>
    const T &get( int item, int attrib ) const {
        return *cast<T>( item, attrib );
    }

    void read_blob( std::istream &is, void *ptr, size_t olen ) {
        auto bs = bitsize(id);
        auto read = [&]( int bits ) -> uint64_t {
            uint64_t t;
            is.read( (char *)&t, bits >> 3 );
            switch( bits ) {
                break; case 64: t = be64toh(t);
                break; case 32: t = be32toh(t); t &= 0xfffffffff;
                break; case 16: t = be16toh(t); t &= 0xffff;
                break; case  8: t &= 0xff;
                default: break;
            }
            return t;
        };
#if 0
        is.read( (char *)&blob[0], blob.size() );
#else
        size_t zlen = read(bs);
        if( zlen ) {
            char *zptr = (char *)malloc( zlen );
            is.read( zptr, zlen );

            //int len = shrinker_decompress( zptr, ptr, olen );
            //size_t dlen = olen; bool ok = LZJB_OK == lzjb_decompress( (uint8_t*)zptr, (uint8_t*)ptr, zlen, &dlen);
            //int zlen = pithy_Decompress( (const char *)zptr, zlen, (char *)ptr, olen );
            //int zlen = bsc_lzp_decompress( (const unsigned char *)zptr, (unsigned char*)ptr, olen, 10, 4, 0);
            //static bool i = (YappyFillTables(),1);
            //ui8 *dst = YappyUnCompress( (const unsigned char *)zptr, ((const unsigned char *)zptr) + zlen, (unsigned char*)ptr );
            //int len = lzjb_decompress2010( (uint8_t*)zptr, (uint8_t*)ptr, zlen, olen, 9);
            //uint32_t len = crush::decompress( (uint8_t*)zptr, (uint8_t*)ptr, olen);
            uint32_t len = ZSTD_decompress( ptr, olen, zptr, zlen ); if( !ZSTD_isError(len) );

            free( zptr );
        } else {
            is.read( (char *)ptr, olen );
        }
#endif
    }


    bool read( std::istream &is ) {
        auto bs = bitsize(id);
        auto read = [&]( int bits ) -> uint64_t {
            uint64_t t;
            is.read( (char *)&t, bits >> 3 );
            switch( bits ) {
                break; case 64: t = be64toh(t);
                break; case 32: t = be32toh(t); t &= 0xfffffffff;
                break; case 16: t = be16toh(t); t &= 0xffff;
                break; case  8: t &= 0xff;
                default: break;
            }
            return t;
        };
        //*this = emmett();
        if( is.good() ) {
            uint32_t id( read(32) );
            if( id == this->id ) { //is_supported_id(id) ) {
                //this->id = id;
                attribs.resize(read(bs)); // 32
                for( auto &attrib : attribs ) {
                    attrib = read(64); // 64
                }
                uint64_t offs = 0;
#if EMMETT_ENABLE_COMPRESSION
                std::vector<uint64_t> flat;
                flat.resize(read(bs)); // 32
                read_blob(is, &flat[0], sizeof(flat[0]) * flat.size());
                entries.resize(flat.size());
                int id = 0;
                for( auto &entry : entries ) {
                    entry.size = flat[id++]; // 32
                    entry.offset = offs; // 64
                    offs += entry.size;
                }
#else
                entries.resize(read(bs)); // 32
                for( auto &entry : entries ) {
                    entry.size = read(bs); // 32
                    entry.offset = offs; // 64
                    offs += entry.size;
                }
#endif

                blob.resize(read(bs)); // 64
                read_blob(is, &blob[0], blob.size());

                return is.good();
            }
        }
        return false;
    }

};

#if defined(EMMETT_BUILD_TESTS) || defined(EMMETT_BUILD_DEMO)

#include <fstream>
#include <cassert>
#include <ctime>

// tiny unittest suite { // usage: int main() { /* orphan test */ test(1<2); suite("grouped tests") { test(1<2); test(1<2); } }
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define suite(...) if(printf("------ %d ", __LINE__), printf(__VA_ARGS__), puts(""), true)
#define test(...)  (errno=0,++tst,err+=!(ok=!!(__VA_ARGS__))),printf("[%s] %d %s (%s)\n",ok?" OK ":"FAIL",__LINE__,#__VA_ARGS__,strerror(errno))
unsigned tst=0,err=0,ok=atexit([]{ suite("summary"){ printf("[%s] %d tests = %d passed + %d errors\n",err?"FAIL":" OK ",tst,tst-err,err); }});
// } rlyeh, public domain.

// benchmark suite {
#include <chrono>
static double now() {
    static auto const epoch = std::chrono::steady_clock::now(); // milli ms > micro us > nano ns
    return std::chrono::duration_cast< std::chrono::microseconds >( std::chrono::steady_clock::now() - epoch ).count() / 1000000.0;
}
template<typename T> double bench_s ( const T &t ) { auto t0 = -now(); return (t(), t0+now()); }
template<typename T> double bench_ms( const T &t ) { return bench_s( t ) * 1000.0; }
#define benchmark(...) printf("[ OK ] %d %gms %s\n", __LINE__, bench_ms([&]{ __VA_ARGS__ ;}), #__VA_ARGS__ )
// } rlyeh, public domain

#endif

#if defined(EMMETT_BUILD_TESTS)

#include <string>

int main() {

    // create data model
    enum { name, data };

    for( auto it : std::vector<int> { emmett::bits8, emmett::bits16, emmett::bits32, emmett::bits64 } ) {
        std::string name1 = "test1.emm" + std::to_string( 1<<(it+3) );
        std::string name2 = "test2.emm" + std::to_string( 1<<(it+3) );

        emmett doctype;
        doctype.id = emmett::doc_v100 | it;
        doctype.push_attr( 'utf-', 'name' );
        doctype.push_attr( 'utf-', 'data' );
        doctype.push_attr( 'u32-', 'crc.' );

        emmett doc = doctype;

        suite( "save" ) {
            doc.push_data( "english.txt" );
            doc.push_data( "hello world" );
            doc.push_data( 0xdeadbeef );

            doc.push_data( "spanish.txt" );
            doc.push_data( "hola mundo" );
            doc.push_data( 0xcafebabe );

            std::ofstream ofs(name1, std::ios::binary);
            test( doc.write(ofs) );

            // inspect
            doc.toc(std::cout);
        }

        suite( "load & verify" ) {
            emmett doc2 = doctype;

            test( doc != doc2 );

            std::ifstream ifs(name1, std::ios::binary);
            test( doc2.read(ifs) );

            test( doc == doc2 );
        }
    }

    for( auto &N : std::vector<uint64_t> { 10 * 1000, 100 * 1000, 1 * 1000000, 10 * 1000000 } ) {
        enum {
            key,
            value
        };

        emmett doctype;
        doctype.id = emmett::doc_v100 | emmett::bits32;
        doctype.push_attr( 'i32-', 'key.' );
        doctype.push_attr( 'i32-', 'val.' );

        std::string name = std::string() + "test.emm32.x" + std::to_string(N);

        emmett doc, copy;
        suite( "benchmark, generating %lld items", N ) {
            doc = doctype;
            benchmark(
                for( uint64_t i = 0; i < N; ++i ) {
                    doc.push_data(i);
                    doc.push_data(i);
                }
            );
            copy = doc;
            test( copy == doc );
        }

        suite( "benchmark, saving %lld items", N ) {
            benchmark(
                std::ofstream ofs(name.c_str(), std::ios::binary);
                test( doc.write(ofs) );
            );
            test( copy == doc );
        }

        suite( "benchmark, loading %lld items", N ) {
            doc = doctype;
            benchmark(
                std::ifstream ifs(name.c_str(), std::ios::binary);
                test( doc.read(ifs) );
            );
            test( copy == doc );
        }
    }
}

#endif

#ifdef EMMETT_BUILD_DEMO

int main() {

    // create data model
    enum {
        name,
        is_file,
        binary_data,
        modification_date,
        checksum,
    };

    // create doctype model
    emmett doctype;
    doctype.id = emmett::doc_v100 | emmett::bits32;
    doctype.push_attr( 'utf-', 'name' );
    doctype.push_attr( 'flg-', 'file' );
    doctype.push_attr( 'utf-', 'data' );
    doctype.push_attr( 'u32-', 'modf' );
    doctype.push_attr( 'u32-', 'crc.' );

    {
        emmett doc = doctype;

        // serialize 3 items

        doc.push_data( "english.txt" );
        doc.push_data( true );
        doc.push_data( "hello world" );
        doc.push_data( std::time(0) / 1 );
        doc.push_data( 0xdeadbeef );

        doc.push_data( "spanish.txt" );
        doc.push_data( true );
        doc.push_data( "hola mundo" );
        doc.push_data( std::time(0) / 2 );
        doc.push_data( 0xcafebabe );

        doc.push_data( "folder" );
        doc.push_data( false );
        doc.push_data( "" );
        doc.push_data( std::time(0) / 4 );
        doc.push_data( 0 );

        // save

        std::ofstream ofs("test.emm", std::ios::binary);
        std::cout << "save :" << "()" [ !!doc.write(ofs) ] << std::endl;
    }

    {
        emmett doc = doctype;

        // load

        std::ifstream ifs("test.emm", std::ios::binary);
        std::cout << "load :" << "()" [ !!doc.read(ifs) ] << std::endl;

        // random access (using enums)

        assert( doc.get<uint32_t>( /*item#*/0, checksum ) == 0xdeadbeef );
        assert( doc.get<    bool>( /*item#*/1, is_file )  == true       );

        // inspect

        doc.toc( std::cout );
    }
}

#endif


/*

[ OK ] 2.157s yappy:    160,000,000 -> 60,039,211
[ OK ] 0.268s shrinker: 160,000,000 -> 59,474,363
[ OK ] 0.263s lzjb:     160,000,000 -> 53,857,186
[ OK ] 0.252s bsc_lzp:  160,000,000 -> 62,433,976
[ OK ] 0.348s fastlz:   160,000,000 -> 60,024,090
[ OK ] 0.277s lzjb2010: 160,000,000 -> 53,857,186
[ OK ] 3.131s crush:    160,000,000 -> 33,785,468

[ OK ] lzjb:   160,000,000 ->  7,500,010 (0.19284s)
[ OK ] crush0: 160,000,000 ->    883,398 (3.26464s)
[ OK ] zstdf:  160,000,000 ->     25,655 (0.03761s)
[ OK ] lzjb:   160,000,000 -> 53,857,186 (0.28427s)
[ OK ] crush0: 160,000,000 -> 33,785,468 (2.93067s)
[ OK ] zstdf:  160,000,000 -> 10,798,308 (0.46205s)

*/