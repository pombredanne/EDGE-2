#pragma once

#include "PackedArray/PackedArray.h"
#include <memory>
#include <cassert>

namespace {
	enum {
		int1_t = 1,
		int2_t = 2,
		int3_t = 3,
		int4_t = 4,
		int5_t = 5,
		int6_t = 6,
		int7_t = 7,
		int9_t = 9,
		int10_t = 10,
		int11_t = 11,
		int12_t = 12,
		int13_t = 13,
		int14_t = 14,
		int15_t = 15,
		int17_t = 17,
		int18_t = 18,
		int19_t = 19,
		int20_t = 20,
		int21_t = 21,
		int22_t = 22,
		int23_t = 23,
		int24_t = 24,
		int25_t = 25,
		int26_t = 26,
		int27_t = 27,
		int28_t = 28,
		int29_t = 29,
		int30_t = 30,
		int31_t = 31,

		uint1_t = int1_t,
		uint2_t = int2_t,
		uint3_t = int3_t,
		uint4_t = int4_t,
		uint5_t = int5_t,
		uint6_t = int6_t,
		uint7_t = int7_t,
		uint9_t = int9_t,
		uint10_t = int10_t,
		uint11_t = int11_t,
		uint12_t = int12_t,
		uint13_t = int13_t,
		uint14_t = int14_t,
		uint15_t = int15_t,
		uint17_t = int17_t,
		uint18_t = int18_t,
		uint19_t = int19_t,
		uint20_t = int20_t,
		uint21_t = int21_t,
		uint22_t = int22_t,
		uint23_t = int23_t,
		uint24_t = int24_t,
		uint25_t = int25_t,
		uint26_t = int26_t,
		uint27_t = int27_t,
		uint28_t = int28_t,
		uint29_t = int29_t,
		uint30_t = int30_t,
		uint31_t = int31_t,
	};
}

template<unsigned bits_per_item>
class pa {

PackedArray* a;

struct accessor {
	PackedArray *a;
	int index;

	accessor &operator=( const uint32_t &other ) {
		return PackedArray_set( a, index, other ), *this;
	}
	operator uint32_t() {
		return PackedArray_get( a, index );
	}
};

public:

pa() : a(0) {

}

pa( unsigned count ) : a(0) {
	assert( bits_per_item < 32 );
	a = PackedArray_create( bits_per_item, count );
	assert( a );
}

~pa() {
	if( a ) PackedArray_destroy(a), a = 0;
}

size_t size() const {
	return a ? PackedArray_bufferSize(a) : 0;
}
bool empty() const {
	return 0 == size();
}

void resize( int newcount ) {
	size_t count = size();
	if( newcount < count ) {
		return;
	} 
	if( empty() ) {
		*this = pa( newcount );
	} 
	else {
		assert( a );
		assert( count > 0 );

		uint32_t *bak = new uint32_t [count];
		assert( bak );

		PackedArray_unpack( a, 0, bak, count );

		PackedArray_destroy( a );
		a = PackedArray_create( bits_per_item, newcount );
		assert( a );

		PackedArray_pack(a, 0, bak, count);
		count = newcount;

		delete [] bak;
	}
}

uint32_t operator[]( int index ) const {
	return PackedArray_get( a, index );
}

accessor operator[]( int index ) {
	return accessor { a, index };
}

};