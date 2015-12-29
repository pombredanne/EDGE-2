#pragma once
#include <map>
#include <functional>
#include <deque>

namespace eve {
    class string;
    using strings = std::deque<eve::string>;
    void console( const std::string &title, const std::string &prompt, const std::map< eve::string, std::function<void(const eve::strings &)> > &cb );
}
