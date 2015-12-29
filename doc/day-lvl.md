## Day XX

### XXX

double ended allocator p211 p291

|---rw-stack1--->      <----rw-stack2---|
|--ro-lvl-->               <--rw-stack--|

save: copy stack -> safe place
init: init & save stack to disk
restart: copy safe -> stack
unload: reset stack pointer
before/after update (movie record): copy stack to disk

align p212

single frame, double frame p213

ue4 model: level persist, level streamable
world
    - lvl 1: persist + 3 zones
    - lvl 2: persist + 5 zones
    - lvl 3: persist + 6 zones
    ie, lvl 1 = 1 persist + 3 zones
    +------------+
    |  \xxx 1 xxx|
    | P \________|
    |____     ___|
    |x 2x\   /x3x|
    +------------+
    level edges, volumes or doors -> stream un/load trigger
also,
    world optimization
    |all shared|shared lvl1+lvl2|common lvl1| --- after loading lvl 1
    |all shared|shared lvl1+lvl2|common lvl2| --- after loading lvl 2
    |all shared|common lvl3|                  --- after loading lvl 3

also, what about minecraft? fully procedural


load, merge, restart

