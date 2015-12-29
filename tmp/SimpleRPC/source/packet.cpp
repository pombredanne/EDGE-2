#include "rpc/detail/packet.hpp"

namespace RPC
{
	namespace detail
	{
		Packet::Packet( const MessageType type, std::vector< char >::size_type minSize )
		{
			m_data.reserve( minSize );
			AddPlainData< MessageType >( type );
		}

		Packet::Packet( std::vector< char >&& data )
			: m_data( data )
		{
		}

		Packet Packet::MakeError( const ErrorType type, const std::string& string )
		{
			Packet p( MessageType::Error, sizeof( MessageType ) + sizeof( MemberType ) + string.size() );
			p.AddPlainData< ErrorType >( type );
			p.AddMember< std::string >( string );
			return p;
		}

		Packet Packet::MakeEmptyCall( const unsigned int index )
		{
			Packet p( MessageType::Call, sizeof( MessageType ) + sizeof( MemberType ) + sizeof( unsigned int ) );
			p.AddMember< unsigned int >( index );
			return p;
		}

		Packet Packet::MakeEmptyResult()
		{
			return Packet( MessageType::Result, sizeof( MessageType ) );
		}

		Packet Packet::ReceiveCall( std::vector< char >&& data )
		{
			Packet p( std::forward< std::vector< char > >( data ) );
			MessageType type = p.GetPlainData< MessageType >( );
			if( type != MessageType::Call )
			{
				std::stringstream ss;
				ss << "Expected Call Packet, got " << int( type ) << "!";
				throw UnexpectedPacketException( ss.str() );
			}
			return p;
		}

		Packet Packet::ReceiveResult( std::vector< char >&& data )
		{
			Packet p( std::forward< std::vector< char > >( data ) );
			MessageType type = p.GetPlainData< MessageType >();
			if( type == MessageType::Error )
			{
				ErrorType error = p.GetPlainData< ErrorType >();
				std::string what = p.GetNextMember< std::string >();
				switch( error )
				{
				case ErrorType::EndOfPacket:
					throw EndOfPacketException( "Remote Exception: " + what );
				case ErrorType::InvalidParameter:
					throw InvalidParameterException( "Remote Exception: " + what );
				case ErrorType::UnexpectedPacket:
					throw UnexpectedPacketException( "Remote Exception: " + what );
				default:
					throw RPCException( "Remote Exception: " + what );
				}
				throw RPCException( p.GetNextMember< std::string >() );
			}
			else if( type != MessageType::Result )
			{
				std::stringstream ss;
				ss << "Expected Result Packet, got " << int( type ) << "!";
				throw UnexpectedPacketException( ss.str() );
			}
			return p;
		}

		void Packet::AddData( const char* data, unsigned int length )
		{
			for( const char* it = data; it != data + length; ++it )
			{
				m_data.push_back( *it );
			}
		}

		const char* Packet::GetData( unsigned int length )
		{
			if( m_position + length > m_data.size() )
			{
				throw EndOfPacketException();
			}
			m_position += length;
			return m_data.data() + m_position - length;
		}
	}
}
