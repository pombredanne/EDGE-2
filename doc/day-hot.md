## Day 07

### Hot-reloading

We will serialize all systems and active classes to disk, then we will restart application and restore all flushed states.

This is our previous game loop (simplified):

```
void loop() {
    update();
    render();
}
int main() {
    if( init() ) {
        loop();
        quit();
    }
}
```

This is roughly the new game loop:

```
void loop() {
    update();
    render();
    if( hotreload ) {
        hotreload = 0;
        for all entities {
            save to disk
        }
        for all systems {
            hibernate
        }
        game {
            quit
            init
        }
        for all systems {
            activate
        }
        for all entities {
            load from disk
        }
    }
}
int main() {
    if( init() ) {
        loop();
        quit();
    }
}
```

### Watchdog

A watchdog library is that one that notifies you about local changes on the monitored files you are watching. I do not know if I will be using this, but it is always handy, specially when monitoring shaders and other graphic assets.

I created quickly a Watchdog library that builds on top of Apathy. The library can be found in the [watchdog/](../src/watchdog) folder.

I could have used the original library (where I took the inspiration from) [Watchdog library](https://github.com/simongeilfus/Watchdog), or maybe [EFSW](https://bitbucket.org/SpartanJ/efsw) instead.
