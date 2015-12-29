#include <iostream>
using std::cout;
using std::endl;

#define MSGPACK_INLINE
#include "msgpackalt.hpp"
using namespace msgpackalt;

typedef std::map<std::string, package> pack_dict;

int main( )
{
	packer packd;
	pack_dict d1, d2;
	
	// create a simple dictionary
	d2["foo"] << -65537;
	d2["yyz"] << 3.141f;
	// pack it into another dictionary
	d1["test"] << "foobar";
	d1["bar"] << -(1ll<<33);
	d1["msg"] << d2;
	packd << d1;
	
	// turn it into a string and print it
	std::string str = packd.string();
	cout << "*** Demonstrating nested dicts ***" << endl;
	for ( std::string::const_iterator i = str.begin(); i != str.end(); ++i )
		printf( "%02x", ( byte )*i ); /* that's just annoying to do with iostream */
	cout << endl << endl;
	
	// unpack the nested dicts and print their key names
	unpacker unpackd( str );
	pack_dict u1, u2;
	unpackd >> u1;
	u1["msg"] >> u2;
	
	// print the keys to screen
	cout << "dict u1 contains entries ";
	for ( pack_dict::const_iterator i = u1.begin(); i != u1.end(); ++i )
		cout << "'" << i->first << "' ";
	cout << endl << "dict u2 contains entries ";
	for ( pack_dict::const_iterator i = u2.begin(); i != u2.end(); ++i )
		cout << "'" << i->first << "' ";
	cout << endl << endl;
	
	// extract individual elements
	int64_t foo; int64_t bar; double yyz; std::string test;
	u1["test"] >> test;
	cout << "test = " << test << endl;
	u1["bar"] >> bar;
	cout << "bar  = " << bar << endl;
	u2["foo"] >> foo;
	cout << "foo  = " << foo << endl;
	u2["yyz"] >> yyz;
	cout << "yyz  = " << yyz << endl;
	return 0;
}
