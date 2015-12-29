#include "drecho/drecho.hpp"
#include "heal/heal.hpp"
#include <fstream>

#ifdef SHIPPING
#define EE_DEBUG(...) do {} while(0)
#define EE_TRACE(...) do {} while(0)
#else
#define EE_DEBUG(...) do { std::clog << "" __VA_ARGS__ << " err:" << errno << " (" __FILE__ ":" << __LINE__ << ")\n"; } while(0)
#define EE_TRACE(...) do { EE_DEBUG( __VA_ARGS__ ); std::clog << heal::stackstring() << "\n"; } while(0)
#endif

// default settings
const bool dr::log_timestamp = true;
const bool dr::log_branch = true;
const bool dr::log_branch_scope = true;
const bool dr::log_text = true;
const bool dr::log_errno = true;
const bool dr::log_location = true;

int main() {
    using namespace std;

    // Set up a few custom keywords to highlight
    dr::highlight( DR_YELLOW, { "warn", "warning" } );
    dr::highlight( DR_GREEN, { "info", "debug" } );
    dr::highlight( DR_CYAN, { "another", "branch" } );

    // Spice up clog from now
    dr::capture( clog );
    cout << "another cout warning that wont be highlighted" << endl;
    clog << "another clog warning that will be highlighted" << endl;

    // Showcase tree usage
    {
        dr::tab scope;
        clog << "this is branch #1" << endl;
        {
            dr::tab scope;
            clog << "this is branch #1.1" << endl;
            clog << "this is branch #1.2" << endl;
            {
                dr::tab scope;
                clog << "this is branch #1.2.1" << endl;
                clog << "this is branch #1.2.2" << endl;
            }
            clog << "this is branch #1.3" << endl;
        }
        clog << "this is branch #2" << endl;

        EE_DEBUG( "this is a test " << 1 );
        EE_TRACE( "this is a test " << 2 );
    }

    // Release our spiced up clog
    dr::release( clog );
}

