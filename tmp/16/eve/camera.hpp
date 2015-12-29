/*
 * Simple camera class
 * Based on quaternion camera class by Vic Hollis
 * - rlyeh
 *
 *
 * Features:
 * - Safe reticles
 * - Frustum clipping

 *
 * Real-Time Cameras: A guide for game designers and developers
 * 2009, Mark Haigh-Hutchinson
 * p22:
 * - lens type: anamorphic, zoom, fisheye
 * - film stock: 35mm, 70mm, film grain, light sensitivity
 * - capture rate: 24fps, 30 fps, 48fps, 96fps...
 * - exposure (shutter speed): overexposed, underexposed
 * - aspect ratio
 * p30
 * - first person camera / third person camera
 * - cinematic / dynamic
 * - look at / position / orientation / frustum / viewport / field of view / aspect ratio
 * p34
 * - safe frame : 10/15%
 * - common display aspect ratios : 1.33:1 (tv 4:3), 1.78:1 (hdtv 16:9), 2.35:1 (anamorphic), 1.85:1 (cinemascope)
 * - motion blur
 * p39
 * - transitions
 *   behavioral
 *   interpolations: from state a to state b in t seconds using tw tween
 *   jump cuts ( interpolations where t = 0 )
 *   wipes and fades ( special fx to do transitions )
 * p40
 * - constraints
 *   motion : distance, character-relative position, path, splines, surface, acceleration/velocity limits
 *   orientation : limit pitch (~disorientation), soft limits / hard limits, acceleration restrictions (lowpass) ...
 * p41
 * - presentation
 *   orthographic 2d
 *   perspective 3d
 *   hybrid 2.5d
 * p64 predicition, ai, pathfinding
 * voy por p80, cinematography
 *
 * todo: http://outerra.blogspot.com.es/2012/11/maximizing-depth-buffer-range-and.html
 */

#pragma once

#include <cmath>

//#include <eve/gl.hpp>
#include <geo/vec.hpp>
#include <geo/frustum.hpp>

#if 0

// fps bar (25 hz)
// take
// frame, scene

#pragma once

// fov
// dov
// orbit
// advance
// link_target
// link_position
// link_up
// tween
// project
// unproject

// bokeh
// speed
// acceleration
// shaking
// set_working_area
// set_wall_mask
//
// ...

#endif


// geo::vec2 projection( geo::vec3 )
// rect3 inverse_projection( geo::vec2 )
// set_pitch, set_roll
// rotate_yaw

//#include <eve/render.hpp>

// camera coordinate system follows opengl basis internally
//
//  .z+
//  |  y+
//  | /
//  |/
//  *---- x+
//
// beware when retrieving cam_pos or cam_basis!
//
//
//
//

namespace eve
{
    class glass;
}


/* Using GLM for our transformation matrices */
#include <glm/glm.hpp> // 4minicam
#include <glm/gtc/matrix_transform.hpp> // 4minicam
#include <glm/gtc/type_ptr.hpp>

#include <geo/mat4.hpp>

namespace eve
{
    struct minicam
    {
        // input values
        geo::vec3 position;
        geo::vec3 offset;

        geo::vec3 angle;        // radians (yaw,roll,pitch)
        geo::vec3 up;
        float yaw()   const { return angle.x; }
        float roll()  const { return angle.y; }
        float pitch() const { return angle.z; }

        geo::vec3 noise;        // disturbs direction

        glm::vec4 viewport;     // viewport (x,y,w,h) ; (x:0, y:0) is bottom-left

        float distance;
        float fov;              // degrees

        // output values, after update()
        geo::mat4 projection, view, view90, mvp, mvp90;
        geo::vec3 forward, right, direction;
        geo::frustum frustum;

        // final output tuples are:
        // final_pos is { (position + offset) }
        // final_dir is { (direction + noise) }
        // final relative axis are { right, forward, up }

        // custom values (for your own purposes)
        bool   does_collision;
        size_t objs_count, objs_rendered;
        size_t tris_count, tris_rendered;
        std::string name;

        minicam( float w = 640, float h = 480 )
        {
            position = geo::vec3(0, 0, 10);
            offset = geo::vec3(0, 0, 0);
            noise = geo::vec3(0, 0, 0);
            angle = geo::vec3(0, 0, 0);
            up = geo::vec3(0, 0, 1);

            resize( w, h );

            distance = 1023.f;
            fov = 75.f * 0.5f; //37.5f;

            does_collision = false;
            objs_count = objs_rendered = 0;
            tris_count = tris_rendered = 0;
            name = "undefined";

            update();
        }

        void update()
        {
            // Refresh forward, right and direction vectors
            float sinx = sinf( angle.x ), sinz = sinf( angle.z ), cosx = cosf( angle.x ), cosz = cosf( angle.z );

            forward = geo::vec3( sinx, cosx, 0 );
            right = geo::vec3( cosx, -sinx, 0 );
            direction = geo::vec3( sinx * cosz, cosx * cosz, sinz );

            // Refresh matrices
            view = glm::lookAt( ( position + offset ), ( position + offset ) + ( direction + noise ), -up );

                // classic distance depth
                projection = glm::perspective( fov*2, viewport.z / viewport.w, 1.f, 1.f + distance );

                // reverse distance depth!
                // projection = glm::perspective( fov, viewport.z / viewport.w, 1.f, 0.f );

                // logarithmic zbuffer. helps getting rid of near/far distance problems.
                // [ref] http://outerra.blogspot.com.es/2009/08/logarithmic-z-buffer.html
                // projection = glm::perspective( fov, viewport.z / viewport.w, 0.0001f, 100000000.f ); // znear = 0.0001 inch, zfar = 39370.0 * 100000.0 inches = 100K kilometers
                // requires following code at the very end of your fragment shader
                //        const float C = 1.0;
                //        const float far = 1000000000.0;
                //        const float offset = 1.0;
                //        gl_FragDepth = (log(C * gl_TexCoord[6].z + offset) / log(C * far + offset));

            mvp = projection * view; //* (model = mat4(1.0));

            // Alternative matrix (right,forward,up) instead of (right,up,forward)
            view90 = glm::rotate( view, -90.f, geo::vec3(-1.0f, 0.0f, 0.0f) );
            mvp90 = projection * view90;

#if( 0 )
            {
                if( 1 ) // near ~0, far 1
                {
                    //glClearDepthdNV( distance );
                    glClearDepth( distance );
                    //glDepthRangedNV( 0.0, distance );
                    glDepthBoundsEXT( 0.0, distance );
                }
                else
                {
                    // inverted: near 1, far 0

                    //glClearDepthdNV( 0.0 );
                    glClearDepth( 0.0 );
                    //glDepthRangedNV( distance, 0.0);
                    glDepthBoundsEXT( distance, 0.0 );
                }
            }
#endif

            // Update frustum
            frustum.setup( fov, viewport.z / viewport.w, 1.f, 1.f + distance );
            frustum.update( position + offset, ( position + offset ) + ( direction + noise ), up );
        }

        void resize( float w, float h )
        {
            viewport = glm::vec4(0,0,w,h);

            update();
        }

        void lookdir( const geo::vec3 &dir /*, geo::vec3 focus_dof */ )
        {
            geo::vec3 _dir = glm::normalize( dir.as<glm::vec3>() );

            angle.x = atan2( _dir.x, _dir.y );
            angle.z = atan2( _dir.z, sqrt( _dir.x * _dir.x + _dir.y * _dir.y ) );

            update(); // recompute direction vector (and matrices)
        }

        void lookat( const geo::vec3 &target /*, geo::vec3 focus_dof */ )
        {
            lookdir( target - ( position + offset ) );
        }

        void rotate( float yaw, float roll, float pitch )
        {
            angle.x -= -yaw;
            angle.y -= roll;
            angle.z -= pitch;

            const float pi = 3.1415926535897932384626433832795f;

            if( angle.x < -pi )           angle.x += pi * 2;
            if( angle.x >  pi )           angle.x -= pi * 2;
            //if( angle.y < -pi )           angle.y += pi * 2;
            //if( angle.y >  pi )           angle.y -= pi * 2;
            if( angle.z < -pi * 0.49f )   angle.z = -pi * 0.49f;
            if( angle.z >  pi * 0.49f )   angle.z =  pi * 0.49f;

            update();
        }

        //@todo
        //void lock_pos();
        //void lock_dir( bool forward, bool right, bool up ); dir * vec3().110()

        void apply() const;
        void apply90() const;

        geo::vec3 unproject( float screen_x, float screen_y ) const; //at_screen()

        geo::vec3 unproject2( float screen_x, float screen_y, float w, float h ) const; //at_screen()

        // typical orbital controllers

        void orbit( float incx, float incy )
        {
            float distance = len( position );
            geo::vec3 at = geo::vec3( position ) + direction * (distance * 0.5f);
            fru( 0, -incx * distance, -incy * distance );
            lookat( at );
            update();
            geo::vec3 dir = norm( position - at );
            position = dir * distance;
            update();
        }

        void radius( float incr )
        {
            float distance = len( position );
            position += direction * ( incr * distance );
            update();
        }

        // typical fps controllers

        void wheel( float inc )
        {
            fov += -inc;
        }

        void rfu( float inc_right, float inc_fwd, float inc_up )
        {
            position += right * inc_right;
            position += forward * inc_fwd;
            position += up * inc_up;
        }

        void ruf( float inc_right, float inc_up, float inc_fwd )
        {
            rfu( inc_right, inc_fwd, inc_up );
        }

        void fru( float inc_fwd, float inc_right, float inc_up )
        {
            rfu( inc_right, inc_fwd, inc_up );
        }

        void fur( float inc_fwd, float inc_up, float inc_right )
        {
            rfu( inc_right, inc_fwd, inc_up );
        }

        void urf( float inc_up, float inc_right, float inc_fwd )
        {
            rfu( inc_right, inc_fwd, inc_up );
        }

        void ufr( float inc_up, float inc_fwd, float inc_right )
        {
            rfu( inc_right, inc_fwd, inc_up );
        }

        void wsad( float w, float s, float a, float d )
        {
            rfu( d-a, w-s, 0.f );
        }

        void wasd( float w, float a, float s, float d )
        {
            rfu( d-a, w-s, 0.f );
        }

        void wsadec( float w, float s, float a, float d, float e, float c )
        {
            rfu( d-a, w-s, e-c );
        }

        void wasdec( float w, float a, float s, float d, float e, float c )
        {
            rfu( d-a, w-s, e-c );
        }
    };

    typedef minicam camera;

}

// compiler will expect some nice library to render this, at linking stage :)
void render( const eve::camera &cam, const float &projection = 0.f ); // [0 = position .. 1 = lookat, 2 = lookat*2, ...]

