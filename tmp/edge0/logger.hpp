#ifndef EE_LOGGER_HPP
#define EE_LOGGER_HPP

namespace ee {

struct logger {
    virtual ~logger() {}
    virtual void dev( const char *fmt, ... ) = 0;
    virtual void info( const char *fmt, ... ) = 0;
    virtual void warning( const char *fmt, ... ) = 0;
    virtual void error( const char *fmt, ... ) = 0;
};

}

#endif
