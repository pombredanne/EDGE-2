#include "rpc/api.hpp"
#include "rpc/detail/packet.hpp"
#include "rpc/detail/receiver.hpp"
#include "rpc/detail/sender.hpp" // SendMessage declaration

#include <utility>

namespace RPC
{
	Message OnReceive( Message&& receivedRaw )
	{
		using namespace detail;
		try
		{
			// this may raise an UnexpectedPacketException
			Packet received( Packet::ReceiveCall( std::forward< std::vector< char> >( receivedRaw ) ) );

			unsigned int index = received.GetNextMember< unsigned int >( );
			return IReceiver::Receive( index, received ).GetData();
		}
		catch( UnexpectedPacketException& e )
		{
			return Packet::MakeError( Packet::ErrorType::UnexpectedPacket, e.what() ).GetData();
		}
	}

	static Message DefaultSender( Message&& m )
	{
		throw NoSenderRegisteredException( "No MessageSender registered!" );
	}

	static MessageSender s_sender = DefaultSender;

	void RegisterSender( const MessageSender& sender )
	{
		s_sender = sender;
	}

	namespace detail
	{
		Packet SendMessage( Packet&& p )
		{
			return Packet::ReceiveResult( s_sender( p.MoveData() ) );
		}
	}
}
