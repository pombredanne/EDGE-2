// based on code by /r/agwaman
// - rlyeh. boost licensed

#pragma once

#include <sstream>

template<char separator>
inline void build_string (std::ostream& o) 
{}    

template<char separator, class First, class... Rest>
inline void build_string (std::ostream& o, const First& value, const Rest&... rest){
    o << value << separator;
    build_string<separator>(o, rest...);
}

template<char separator, class... T>
inline std::string concat(const T&... value) {
    std::ostringstream o;
    build_string<separator>(o, value...);
    std::string out = o.str();
    return out.empty() ? out : (out.pop_back(), out);
}
