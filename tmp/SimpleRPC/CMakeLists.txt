cmake_minimum_required(VERSION 2.8.8)

project(SimpleRPC)

# enable C++11
if(NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(NOT MSVC)

try_compile( IsTriviallyCopyableAvailable # result var
	"${PROJECT_BINARY_DIR}/cmake_try_compile" # bin dir
	"${CMAKE_SOURCE_DIR}/cmake_platformcheck/is_trivially_copyable_template.cpp" # source file
	OUTPUT_VARIABLE IsTriviallyCopyableErrors
	)
try_compile( HasTrivialCopyAvailable # result var
	"${PROJECT_BINARY_DIR}/cmake_try_compile" # bin dir
	"${CMAKE_SOURCE_DIR}/cmake_platformcheck/has_trivial_copy_function.cpp" # source file
	OUTPUT_VARIABLE HasTrivialCopyErrors
	)

# see if is_trivially_copyable<> is available
if( IsTriviallyCopyableAvailable )
	set( IsTriviallyCopyableDefinition "( std::is_trivially_copyable< T >::value )" )
# see if __has_trivial_copy() is available instead
elseif( HasTrivialCopyAvailable )
	message( "std::is_trivially_copyable<> not available, using __has_trivial_copy() instead." )
	set( IsTriviallyCopyableDefinition "__has_trivial_copy( T )" )
else( IsTriviallyCopyableAvailable )
	# unavailable, can't check...
	message( "neither std::is_trivially_copyable<> nor __has_trivial_copy() available, use with caution!" )
	set( IsTriviallyCopyableDefinition "true
#warning is_trivially_copyable<> is not available, running without safety net!"
		)
endif( IsTriviallyCopyableAvailable  )

# configure compiler/platform specific stuff (is_trivially_copyable)
configure_file( "${CMAKE_SOURCE_DIR}/cmake_template/platform.hpp" "${CMAKE_SOURCE_DIR}/include/rpc/detail/" )

include_directories("${CMAKE_SOURCE_DIR}/include")

add_library(SimpleRPC STATIC
	"include/rpc/api.hpp"
	"include/rpc/exceptions.hpp"
	"include/rpc/receivers.hpp"
	"include/rpc/senders.hpp"
	
	"include/rpc/detail/helpers.hpp"
	"include/rpc/detail/packet.hpp"
	"include/rpc/detail/receiver.hpp"
	"include/rpc/detail/sender.hpp"
	
	"include/rpc/detail/platform.hpp" # configured by cmake
	
	"source/api.cpp"
	"source/packet.cpp"
	"source/receiver.cpp"
	"source/sender.cpp"
	)

include_directories("${CMAKE_SOURCE_DIR}/test")
add_executable(SimpleRPC_Test
	"test/main.cpp"
	"test/rpcReceivers.cpp"
	"test/rpcSenders.cpp"
	"test/rpcDefinitions.hpp"
	)
target_link_libraries(SimpleRPC_Test SimpleRPC)
