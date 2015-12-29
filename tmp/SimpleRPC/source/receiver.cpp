#include "rpc/detail/receiver.hpp"
#include "rpc/exceptions.hpp"
#include <sstream>

namespace RPC
{
	namespace detail
	{
		static std::vector< IReceiver* >& GetReceivers()
		{
			// Careful! This gets called by constructors of global variables in another unit, so it mustn't be a global variable!
			static std::vector< IReceiver* > s_receivers;
			return s_receivers;
		}
		
		IReceiver::IReceiver()
		{
			GetReceivers().push_back( this );
		}

		IReceiver::~IReceiver()
		{
			for( auto & receiver : GetReceivers() )
			{
				if( receiver == this )
				{
					receiver = nullptr;
					break;
				}
			}
		}

		Packet IReceiver::Receive( unsigned int index, Packet& input )
		{
			if( index >= GetReceivers().size( ) )
			{
				std::stringstream ss;
				ss << "RPC index " << index << " is out of bounds [" << 0 << ", " << GetReceivers().size( ) - 1 << "]!";
				throw InvalidIndexException( ss.str() );
			}
			try
			{
				Packet output( Packet::MakeEmptyResult() );
				GetReceivers()[ index ]->Receive( input, output );
				return output;
			}
			catch( InvalidParameterException& e )
			{
				return Packet::MakeError( Packet::ErrorType::InvalidParameter, e.what( ) );
			}
			catch( EndOfPacketException& e )
			{
				return Packet::MakeError( Packet::ErrorType::EndOfPacket, e.what( ) );
			}
			catch( std::exception& e )
			{
				return Packet::MakeError( Packet::ErrorType::RPCException, std::string( "RPC threw: " ) + e.what( ) );
			}
			catch( ... )
			{
				return Packet::MakeError( Packet::ErrorType::RPCException, "RPC threw unknown exception!" );
			}
		}
	}
}
