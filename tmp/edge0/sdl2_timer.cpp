#ifndef EE_TIMER_HPP
#define EE_TIMER_HPP

#include <stdint.h>

namespace ee {

struct timer {
    virtual ~timer() {}
    virtual uint64_t time() = 0;
};

}

#endif
