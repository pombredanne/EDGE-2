#ifndef RPC_DETAIL_PACKET_HPP_INCLUDED
#define RPC_DETAIL_PACKET_HPP_INCLUDED

#include "../exceptions.hpp"
#include "helpers.hpp"
#include "platform.hpp"

#include <vector>
#include <type_traits>
#include <cstring>
#include <string>
#include <sstream>
#include <utility>

namespace RPC
{
	namespace detail
	{
		
		class Packet
		{
			template< class T > friend struct SpecializedPacket;
			
		public:
			enum class ErrorType : char
			{
				EndOfPacket,
				InvalidParameter,
				UnexpectedPacket,
				RPCException // RPC threw
			};

			static Packet MakeError( const ErrorType type, const std::string& what );
			static Packet MakeEmptyCall( const unsigned int index );
			static Packet MakeEmptyResult();

			static Packet ReceiveCall( std::vector< char >&& data );
			static Packet ReceiveResult( std::vector< char >&& data );

			template< typename T >
			void AddMember( typename ConstRef< T >::type t );

			template< typename T >
			typename Plain< T >::type GetNextMember();

			const std::vector< char > GetData() const { return m_data; }
			std::vector< char >&& MoveData() { return std::move( m_data ); }

		private:
			enum class MessageType : char
			{
				Call, // Contains the RPC ID, followed by arguments in ascending order.
				Result, // Contains results from a function call: return value, last reference argument, ..., first reference argument (where available).
				Error // contains an error type, followed by the what() string
			};

			enum class MemberType : char
			{
				Plain, // plain data, can be copied verbatim
				Vector, // an std::string
				String // an std::vector
			};

			/// Create an empty Packet with the given size reserved
			Packet( const MessageType type, std::vector< char >::size_type minSize );

			/// Create a packet from given data
			Packet( std::vector< char >&& data );

			/// Saves Plain Data = memcpy (copies the bytes)
			template< typename T >
			void AddPlainData( typename ConstRef< T >::type t );

			/// Gets Plain Data = memcpy (copies the bytes); increases position
			template< typename T >
			typename Plain< T >::type GetPlainData();

			void AddData( const char* data, unsigned int length );
			/// Gets binary data, increases position
			const char* GetData( unsigned int length );

			std::vector< char > m_data;
			unsigned int m_position{ 0 };
		};
		
		//    Specialization

		// Plain Data
		template< typename T >
		struct SpecializedPacket
		{
			static_assert( !std::is_reference< T >::value, "Use std::remove_reference< T >::type for Specialized!" );
			static_assert( !std::is_const< T >::value, "Use std::remove_const< T >::type for Specialized!" );

			static_assert( !std::is_pointer< T >::value, "Can't serialize pointers!" );
			static_assert( std::is_default_constructible< typename Plain< T >::type >::value, "Only types with a default constructor can be serialized!" );
			static_assert( IS_TRIVIALLY_COPYABLE( typename Plain< T >::type ), "Only trivially copyable types and selected STL containers can be serialized!" );

			static T GetNextMember( Packet& p )
			{
				Packet::MemberType type = p.GetPlainData< Packet::MemberType >();
				if( type != Packet::MemberType::Plain )
				{
					std::stringstream ss;
					ss << "Expected Plain data member, got " << int( type ) << "!";
					throw InvalidParameterException( ss.str() );
				}
				return p.GetPlainData< T >();
			}

			static void AddMember( Packet& p, typename ConstRef< T >::type t )
			{
				p.AddPlainData< Packet::MemberType >( Packet::MemberType::Plain );
				p.AddPlainData< T >( t );
			}
		};

		// std::string
		template<>
		struct SpecializedPacket< std::string >
		{
			static std::string GetNextMember( Packet& p )
			{
				Packet::MemberType type = p.GetPlainData< Packet::MemberType >();
				if( type != Packet::MemberType::String )
				{
					std::stringstream ss;
					ss << "Expected Plain data member, got " << int( type ) << "!";
					throw InvalidParameterException( ss.str() );
				}
				unsigned int size = p.GetPlainData< unsigned int >();
				return std::string( p.GetData( size ), size );
			}

			static void AddMember( Packet& p, const std::string& str )
			{
				p.AddPlainData< Packet::MemberType >( Packet::MemberType::String );
				p.AddPlainData< unsigned int >( str.size() );
				p.AddData( str.data(), str.size() );
			}
		};

		// std::vector
		template< typename T >
		struct SpecializedPacket< std::vector< T > >
		{
			static std::vector< T > GetNextMember( Packet& p )
			{
				Packet::MemberType type = p.GetPlainData< Packet::MemberType >();
				if( type != Packet::MemberType::Vector )
				{
					std::stringstream ss;
					ss << "Expected Vector member, got " << int( type ) << "!";
					throw InvalidParameterException( ss.str() );
				}
				unsigned int numMembers = p.GetPlainData< unsigned int >();
				std::vector< T > result;
				result.reserve( numMembers );
				for( unsigned int i = 0; i < numMembers; ++i )
				{
					result.emplace_back( p.GetNextMember< T >() );
				}
				return result;
			}

			static void AddMember( Packet& p, const std::vector< T >& vec )
			{
				p.AddPlainData< Packet::MemberType >( Packet::MemberType::Vector );
				p.AddPlainData< unsigned int >( vec.size() );
				for( typename ConstRef< T >::type elem : vec )
				{
					p.AddMember< T >( elem );
				}
			}
		};

		template< typename T >
		void Packet::AddPlainData( typename ConstRef< T >::type t )
		{
			AddData( reinterpret_cast< const char* >( &t ), sizeof( typename Plain< T >::type ) );
		}

		template< typename T >
		typename Plain< T >::type Packet::GetPlainData()
		{
			typedef typename Plain< T >::type PlainT;
			PlainT result;
			std::memcpy( &result, GetData( sizeof( PlainT ) ), sizeof( PlainT ) );
			return result;
		}
		
		template< typename T >
		void Packet::AddMember( typename ConstRef< T >::type t )
		{
			SpecializedPacket< typename Plain< T >::type >::AddMember( *this, t );
		}

		template< typename T >
		typename Plain< T >::type Packet::GetNextMember()
		{
			return SpecializedPacket< typename Plain< T >::type >::GetNextMember( *this );
		}
	}
}

#endif //RPC_DETAIL_PACKET_HPP_INCLUDED
