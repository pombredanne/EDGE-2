#include <iostream>
#include <string>

#include "rpc/api.hpp"
#include "rpc/senders.hpp"

void HelloWorld()
{
	std::cout << "Hello World!" << std::endl;
}

void HelloI( int i )
{
	std::cout << "Hello " << i << std::endl;
}

int HelloStr( const std::string& str )
{
	std::cout << "Hello " << str << std::endl;
	return 42;
}

int Get42()
{
	return 42;
}

int GetI( const int i )
{
	return i;
}

void GetRef( int& ref )
{
	ref = 42;
}

int main( int argc, char** argv )
{
	// Register Loopback
	RPC::RegisterSender( RPC::OnReceive );
	if( false )
	{
	}
	std::cout << "RPC::HelloWorld() -> ";
	RPC::HelloWorld();
	std::cout << "Get42() -> " << RPC::Get42() << std::endl;
	std::cout << "GetI( 1337 ) -> " << RPC::GetI( 1337 ) << std::endl;
	std::cout << "HelloI( -42 ) -> ";
	RPC::HelloI( -42 );
	int i = -1;
	RPC::GetRef( i );
	std::cout << "GetRef() -> " << i << std::endl;
	std::cout << R"(HelloStr( "RPC" ) -> )";
	RPC::HelloStr( "RPC" );

	std::string ignored;
	std::getline( std::cin, ignored );
	return 0;
}
