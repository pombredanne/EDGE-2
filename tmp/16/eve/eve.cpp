#include "eve.hpp"

namespace eve {

//#   undef echo
//    std::ostream &log = dr::echo;

    bool app::init( int argc, const char **argv, const std::string &options ) {
/*
        eve::warns.push_back( []( const std::string &text) {
            return bubble::show(
            "title.text=ƎVE warning;"
            "body.icon=14;"
            "body.text=" + text + ";"
            //"progress=-1;"
            "footer.icon=-3;"
            "footer.text=ƎVE version (built " EVE_VERSION_FULL ")", []( bubble::vars &ui ) {
                // ui["progress"] = ui["progress"] + 10;
            } ), true;
        } );
        eve::fails.push_back( []( const std::string &text ) {
            return bubble::show(
            "title.text=ƎVE assert;"
            "body.icon=89;"
            "body.text=" + text + ";"
            //"progress=-1;"
            "footer.icon=-3;"
            "footer.text=ƎVE version (built " EVE_VERSION_FULL ")", []( bubble::vars &ui ) {
                // ui["progress"] = ui["progress"] + 10;
            } ), true;
        } );
*/
        return true;
    }
    bool app::quit() {
        return true;
    }
    bool app::open() {
        return true;
    }
    bool app::pump() {
        return true;
    }

    bool init( int argc, const char **argv ) {
        /*
        dr::highlight( DR_GREEN, { "eve>"} );
        dr::highlight( DR_YELLOW, { "warn", "warning", "info", "debug" } );
        dr::highlight( DR_RED, { "err", "errno", "error" });
        static const bool auto_init1 = dr::capture( std::cout );
        static const bool auto_init2 = dr::capture( std::cerr );
        //static const bool auto_init3 = dr::capture( std::clog );
        */
        return true;
    }
}
