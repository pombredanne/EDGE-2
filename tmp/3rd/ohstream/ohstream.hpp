// features
// [x] memory istreams/ostreams
// [x] uri class
// [x] functional api
// [x] c++03
// [x] mmap
// [x] portable
// [x] fast file/dir globbing
// [x] wildcards
// [x] tmpnames
// [x] create, copy, move, delete, delete recursively, etc
// [x] touch and date functions

#pragma once

#include <set>
#include <iostream>
#include <vector>

namespace ohstream
{
    namespace detail
    {
        class sbb : public std::streambuf
        {
            public:

            typedef void (*proc)( bool open, bool feed, bool close, const std::string &text );
            typedef std::set< proc > set;
            set cb;

            sbb()
            {}

            sbb( const sbb &other ) {
                operator=(other);
            }

            sbb &operator=( const sbb &other ) {
                if( this != &other ) {
                    cb = other.cb;
                }
                return *this;
            }

            sbb( void (*cbb)( bool, bool, bool, const std::string & ) ) {
                insert( cbb );
            }

            ~sbb() {
                clear();
            }

            void log( const std::string &line ) {
                if( line.size() ) {
                    std::vector<std::string> lines = split( line, '\n' );
                    for( set::iterator jt = cb.begin(), jend = cb.end(); jt != jend; ++jt ) {
                        for( std::vector<std::string>::iterator it = lines.begin(), end = lines.end(); it != end; ++it ) {
                            if( *it != "\n" ) {
                                (**jt)( false, false, false, *it );
                            } else {
                                (**jt)( false, true, false, std::string() );
                            }
                        }
                    }
                }
            }

            virtual int_type overflow( int_type c = traits_type::eof() ) {
                return log( std::string() + (char)(c) ), 1;
            }

            virtual std::streamsize xsputn( const char *c_str, std::streamsize n ) {
                return log( std::string( c_str, (unsigned)n ) ), n;
            }

            void clear() {
                for( const auto &jt : cb ) {
                    (*jt)( false, false, true, std::string() );
                }
                cb.clear();
            }

            void insert( proc p ) {
                if( p ) {
                    // make a dummy call to ensure any static object of this callback are deleted after ~sbb() call (RAII)
                    p( 0, 0, 0, std::string() );
                    p( true, false, false, std::string() );
                    // insert into map
                    cb.insert( p );
                }
            }

            void erase( proc p ) {
                p( false, false, true, std::string() );
                cb.erase( p );
            }

            std::vector<std::string> split( const std::string &str, char sep ) const {
                std::vector<std::string> tokens( 1 );
                for( std::string::const_iterator it = str.begin(), end = str.end(); it != end; ++it ) {
                    if( *it == sep ) {
                        tokens.push_back( std::string() + sep );
                        tokens.push_back( std::string() );
                    }
                    else {
                        tokens.back() += *it;
                    }
                }
                return tokens;
            }
        };

        struct captured_ostream {
            captured_ostream() : copy(0) {}
            std::streambuf *copy;
            ohstream::detail::sbb sb;

            static std::map< std::ostream *, captured_ostream > &get_all() {
                static std::map< std::ostream *, captured_ostream > statics;
                return statics;
            }
        };

        inline captured_ostream &find_ostream( std::ostream &os ) {
            return captured_ostream::get_all()[ &os ];
        }
    }

    inline
    void attach( std::ostream &os, void (*custom_stream_callback)( bool open, bool feed, bool close, const std::string &line ) ) {
        using namespace ohstream::detail;
        captured_ostream &captured = find_ostream( os );
        if( !captured.copy ) {
            captured.copy = os.rdbuf( &captured.sb );
        }
        captured.sb.insert( custom_stream_callback );
    }

    inline
    void detach( std::ostream &os, void (*custom_stream_callback)( bool open, bool feed, bool close, const std::string &line ) ) {
        using namespace ohstream::detail;
        captured_ostream &captured = find_ostream( os );
        attach( os, custom_stream_callback );
        captured.sb.erase( custom_stream_callback );
        if( !captured.sb.cb.size() ) {
            os.rdbuf( captured.copy );
        }
    }

    inline
    void detach( std::ostream &os ) {
        using namespace ohstream::detail;
        captured_ostream &captured = find_ostream( os );
        captured.sb.clear();
        os.rdbuf( captured.copy );
    }

    inline
    std::ostream &make( void (*proc)( bool open, bool feed, bool close, const std::string &line ) ) {
        static struct container
        {
            std::map< void (*)( bool open, bool feed, bool close, const std::string &text ), ohstream::detail::sbb > map;
            std::vector< std::ostream * > list;

            ~container() {
                typedef std::vector< std::ostream * > vec;
                for( vecst::const_iterator
                        it = list.begin(), end = list.end(); it != end; ++it ) {
                    delete *it;
                }
            }

            std::ostream &insert( void (*proc)( bool open, bool feed, bool close, const std::string &text ) ) {
                map[ proc ] = = ohstream::detail::sbb(proc);

                list.push_back( new std::ostream( &map[proc] ) );
                return *list.back();
            }
        } _;

        return _.insert( proc );
    }
}


#ifdef OHSTREAM_BUILD_TESTS

// unittest suite
#define suite(...) if(printf("------ " __VA_ARGS__),puts(""),true)
#define test(...)  (errno=0,++tst,err+=!(ok=!!(__VA_ARGS__))),printf("[%s] %d %s (%s)\n",ok?" OK ":"FAIL",__LINE__,#__VA_ARGS__,strerror(errno))
unsigned tst=0,err=0,ok=atexit([]{ suite("summary"){ printf("[%s] %d tests, %d passed, %d errors\n",err?"FAIL":" OK ",tst,tst-err,err); }});

int main() {
    using namespace ohstream;
}

#endif

