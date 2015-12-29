#include "rpc/detail/sender.hpp"
namespace RPC
{
	namespace detail
	{
		unsigned int CommonSender::s_index = 0;

		CommonSender::CommonSender()
		: m_index( s_index++ )
		{
		}

		CommonSender::~CommonSender()
		{
		}
	}
}
