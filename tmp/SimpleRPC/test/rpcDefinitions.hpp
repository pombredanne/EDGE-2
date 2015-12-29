// This is where the functions that are to be turned into RPCs are defined.
// Note lack of include guard - this is included multiple times with different RPC_FUNCTION definitions

#define RPC_API_NAME "RPC Test"
#define RPC_API_VERSION 0

// RPC_FUNCTION( Foo, int, std::string, std::vector< float > )
RPC_FUNCTION( HelloWorld, void, void )
RPC_FUNCTION( HelloI, void, int )
RPC_FUNCTION( HelloStr, int, const std::string& );
RPC_FUNCTION( Get42, int, void )
RPC_FUNCTION( GetI, int, const int )
RPC_FUNCTION( GetRef, void, int& );
