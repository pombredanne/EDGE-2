#ifndef RPC_DETAIL_HELPERS_HPP_INCLUDED
#define RPC_DETAIL_HELPERS_HPP_INCLUDED

namespace RPC
{
	namespace detail
	{
		/// Empty struct that just carries type information for function overloading
		template< typename... Types >
		struct TypeList {};

		/// Remove const and reference
		template< typename T >
		struct Plain
		{
			typedef typename std::remove_const< typename std::remove_reference< T >::type >::type type;
		};

		/// Add const and reference
		template< typename T >
		struct ConstRef
		{
			typedef typename std::add_lvalue_reference< typename std::add_const< T >::type>::type type;
		};
	}
}

#endif //RPC_DETAIL_HELPERS_HPP_INCLUDED
