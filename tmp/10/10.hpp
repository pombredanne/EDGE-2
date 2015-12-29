#pragma once
#include <stdint.h>
#include <string>

int eeTimeGetClock( uint64_t &t );
int eeTimeGetStamp( std::string &stamp, const uint64_t &t );

