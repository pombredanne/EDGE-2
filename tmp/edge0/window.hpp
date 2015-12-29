#ifndef EE_WINDOW_HPP
#define EE_WINDOW_HPP

namespace ee {

struct window {

    virtual ~window() {}
    virtual bool open(int w, int h) = 0;
    virtual int width() = 0;
    virtual int height() = 0;

    virtual bool is_minimized() = 0;
    virtual bool is_maximized() = 0;
    virtual bool is_fullscreen() = 0;
    virtual bool is_fullscreen_desktop() = 0;
    virtual bool is_visible() = 0;
    virtual bool is_resizable() = 0;
    virtual bool is_borderless() = 0;
    virtual bool has_input_focus() = 0;
    virtual bool has_input_grabbed() = 0;
    virtual bool has_mouse_focus() = 0;
    virtual bool has_mouse_captured() = 0;

    // private
    int w, h;
};

}

#endif