#ifndef EE_INPUT_H
#define EE_INPUT_H

namespace ee {

struct input {
    virtual ~input() {};

    virtual void process( bool has_focus ) = 0;

    virtual void terminate( bool enabled ) = 0;
    virtual bool terminated() = 0;

    virtual void restart( bool enabled ) = 0;
    virtual bool restarted() = 0;

    virtual void debug( bool enabled ) = 0;
    virtual bool debugged() = 0;

    virtual int mouse_x() = 0;
    virtual int mouse_y() = 0;
    virtual int mouse_left() = 0;
    virtual int keyboard( int keycode ) = 0;
};

}

#endif