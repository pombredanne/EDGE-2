#ifndef RPC_SENDERS_HPP_INCLUDED
#define RPC_SENDERS_HPP_INCLUDED

#include "detail/sender.hpp"

namespace RPC
{
#	define RPC_FUNCTION( name, ... ) extern detail::Sender< __VA_ARGS__ > name;
#	include "rpcDefinitions.hpp"
#	undef RPC_FUNCTION
#	define RPC_FUNCTION( name, ... ) detail::Sender< __VA_ARGS__ > name;
}

#endif // RPC_SENDERS_HPP_INCLUDED
