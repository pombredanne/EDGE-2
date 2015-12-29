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




