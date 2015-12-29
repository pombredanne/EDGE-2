## Day XX

### XXX

cinematics, igual q video pero allow QTE inputs

```
composite(all active layers, target_texture);
renderer->full_screen_quad(target_texture)
```

example:

[active: onoff] + video layer 1
[active: onoff] + video layer 2
[active: onoff] + video layer 3
...

[active: onoff] + cinematic layer 1
[active: onoff] + cinematic layer 2
[active: onoff] + cinematic layer 3
...

[active: onoff] + qte input layer 1
[active: onoff] + qte input layer 2
[active: onoff] + qte input layer 3
...

[active: onoff] + subtitle layer 1
[active: onoff] + subtitle layer 2
[active: onoff] + subtitle layer 3
...


api

parse, play, pause, stop, render(texture), render(gfxengine)
get subtitle(xx:xx)
aspect ratio, squeeze
load, merge(type, layer)
seek frame, seek second, play, stop, speed(-32..0..1..32)
get frames
get length (seconds)
toggle(type, layer, onoff)
