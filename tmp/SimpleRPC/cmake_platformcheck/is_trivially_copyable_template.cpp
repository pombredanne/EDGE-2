#include <type_traits>

struct MyPOD
{
	int x;
};

static_assert( std::is_trivially_copyable< MyPOD >::value,
	"is_trivially_copyable does not work correctly!" );

int main( int argc, char** argv )
{
	return 0;
}
