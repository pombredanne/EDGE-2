#ifndef RPC_RECEIVER_HPP_INCLUDED
#define RPC_RECEIVER_HPP_INCLUDED

#include <type_traits>
#include <vector>

#include "packet.hpp"
#include "helpers.hpp"

namespace RPC
{
	namespace detail
	{
		/// Receiver Interface, so a list can be kept
		class IReceiver
		{
		public:
			static Packet Receive( unsigned int index, Packet& input );
		protected:
			IReceiver();
			virtual ~IReceiver();

			virtual void Receive( Packet& input, Packet& output ) = 0;
		private:
			IReceiver( IReceiver&& ) = delete;
			IReceiver( const IReceiver& ) = delete;
			IReceiver& operator=( const IReceiver& ) = delete;
		};

		/// Shared Receiver Code
		template< typename RetType, typename... Parameters >
		class ActualReceiver : public IReceiver
		{
		public:
			typedef RetType( *Funcptr )( Parameters... );
		protected:
			ActualReceiver( Funcptr ptr ) : m_ptr( ptr ) {}
			void Receive( Packet& input, Packet& output );
		private:
			void HandleArguments( Packet& input, Packet& output, TypeList<>, Parameters... parameters );
			template< typename Head, typename... Tail, typename... Arguments >
			void HandleArguments( Packet& input, Packet& output, TypeList< Head, Tail... >, Arguments... arguments );

			const Funcptr m_ptr;
		};



		//    Receiver class - handles Parameters being (void)

		template< typename RetType, typename... Parameters >
		class Receiver : public ActualReceiver< RetType, Parameters... >
		{
		public:
			Receiver( typename ActualReceiver< RetType, Parameters... >::Funcptr ptr ) : ActualReceiver< RetType, Parameters... >( ptr ) {}
		};

		template< typename RetType >
		class Receiver< RetType, void > : ActualReceiver< RetType >
		{
		public:
			Receiver( typename ActualReceiver< RetType >::Funcptr ptr ) : ActualReceiver< RetType >( ptr ) {}
		};

		// Return value handler - handles return value being void
		template< typename RetType, typename... Parameters >
		struct ResultStorer
		{
			typedef RetType( *Funcptr )( Parameters... );
			static void Call( Packet& output, Funcptr ptr, Parameters... parameters )
			{
				// function with return value, send it back!
				output.AddMember< RetType >( ptr( parameters... ) );
			}
		};

		template< typename... Parameters >
		struct ResultStorer < void, Parameters... >
		{
			typedef void( *Funcptr )( Parameters... );
			static void Call( Packet& output, Funcptr ptr, Parameters... parameters )
			{
				// no return value, just call function with parameters.
				ptr( parameters... );
			}
		};

		//    Implementation

		template< typename RetType, typename... Parameters >
		void ActualReceiver< RetType, Parameters... >::HandleArguments( Packet& input, Packet& output, TypeList<>, Parameters... parameters )
		{
			ResultStorer< RetType, Parameters... >::Call( output, m_ptr, parameters... );
			
		}

		template< typename RetType, typename... Parameters >
		template< typename Head, typename... Tail, typename... Arguments >
		void ActualReceiver< RetType, Parameters... >::HandleArguments( Packet& input, Packet& output, TypeList< Head, Tail... >, Arguments... arguments )
		{
			typedef typename Plain< Head >::type PlainArgument; 
			PlainArgument arg( input.GetNextMember< PlainArgument >() );
			HandleArguments( input, output, TypeList< Tail... >(), arguments..., arg );
			// non-const reference?
			if( std::is_reference< Head >::value && !std::is_const< typename std::remove_reference< Head >::type >::value )
			{
				// That needs to be written back, send it back!
				output.AddMember< PlainArgument >( arg );
			}
		}

		template< typename RetType, typename... Parameters >
		void ActualReceiver<RetType, Parameters... >::Receive( Packet& input, Packet& output )
		{
			HandleArguments( input, output, TypeList< Parameters... >( ) );
		}
	}
}

#endif // RPC_RECEIVER_HPP_INCLUDED
