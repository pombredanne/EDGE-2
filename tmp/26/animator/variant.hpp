// tiny variant type.
// - rlyeh, boost licensed ~~ listening to mythos69 - requiem

#pragma once
#include <string>

struct var {
    int type;
    union {
        int integer;
        double real;
        std::string *string;
    };

    void cleanup();

    var() : type(0), integer(0)
    {}

    var( const int &i ) : type(0), integer(i)
    {}

    var( const double &r ) : type(1), real(r)
    {}

    var( const std::string &r ) : type(2), string( new std::string(r) )
    {}

    template<size_t N>
    var( const char (&s)[N]) : type(2), string( new std::string(s) )
    {}

    var( const var &other ) {
        operator=( other );
    }

    ~var() {
        cleanup();
    }

    var &operator=( const var &other ) {
        if( &other != this ) {
            cleanup();
            type = other.type;
            if( type == 0 ) integer = other.integer;
            if( type == 1 ) real = other.real;
            if( type == 2 ) string = new std::string( *other.string );
        }
        return *this;
    }
};

template<typename T> inline bool isType(const var &v) { return false; }
template<>           inline bool isType<int>(const var &v) { return v.type == 0; }
template<>           inline bool isType<double>(const var &v) { return v.type == 1; }
template<>           inline bool isType<std::string>(const var &v) { return v.type == 2; }

template<typename T> inline const T& cast(const var &v) { static T t; return t = T(); }
template<>           inline const int& cast<int>(const var &v) { return v.integer; }
template<>           inline const double& cast<double>(const var &v) { return v.real; }
template<>           inline const std::string& cast<std::string>(const var &v) { return *v.string; }

inline
void var::cleanup() {
    if(isType<std::string>(*this)) delete string, string = 0;
    type = 0;
}

