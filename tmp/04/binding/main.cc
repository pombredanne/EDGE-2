#include <string>

class Web
{
public:
    // base_url is optional
    Web(const std::string& base_url);
    ~Web();

    static void go_home();

    static std::string home_url();
    static void set_home_url(const std::string& url);

    std::string url() const;
    void set_url(const std::string& url);
    std::string resolve_url(const std::string& uri);

    // doing reload if uri is empty
    std::string load(const std::string& uri);
};

#include <LuaIntf/LuaIntf.h>

LuaBinding(L).beginClass<Web>("Web")
    .addConstructor(LUA_ARGS(_opt<std::string>))
    .addStaticProperty("home_url", &Web::home_url, &Web::set_home_url)
    .addStaticFunction("go_home", &Web::go_home)
    .addProperty("url", &Web::url, &Web::set_url)
    .addFunction("resolve_url", &Web::resolve_url)
    .addFunction("load", &Web::load, LUA_ARGS(_opt<std::string>))
    .addStaticFunction("lambda", [] {
        // you can use C++11 lambda expression here too
        return "yes";
    })
.endClass();

int main() {
    
}
