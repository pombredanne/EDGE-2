#ifndef RPC_EXCEPTIONS_HPP_INCLUDED
#define RPC_EXCEPTIONS_HPP_INCLUDED

#include <stdexcept>

namespace RPC
{
	struct EndOfPacketException : std::runtime_error
	{
		EndOfPacketException( const std::string& what = "Unexpected End of Packet" ) : std::runtime_error( what ) {}
	};

	struct InvalidParameterException : std::runtime_error
	{
		InvalidParameterException( const std::string& what = "Unexpected Parameter Type" ) : std::runtime_error( what ) {}
	};

	struct UnexpectedPacketException : std::runtime_error
	{
		UnexpectedPacketException( const std::string& what = "Unexpected Packet Type" ) : std::runtime_error( what ) {}
	};

	struct RPCException : std::runtime_error
	{
		RPCException( const std::string& what ) : std::runtime_error( what ) {}
	};

	struct InvalidIndexException : std::logic_error
	{
		InvalidIndexException( const std::string& what = "Invalid Index" ) : std::logic_error( what ) {}
	};

	struct NoSenderRegisteredException : std::logic_error
	{
		NoSenderRegisteredException( const std::string& what = "No sender registered" ) : std::logic_error( what ) {};
	};
}

#endif // RPC_EXCEPTIONS_HPP_INCLUDED
