#include "eve.hpp"

#include <iostream>

// test api
EVE_SUITE() {
    EVE_TEST( 1 < 2 );
    EVE_TEST( 3 > 2 );
    std::cout << "im on a suite" << std::endl;
}


struct savegame {
    int level;
    std::string name;
};
EVE_SERIALIZE( savegame &it, (it.level, it.name) );

int main( int argc, const char **argv ) {
    eve::init( argc, argv );

    // getopt api
    eve::getopt invokation( argc, argv );

    // smart asserts
    EVE_ASSERT( 1 < 2 );

    // string api
    EVE_ASSERT( eve::string( "hello world \1 \2", 1, 2 ) == "hello world 1 2" );

    // encryption api
    eve::string encrypted = eve::encrypt( "hello world", "my-password" );
    eve::string decrypted = eve::decrypt( encrypted, "my-password" );
    EVE_ASSERT( decrypted == "hello world" );

    // pixel/image api
    eve::pixel rgba = eve::rgba(1,1,1,1);
    eve::pixel hsla = eve::hsla(1,1,1,1);
    eve::image img;

    // todoby api
    EVE_TODOBY( "Sep 26 2015" );
    EVE_TODOBY( "May 19 2014" );
    EVE_TODOBY( "Aug 11 2011" );

    // ini api
    eve::ini ini;
    ini.load( "[test]\nnumber=123\nstring=hello world;this is a comment" );
    EVE_ASSERT( ini["test.number"] == "123" );
    EVE_ASSERT( ini["test.string"] == "hello world" );
    ini["added.number"] = "456";
    std::cout << ini.save() << std::endl;

    // serialization
    savegame game { 10, "level#01" };
    std::string json = eve::to_json( game );
    game = savegame();
    eve::from_json( game, json );
    EVE_ASSERT( game.level == 10 );
    EVE_ASSERT( game.name == "level#01" );

    // entity/component/system based
    eve::entity player = 1;

    // debug api
    std::cout << eve::is_debug() << std::endl;
    std::cout << eve::is_release() << std::endl;
    //std::cout << eve::is_master() << std::endl;

    if( !eve::is_asserting() ) {
        std::cout << "Asserts are disabled. No assertions will be perfomed" << std::endl;
    } else {
        std::cout << "Asserts are enabled. Assertions will be perfomed" << std::endl;
    }

    // print current stack trace
    for( auto &line : eve::stacktrace("\1) \2") ) {
        std::cout << line << std::endl;
    }

    // add a web server, with stats in ajax
    eve::http_create( 8080, "/", []( const eve::http_request &req, std::ostream &headers, std::ostream &content ) {
        headers << eve::http_mime(".html"); 
        content << req.url;
        return 200;
    } );
    std::cout << "webservers installed in localhost:8080 (/)" << std::endl;

    // initialize chain of warns and fails
    // these prototypes return !=0 if they handle issue, or return 0 to delegate issue to inner ring
    eve::warns.push_back( []( const std::string &text ) {
        eve::alert( text, "this is our custom assert title" );
        return true;
    });

    eve::fails.push_back( []( const std::string &error ) {
        eve::errorbox( error + "\n\n" + eve::stackstring("\1) \2\n", 7) );
        // die();
        return true;
    });

    eve::alert( "this is a test" );
    eve::warn("this is a warning");
    eve::fail("this is a fail");

    eve::alert( 3.14159f );
    eve::alert( std::ifstream(__FILE__), "current source code" );
    eve::alert( eve::hexdump("hello world") );
    eve::alert( eve::prompt("type a number") );

    // console api
    eve::console( "Welcome to " EVE_VERSION_FULL, "eve>", {
        {   "args", [&]( const eve::strings &args ) {
                std::cout << invokation.str() << std::endl;
        } },
        {   "print", [&]( const eve::strings &args ) {
            for( auto &it : args ) std::cout << it << ','; std::cout << std::endl;
        } },
        {   "println", [&]( const eve::strings &args ) {
            for( auto &it : args ) std::cout << "\t" << it << std::endl;
        } },
        {   "globals", [&]( const eve::strings &args ) {
            for( auto &g : eve::globals ) std::cout << g.first << "=" << g.second << std::endl;
        } },
        {   "exit", [&](const eve::strings &args ) {
            std::exit(1);
        } },
        {   "vars", [&](const eve::strings &args ) {
            // live vars can be tweaked during runtime
            std::cout << "int a=" << EVE_LIVE( 4321 ) << std::endl;
            std::cout << "hello=" << EVE_LIVE( "hello world" ) << std::endl;
        } }
    } );

    /*
    ./~> console
    eve>
    eve>k=100+230
    eve>k
    330
    eve>println(k,100)
            330
            100
    eve>println(k*300,100)
            k*300
            100
    eve>^C

    */

    for(;;) {
    }

}
