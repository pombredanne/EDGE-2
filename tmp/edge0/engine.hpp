#ifndef EE_ENGINE_H
#define EE_ENGINE_H

namespace ee {

bool init( int argc, const char **argv );
bool restart();
bool quit();

// singletons
extern struct renderer   *gfx;
extern struct input      *hid;
extern struct script     *env;
extern struct window     *wnd;
extern struct logger     *log;
extern struct timer      *tme;
extern struct filesystem *fs;
}

#endif