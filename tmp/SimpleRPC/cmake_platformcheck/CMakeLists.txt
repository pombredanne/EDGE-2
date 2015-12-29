cmake_minimum_required(VERSION 2.8.8)

# check for availability of features
project(CMake_PlatformChecks)

# enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable( IsTriviallyCopyableTemplate
	"is_trivially_copyable_template.cpp"
	)
add_executable( HasTrivialCopyFunction
	"has_trivial_copy_function.cpp"
	)
