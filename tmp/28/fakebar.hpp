#pragma once
#include <stdlib.h>
#include <chrono>
#include <random>

class fakebar {

	double current = 0;
	double at = 0;
	double next = 0;

	double raw() const {
		return std::chrono::system_clock::now().time_since_epoch().count();
	}

	double now() const {
		return raw() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
	};

    std::chrono::system_clock::rep seed = raw();
    std::default_random_engine generator = std::default_random_engine( seed );
    std::uniform_real_distribution<double> distribution = std::uniform_real_distribution<double>( 0.0, 1.0 );

	double clamp( double x ) {
		return x < 0 ? 0 : ( x > 100 ? 100 : x );
	}

	double rand( double from, double to ) {
	    return from + distribution(generator) * (to - from);
	}

public:

	fakebar() 
	{}

	float progress() {
		if( now() > next ) {
			push();
		}
		double speed = ( at >= 100 ? 0.03 : 0.003 );
		return current = current * ( 1 - speed ) + at * speed;
	}

	void push() {
		next = now() + rand(0,2);
		at = clamp( at + rand(0,5) );
	}

	void progress( float percent ) {
		next = now() + rand(0,2);
		at = clamp( percent );
	}

	void finish() {
		next = now() + rand(0,2);
		at = clamp( 100 );
	}
};
