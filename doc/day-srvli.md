## Day 09

### Server/client architecture

PTTH --> auth / salt / public key --> proxy <--> proxy-server

{ protocol, depot, revisions }

python server?

2 ports: p+0 binary, p+1 text

client
connect();
for(;;){
    send(now(),exec(recv()),inputs());
}

send: t, state, inputs
recv: rpc call (argc, argv) --> binding, serialization

modules <== bindings at rpc
dynload, dynclose, dynlist, dynget

server
bind();
for(;;) {
    on(connected()) {
        switch(send(ver)) {
        case 200: ok
        case 100: err
        }
    }
    yield();
}
