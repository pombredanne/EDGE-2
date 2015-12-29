## Day 04

### Entity/component/systems

proposal:

```
lightswitch {
    pointlight { intensity: 3000, visible:true },
    sphere { radius: 250.0, parent: root,
        on: [other]{ if(other==player) visible^=true; } },
        off: [other]{ if(other==player) visible^=true; } },
}
pawn {
    movement {}
    collision {}
    mesh {}
}
```

systems
w/ timer
ie, hid, timer, render, etc...

enable, disable : paused and buffering
pause, resume : paused and !buffering

queue messages

classes: negative bit
elements: positive bit

what, when, how, who
triggers (where), ifs-conditions (when), actions []{} (what)

where
damage volume
blocking volume
camera blocking volume
physics volume
trigger volume
kill volume
level streaming volume
fog volume
light volume
...

when
alter components by volume ... (gravity)
alter systems by volume ... (lighting)
alter entities by volume ... (player)
alter tags by volume ... any_of(team), all_of(team), none_of(team)

actor/pawn
    ecs
    ai { emitter, sensing } { data events + transform (where)(when) -> [smell][hunger],... }


entity (id)
    id {
        class, <0
        invalid, 0
        instance, >0
    }
    id {
        remote < half,
        local >= half,
    }

system (prio) (C++/Lua)
    01-damage []{}
    02-movement[]{}
    03-render[]{}
    04-audio[]{}

class/component matrix (csv or lua)
                             system movement
                        /---------------------\
    item       | light | position | velocity
    -----------|-------|----------|-----------------
    light01_t  | {white,300k} | {0,0,0} |
    enemy01_t | | { 0,0,0 } | { 0,0,0 }
