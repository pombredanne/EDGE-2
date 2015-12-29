## Day XX

### XXX

debug toolkit
event {
  click analog 0..1
  hover bool 0..1
  in, out, hot,
  relative widget x, y,
  relative parent x, y,
  screen absolute x, y,
}
if( const event e = slider("bla bla") ) {
    if( e.click && e.hover ) {
      volume = e.widget.relative_x * 100;
    }
}

resmanager
```
if( fail(res) ) {
    add_debug_draw( text3d, pos, secs(5), name(res) );
}
```

shapes
    line ---x-----x-----
    segment x------x
    arrow, ray x------>
    points in line ---x------x----
    axes y w/ lines, captions
         |/z
    -----+-----x
        /|
    points, ~= axes, w/small segments, no captions
    planes
    sphere, circle
    box, square      x
    capsule          |
    formated text   _|_
    coordinates   // | \\
                  \\___//

attributes
    fade time (alpha)
    color, thickness, radius, quality,
    depth test on/off
    world/screen space

logging, queue, max draw calls

menu ingame: { group, sub, label } control { keyboard, mouse, gamepad, cursors }
checkbox bool, slider --[]---, range []---[],
label
text input, text area, color hsv/hsl/rgb, button [ ok ], button2 [ ok | fail ], button 3 [ ok | retry | fail],
[][][] matrix
[][][]
[][][]    |
graph ----+----
          |
histogram
      []
  []  []
[][][][][]
tooltip p380

console ingame

debug cameras: fly, detach, attach, drop player, drop ai, drop item

game control: pause/resume, step, tempo ( -100..1.. 0 pause .. 1..2..4..8..16..32..100), dump/restore, inspect

control npc: impersonate ai / input, AI : edit commands

cheats: invencible, weapons, ammo, items, location, buy/money, select player, select world, select level, change attr

fastpath: edge.exe [game] [level] [key=val attr] [...]

screenshots & videos: no hud, no debug draw, yes watermark.png top left bottom right center, yes logos, yes hires, optional text
videos: savegame + begin camera + end camera + time (travelling) -> save as mjpeg

profiling csv
frame | time | file:line | file:line
0     | 13 ms | 9 ms | 12 ms
1     | 16 ms | 2 ms | 3 ms |
2     | 84 ms | 3 ms | 4 ms |
3     | 132 ms | ...


