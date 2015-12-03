#pragma once

// return error codes
// <0, error
//  0, ok, no result
// >0, ok, result

enum {
    EE_ERR_INVALID_ARGUMENT = -1,
    EE_ERR_INVALID_DEVICE = -2,
    EE_ERR_INVALID_STATUS = -3,
    EE_ERR_NO_DATA = -4,
    EE_ERR_NO_SUCH_FILE_OR_DIRECTORY = -5,
    EE_ERR_NO_ENOUGH_MEM = -6,
    EE_ERR_NO_EMPTY = -7,
};

