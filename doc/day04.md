## Day 04

Ok, so we have to cover scripting, binding and dynamic modules now.

I plan to

### Scripting
### Binding
### Modules
dynload/lua
see systems: every sys has own timer

layer {
    init -> tick, render, ... -> tick, render, ... -> quit
}

module {
    init(options ...)
    drop()

    suspend()
    resume()
    tick(t,dt)

    load(istream)
    save(ostream)

    set(var, val)
    get(var)
    rpc(fn, args ...)
}

vm : module {
    bind(name, lambda)

    bool compile(script)
    bool link()

    eval(script, timeout);
}

binding
- "C" api
- dynload: fn = dynload("dll", "fn", int, int);
- fn(1,2)
