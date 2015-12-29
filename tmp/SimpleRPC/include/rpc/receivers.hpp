#ifndef RPC_RECEIVERS_HPP_INCLUDED
#define RPC_RECEIVERS_HPP_INCLUDED

#include "detail/receiver.hpp"

// definitions of functions to be called
#define RPC_FUNCTION( name, retType, ... ) retType name( __VA_ARGS__ );
#include "rpcDefinitions.hpp"
#undef RPC_FUNCTION
#define RPC_FUNCTION( name, ... ) RPC::detail::Receiver< __VA_ARGS__ > name##_receiver( name );

#endif // RPC_RECEIVERS_HPP_INCLUDED
