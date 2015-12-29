#include <functional>
#include <vector>

namespace RPC
{
	typedef std::vector< char > Message;
	typedef std::function< Message( Message&& ) > MessageSender;

	/**
		Call this whenever you receive a message on the Remote where the RPCs are to be called

		@return Message to send back.
	**/
	Message OnReceive( Message&& received );

	/**
		Call this in the program doing the RPC calls to register a function for sending Messages. It should then wait for an answer from the Remote and return that.
	**/
	void RegisterSender( const MessageSender& sender );
}