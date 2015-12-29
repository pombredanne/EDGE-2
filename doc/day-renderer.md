## Day 15

### Basic Renderer

GL4

vec2, vec3, vec4,
mat4, quat

2d: safe zone < overscan zone, show reticle onoff

                   safe 1, overscan > 1
                    +---------------+
                    | +-----^-----+ |
safe -1,            | |     |     | |
overscan<-1         | <-----+-----> | safe 1, overscan > 1
                    | |     |     | |
                    | +-----v-----+ |
                    +---------------+
                        safe -1, overscan < -1

3d
+y/+z
|/
+--+x

2.5d {
3d or 2d layers, in
2d z-depth: -2, -1, 0, 1, 2 ...
}

3d chars in 2d env
2d chars in 3d env
parallax also

api per layer {
    fbo, passfx,
    screenshot,
    texture load
    index buffer
    vertex buffer
    uv buffer ?
}
