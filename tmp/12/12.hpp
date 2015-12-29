#include <string>

#ifndef EE_VERBOSITY
#define EE_VERBOSITY 0
#endif

#ifndef EE_VELOCITY
#define EE_VELOCITY  0 // optim level, p367
#endif

// Config (cvars). Must be visible from Lua too.
// load, save, get -> cfg.ini, usr/cfg.ini
int eeConfigGet( std::string &value, const std::string &key );
int eeConfigSet( const std::string &key, const std::string &value );
int eeConfigLoad();
int eeConfigLoadUsr();
int eeConfigSave();
int eeConfigSaveUsr();

// Log to stream.
int eeDebugLog( int channel_mask, const std::string &txt );

// Log to stream. With stack.
int eeDebugTrace( int channel_mask, const std::string &txt );

// Retrieve log
int eeDebugDump( std::ostream & );

// Callstacks
int eeDebugGetStack( void **cs );
int eeDebugResolveStack( std::string &out, const void **in );

// Debugger
int eeDebuggerInvoke(); // C++, Lua
int eeDebuggerPause();
int eeDebuggerResume();
int eeDebuggerToggleBreakpoint( const char *file, int line, int onoff );
int eeDebuggerToggleDataPoint( const char *text );

// Console. Must be C++ (RPC) and Lua friendly
int eeConsoleRead( std::string &input );
int eeConsoleEval( std::string &output, const std::string &script );
int eeConsoleDump( std::ostream &os );

// Memory info
int eeMemoryRealloc( ptr, size, align );
int eeMemoryStatus();
int eeMemoryDumpLeaks( std::ostream &log );

// Profiling, $
int eeProfileToggle( int channel, int onoff );
int eeProfileBegin( int channel );
int eeProfileEnd( int channel );
int eeProfileReset();
int eeProfileDump();

// Crash reports p371
// Coredumps

// Launch options
// to cvars( argc, argv )

// Gameobjects p714
// serialization -> cout || debug ui -> [key|value]
// for( all : sys ) if( sys has entity ) dump( sys[entity] );
// key|value table, tweak imgui
