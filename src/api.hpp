#pragma once
#include <stdint.h>
#include <string>
#include <functional>
#include <vector>

// API schema:
//
// int eeBase91Encode( std::string &out, const std::string &input );
//  |  | |     |       |                 |
//  |  | |     |       |                 +----> read-only args: const arguments, read-only inputs
//  |  | |     |       +----------------------> writeable args: non-const arguments, writeable outputs
//  |  | |     +------------------------------> function: action, start with infinitive verb always: get, set, update...
//  |  | +------------------------------------> namespace: subsystem, sustantive always: network, disk, memory...
//  |  +--------------------------------------> edge prefix: `ee` always
//  +-----------------------------------------> result code: integer always; error(<0), ok(0), ok and id(>0)

// IDs:
//  0, invalid
// >0,   valid

// Messages:
// message = pair< int argc, const char **argv >
//
// Opcodes:
// opcode = message.argv[0]

enum eeResultCodes {
    EE_ERR_INVALID_ARGUMENT = -1,
    EE_ERR_INVALID_DEVICE = -2,
    EE_ERR_INVALID_STATUS = -3,
    EE_ERR_NO_DATA = -4,
    EE_ERR_NO_SUCH_FILE_OR_DIRECTORY = -5,
    EE_ERR_NO_ENOUGH_MEM = -6,
    EE_ERR_NO_EMPTY = -7,
};

struct eeGameState {
    std::vector<void*> stack;
    uint64_t time;
    uint64_t delta;
    uint64_t framerate;
    float    lod;
    float    frustum[6*4];
};

// custom aliases
using str = std::string;       template<typename T>
using vec = std::vector<T>;    

// base91
int eeBase91Encode( std::string &out, const std::string &input );
int eeBase91Decode( std::string &out, const std::string &input );

// mime detect
int eeMimeDetect( std::string &extension, std::string &mime_type, const std::string &data );

// watchdog
int eeWatchdogMonitorAddings( const std::string &uri, const std::function<void(const std::string &)> &callback );
int eeWatchdogMonitorRemoves( const std::string &uri, const std::function<void(const std::string &)> &callback );
int eeWatchdogMonitorTouches( const std::string &uri, const std::function<void(const std::string &)> &callback );

// netstring
int eeNetstringEncode( std::string &out, const std::string &input );
int eeNetstringDecode( std::string &out, const std::string &input );

// game loop
int eeGameInit();
int eeGameQuit();
int eeGameUpdate( eeGameState *state, uint64_t t, uint32_t dt );
int eeGameRender( const eeGameState *state0, const eeGameState *state1, float delta01 );

// module
int eeModuleGetApplicationPath( std::string &out );
int eeModuleGetLibraryPath( std::string &out );
