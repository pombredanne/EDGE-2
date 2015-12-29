## To write / to do (would be nice to have)
- decals, lighting, shadowing, network, occlusion culling, cloud/gaming platform, multiplayer.
- backup (cloud, local, db)
- properties (key:v)
- resource (id, async)
- social (daily challenge, leaderboards)
- locale (lockit, utf8, time, date, money, ...)
- network (socket, bind, adhoc, server/client, p2p)
- resource (id, saync)
```
	async io, fault tolerant
	loading whole dataset is not always the best choice
	(see console games, dirty DVDs on last levels)
	vec<s> files;
	async(output = preload(files));
	bool finished = async.done();
	bool ok = f.size() == output.size()
```

```
if(debug && dt>10hz) dt = 30hz;
if(paused||minimized) dt = 10hz;
```

asset prj-wake | ~ps3 ~ps4 | p275
res/ -> bin/ | verify pack | pack

usr error -> gracefully, game ui
dev error -> stacktrace, coredumps, vars, registers, debug invoker, save state per frame

```
+-----+---------------------------+
| pic | dev error                 | [stack] [attach debugger]
+-----+ location [v] stack        | [load][save] [submit]
|                                 | [restore N frames and restart]
| [options ...]                   | [ignore] [forget *] [break]
+---------------------------------+

// forget
if( not FILE:LINE in cache ) show();
else if get.content(FILE:LINE) != cache(FILE:LINE) show();
```

loading/saving: json

events, triggers, volumes, messages: p775 p778

### 02- Pipes
### 02- Resource manager (memo) p283 p284
### 02- vfs, mounting
- load lvl 1-2 p290

persist storage
- sqlite3
- journey
- replication, sharding, cloud

api & errors
#define EE_MAKE_ERROR_STR(api_family, num_error, str) (str)
#define EE_MAKE_ERROR_NUM(api_family, num_error, str) ( (1<<31) | ((api_family)<<16) | (num_error) )
#define EE_MAKE_ERROR(api_family, num_error, str)
// list xmacro
#define EE_MAKE_ERROR EE_MAKE_ERROR_STR
#include xlist
EE_MAKE_ERROR(0x3, 0x1, "This is a generic error") -> 0x80030001

```
int ret = loadModule( const char *name );
if( ret < 0 ) printf("%s", strerror(ret));
if( ret > 0 ) id = ret;
```

ns
eeNetAdhocMatchingSelectTarget();
ee - ns
Net - modulo
AdhocMatching - libreria
SelectTarget - func

real samples {
	EE_NET_ADHOC_MATCHING_EVENT_HELLO = 1
	EE_NET_ADHOC_MATCHING_EVENT_REQUEST
	EE_NET_ADHOC_MATCHING_EVENT_LEAVE
	EE_NET_ADHOC_MATCHING_EVENT_DENY
	EE_NET_ADHOC_MATCHING_EVENT_CANCEL
	EE_NET_ADHOC_MATCHING_EVENT_ACCEPT
	EE_NET_ADHOC_MATCHING_EVENT_ESTABLISHED
	EE_NET_ADHOC_MATCHING_EVENT_TIMEOUT
	EE_NET_ADHOC_MATCHING_EVENT_ERROR
	EE_NET_ADHOC_MATCHING_EVENT_BYE
	EE_NET_ADHOC_MATCHING_EVENT_DATA
	EE_NET_ADHOC_MATCHING_EVENT_DATA_ACK
	EE_NET_ADHOC_MATCHING_EVENT_DATA_TIMEOUT
}

```
rand();
randd();
```

![image](http://i.stack.imgur.com/jaKUP.png)
ref: Game Engine Architecture by Jason Gregory

- no build system required: engine as dll. binary as player
- scriptable, multi lang
- package manager
- no editors required
- debuggable: time rewind
- lod by design, compact by design
- support proceduralism IN ALL LEVELS while(levels) {
  - procedural generators + decos -> level decorators
  - place by hand
  - }
 - clean api
 - quad layer: gui > fb > gui > fb; or fb > gui > fb > gui
 - desktop, mobile and web
- two layers: low-level api, high-level api
- bindable by default (C API)

## .drop content asset
1. mount filesystem
2. mount lan filesystem (w/ prj & user identification) (with runtime on/off toggle)
 - now you can drop assets in http_server/user/ or http_server/prj/ in runtime

## multiuser support
- object placement is put on level coordinates
- levels are put in world coordinates

## changes and dev events are pushed notifications
history log file format
- rewind
- seek
- diff history
  values cmd
m 100    +100
t 100    0
w 150    +50
T 50     /3
f 10     -40
s 90     +80
S 30     -60
today? 30
what did we do from f to S? -20 (-40+80-60)
simulate from w with 300 instead... 300 /3 -40 +80 -60 = 100

##  missing assets
a) if debug/release: dont assert. send mailto art team instead :o)
b) if debug/release: replace by highlighted placeholder (##UPPERCASE TEXT, (x) icon, finger geo, check texture, willhelm, rickroll, etc...)
c) if shipping: replace by null placeholder ("", blank icon, blank geo, blank texture, blank sound, blank video, etc)

### hid

vec1, vec2, vec3, vec4, mat4 (pose) + internal

buffering |sta|sta|sta|status|
polling -------read-------^

digital->ok
analog->IIR,lowpass,avg

1euro->gesture( interval[buff,t) ) -> returns: multimap< pct, gesture >: [94%, gesture1,gesture2], [90%, gesture3]
tap,up,down,tap2,on,off: require signal and t
chord: up(x, t0) && up(y, t0)
seq: in 200 ms { }
filters: buff avg(buff), buff lowpass(buff), buff iir(buff);
pipe: read(joy) >> invert(y) >> iir >> digitalize >> queue(buff)

monkey testing: digital[0,1, @frequency], analog[ -1.2,1.4, @frequency ]

cmd buffer
abs t|now|diff (logger cmd)
100| 50 | + 50
105| 60 | + 10
110|-25 | -85

### hid proposal

```
controller player1;
player1.use( mouse, 0 );
player1.use( keyboard, 0 );
player1["strafe-left"] = alias { keyboard[0]["a"], keyboard[0]["A"], clamp(0, -1, mouse.x) }; // or VK_A
player1["combo1"] = chord { ms(100), keyboard[0][VK_A], "fire2" };
player1["cheat"] = sequence { s(1), "a", "b", chord { "a" + "b" } };

if( hold(player1["strafe-left"]) ) {
   // bla bla
}
```

### hid metrics

player1 keyboard = vec1<bit>(256) = 32 bytes/frame * 120 frames/buffer = 3840 bytes/buffer
player1 mouse = vec2<float>(3) axes + vec1<float>(2) wheels + vec1<bit>(8) buttons = 24+8+1 = 33 bytes/frame * 120 frames/buffer = 3960 bytes/buffer

threaded sys for polling: frequency, reset, frame lock,

### renderer

hsla, rgba { pixels, textures, images }

### editor

ofxmanipulator

### cinematics

forced subtitles [on/off] { video, cinematics }

### logging, spdlog

buffering options: sync/async, flush, size
append to disk
fname: date.txt + rotating loggers :)
viewer -> display(read(tail(logfile))))

DEBUG(...)
TRACE(...)

format: [frame num %03d][milli %02d][sec %02d][thread #][entity #][level 0..9][func file line]%v

### ecs

obj.property becomes property[obj] (or for convenience: obj[property])

obj -= property; obj += property;
obj.dump(); obj.restore();

world->ecs; 1 world-> N systems; M components; O entities
system { c++ or lua } -> void update(t,dt)

### dynload

1st scripting, dynload, binding { and serialization }
2nd cli srv + RPC { queue/buffering, localhost tests, remote unittest }
3rd ecs + messages { remote/local messaging }

module {
	load()
	unload()
	func<bool()> init, quit, suspend, resume
	func<bool(t,dt)> tick; // dll or script
}

modules {
	init() : find and register all dlls and scripts into vm. do not load them
}

### pkg, .drop

module {
	"name",
	"version",
	"deps",
	init, quit, suspend, resume, tick, load, save
	"pak" or "toc": mime91...
}

rpc { "family", "func", argc, argv }

### trigger

begin, end, on, off

region #15 vs player ->
