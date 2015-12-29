#ifndef RPC_SENDER_HPP_INCLUDED
#define RPC_SENDER_HPP_INCLUDED

#include "packet.hpp"
#include <utility>
#include <type_traits>

namespace RPC
{
	namespace detail
	{
		Packet SendMessage( Packet&& message ); // defined in api.cpp

		class CommonSender
		{
		protected:
			CommonSender();
			~CommonSender();

			const unsigned int m_index;
		private:
			static unsigned int s_index;
		};

		// ActualSender - actual implementation(s) of Sender variants

		template< typename RetType, typename... Parameters >
		class Sender : public CommonSender
		{
		public:
			/**
				@brief Remote Procedure Call

				Make sure to call RegisterSender() first.
			**/
			RetType operator()( Parameters... parameters );

			static_assert( !std::is_reference< RetType >::value, "Can't Serialize function returning a reference!" );
			static_assert( !std::is_pointer< RetType >::value, "Can't Serialize function returning a pointer!" );

		private:
			typename std::pair< RetType, Packet > HandleArguments( Packet& toSend);

			template< typename Head, typename... Tail >
			typename std::pair< RetType, Packet > HandleArguments( Packet& output, Head head, Tail... tail );
		};

		// Spezialization for no return value
		template< typename... Parameters >
		class Sender< void, Parameters... > : public CommonSender
		{
		public:
			void operator()( Parameters... parameters );
		};

		// Spezialization for no parameters, expressed as (void)
		template< typename RetType >
		class Sender< RetType, void > : public Sender< RetType >{};

		// Spezialization for no parameters and no return value
		template<>
		class Sender< void, void > : public Sender< void >{};


		//        Implementation

		//    with return value

		// no more arguments
		template< typename RetType, typename... Tail >
		struct ArgumentsHandler
		{
			static typename std::pair< RetType, Packet > Handle( Packet& toSend )
			{
				Packet result = SendMessage( std::move( toSend ) );
				RetType retVal = result.GetNextMember< RetType >();
				return std::make_pair< RetType, Packet >( std::move( retVal ), std::move( result ) );
			}
		};

		// argument, but no non-const reference
		template< typename RetType, typename Head, typename... Tail >
		struct ArgumentsHandlerNoNonConstRef
		{
			static typename std::pair< RetType, Packet > Handle( Packet& output, Head head, Tail... tail )
			{
				output.AddMember< Head >( head );
				return ArgumentsHandler< RetType, Tail... >::Handle( output, tail... );
			}
		};

		// Non-reference Argument
		template< typename RetType, typename Head, typename... Tail >
		struct ArgumentsHandler< RetType, Head, Tail... >
			: ArgumentsHandlerNoNonConstRef< RetType, Head, Tail... >
		{};

		// Const-reference Argument
		template< typename RetType, typename Head, typename... Tail >
		struct ArgumentsHandler< RetType, const Head&, Tail... >
			: ArgumentsHandlerNoNonConstRef< RetType, const Head&, Tail... >
		{};

		// Non-const reference Argument
		template< typename RetType, typename Head, typename... Tail >
		class ArgumentsHandler< RetType, Head&, Tail... >
		{
			static typename std::pair< RetType, Packet > Handle( Packet& output, Head & head, Tail... tail )
			{
				output.AddMember< Head >( head );
				typename std::pair< RetType, Packet > results = ArgumentsHandler< RetType, Tail... >::Handle( output, tail... );
				head = results.second.template GetNextMember< Head >();
				return results;
			}
		};

		template< typename RetType, typename... Parameters >
		RetType Sender< RetType, Parameters... >::operator()( Parameters... parameters )
		{
			Packet output( Packet::MakeEmptyCall( m_index ) );
			return ArgumentsHandler< RetType, Parameters... >::Handle( output, parameters... ).first;
		}

		//    no return value

		// no more arguments
		template< typename... Tail >
		struct ArgumentsHandlerNoReturn
		{
			static Packet Handle( Packet& toSend )
			{
				return SendMessage( std::move( toSend ) );
			}
		};

		// argument, but no non-const reference
		template<  typename Head, typename... Tail >
		struct ArgumentsHandlerNoReturnNoNonConstRef
		{
			// Argument, but not a non-const reference
			static Packet Handle( Packet& output, Head head, Tail... tail )
			{
				output.AddMember< Head >( head );
				return ArgumentsHandlerNoReturn< Tail... >::Handle( output, tail... );
			}
		};

		// Non-reference Argument
		template< typename Head, typename... Tail >
		struct ArgumentsHandlerNoReturn< Head, Tail... >
			: ArgumentsHandlerNoReturnNoNonConstRef< Head, Tail... >
		{
		};

		// Const-reference Argument
		template< typename Head, typename... Tail >
		struct ArgumentsHandlerNoReturn< const Head &, Tail... >
			: ArgumentsHandlerNoReturnNoNonConstRef< const Head&, Tail... >
		{
		};

		// Non-const reference Argument
		template< typename Head, typename... Tail >
		struct ArgumentsHandlerNoReturn< Head& , Tail... >
		{
			// Non-const reference argument
			static Packet Handle( Packet& output, Head& head, Tail... tail )
			{
				output.AddMember< Head >( head );
				Packet results = ArgumentsHandlerNoReturn< Tail... >::Handle( output, tail... );
				head = results.GetNextMember< Head >();
				return results;
			}
		};

		template< typename... Parameters >
		void Sender< void, Parameters... >::operator()( Parameters... parameters )
		{
			Packet output( Packet::MakeEmptyCall( m_index ) );
			ArgumentsHandlerNoReturn< Parameters... >::Handle( output, parameters... );
		}
	}
}

#endif // RPC_SENDER_HPP_INCLUDED
