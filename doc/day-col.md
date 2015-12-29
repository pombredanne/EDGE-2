## Day XX

### XXX

ozcollide? + signals

lod
ai: far away? dumb or idle ai, non skinned anims...
gfx: far away? less polys, poor textures, impostors...
audio: far away? cheaper codecs, disable doppler, ...

spatial partioning (coarse)
grid, tree of grids, hierarchical

collisions (fine)
capsule, polysoup, navmesh, sphere, circle,
box, cube, triangle, tray, line, segment, plane,
scene, region,

culling
- front/back face
- occlusion :(
- frustum vs scene
- frustum lvl 0, lvl 1, lvl 2...

        .
       / \
      /   \
     /     \
    /\      \
   /  \      \
  /\   \      \
 /1 \ 2 \   3  \
/____\___\______\


scenegraph for grouping and retrieve summed values
+--- a
+-+- b, b1
| +- b2
+--- c

examples: composed {
volumes, positions, transforms, logical grouping, etc
}

scenegraph api
attach( my(center), his(centroid) ),
detach( my(hip01), his(foot01) ),
detach_all();



