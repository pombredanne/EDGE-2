#include <type_traits>

struct MyPOD
{
	int x;
};

static_assert( __has_trivial_copy( MyPOD ),
	"is_trivially_copyable does not work correctly!" );

int main( int argc, char** argv )
{
	return 0;
}
