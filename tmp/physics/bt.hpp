#pragma once

#include <set>

#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

#include <eve/render.hpp>
#include <geo/vec.hpp>

#include "randworld.hpp"

namespace bt
{
    enum btShape { btSphere = 0, btBox, btCapsule, btRandom, btPlane, btUndefined }; //ragdoll, car

    class object
    {
        public:

        btRigidBody *body;
        btShape shape;

        object() : body(0), shape(btUndefined)
        {}

        glm::mat4 transform() const
        {
            // get the transformation of the body into an OpenGL matrix
            btTransform trans;
            body->getMotionState()->getWorldTransform(trans);

            btScalar m[16];
            trans.getOpenGLMatrix(m);

            return glm::make_mat4( m );
        }

        void render() const
        {
            eve::disable::lighting no_lights;
            eve::style::color color( body->isActive() ? 1 : 0, 0, body->isActive() ? 0 : 1 );
            eve::style::wireframe wf;

            eve::matrix::push p;
            eve::matrix::transform mult( transform() );

            if( shape == btSphere )
                eve::geometry::sphere sph( 1 );
            if( shape == btBox )
                eve::geometry::cube cb;
            if( shape == btCapsule )
                eve::geometry::capsule cp( 1, 1 );
        }

        void push( const eve::vec3 &force )
        {
            body->applyCentralImpulse( force );
        }

        void lock( bool x, bool y, bool z )
        {
            body->setLinearFactor( eve::vec3(x,y,z) );
        }

        eve::vec3 position() const // for debugging purposes!
        {
            glm::mat4 mat4 = transform();
            return eve::vec3( mat4[3].x, mat4[3].y, mat4[3].z );
        }
    };

    class world
    {
        private:

        world( const world & );
        world &operator=( const world & );

        public:

        world() :
            broadphase(0),
            collisionConfiguration(0),
            dispatcher(0),
            solver(0),
            dynamicsWorld(0)
        {}

        ~world()
        {
            clear();
        }

        void clear()
        {
            while( bodies.size() )
                destroy( (*bodies.begin()) );

            if(          dynamicsWorld ) delete dynamicsWorld, dynamicsWorld = 0;
            if(                 solver ) delete solver, solver = 0;
            if( collisionConfiguration ) delete collisionConfiguration, collisionConfiguration = 0;
            if(             dispatcher ) delete dispatcher, dispatcher = 0;
            if(             broadphase ) delete broadphase, broadphase = 0;
        }

        void setup( const eve::vec3 &gravity )
        {
            clear();

            broadphase = new btDbvtBroadphase();

            collisionConfiguration = new btDefaultCollisionConfiguration();
            dispatcher = new btCollisionDispatcher(collisionConfiguration);

            solver = new btSequentialImpulseConstraintSolver();

            dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
            dynamicsWorld->setGravity( gravity );
        }

        template<btShape S>
        object add( float kg_mass = 0.f, const eve::vec3 &position = eve::vec3(0,0,0) )
        {
            object o;
            btScalar mass = kg_mass;

            o.shape = S;

            while( o.shape == btRandom )
                o.shape = btShape( eve::rand( 0, btRandom - 1 ) );

            btCollisionShape *shape = 0;

            switch( o.shape )
            {
                case btSphere:
                    shape = new btSphereShape(1);
                    break;
                case btPlane:
                    shape = new btStaticPlaneShape(btVector3(0,1,0),1);
                    break;
                case btBox:
                    shape = new btBoxShape(btVector3(0.5f,0.5f,0.5f));
                    break;
                case btCapsule:
                    shape = new btCapsuleShape(1,1);
                    break;
                default: case btUndefined:
                    return object();
            }

            btVector3 inertia(0,0,0);

            if( mass > 0 )
                shape->calculateLocalInertia( mass, inertia );

            btMotionState *motion = new btDefaultMotionState( btTransform( btQuaternion(0,0,0,1), position ) );

            btRigidBody::btRigidBodyConstructionInfo setup( mass, motion, shape, inertia );
            setup.m_linearDamping = 0.2f;  // Think of this as resistance to translational motion.
            setup.m_angularDamping = 0.1f;  // Think of this as resistance to rotation.
            setup.m_friction = 1.5f;
            setup.m_restitution = 0.f; //1.3f; // 0 = doesnt bounce, more stable; the higher the bouncier

            btRigidBody *body = new btRigidBody( setup );

            //o.body->setAngularVelocity( btVector3(1,1,1) ); // spinning dir/speed
            // o.body->setRestitution( 0 ); // bouncing
            //o.body->activate( true );
            // applyCentralImpulse( vec3 )
            // applyTorqueImpulse

            dynamicsWorld->addRigidBody( body );

            /*
                #define BIT(x) (1<<(x))
                enum collisiontypes {
                COL_NOTHING = 0, //<Collide with nothing
                COL_SHIP = BIT(0), //<Collide with ships
                COL_WALL = BIT(1), //<Collide with walls
                COL_POWERUP = BIT(2) //<Collide with powerups
                }

                int shipCollidesWith = COL_WALL;
                int wallCollidesWith = COL_NOTHING;
                int powerupCollidesWith = COL_SHIP | COL_WALL;
                After setting these up, simply add your body objects to the world as normal, except as the second and third parameters pass your collision type for that body, and the collision mask.
                If you are not using a recent enough version of Bullet, you may find this overload of addRigidBody does not exist. In this case, use btDynamicsWorld::addCollisionObject instead of the more usual btDynamicsWorld::addRigidBody. You may have to set the gravity on the rigid body manually in this case.
                btRigidBody ship; // Set up the other ship stuff
                btRigidBody wall; // Set up the other wall stuff
                btRigidBody powerup; // Set up the other powerup stuff

                mWorld->addRigidBody(ship, COL_SHIP, shipCollidesWith);
                mWorld->addRigidBody(wall, COL_WALL, wallCollidesWith);
                mWorld->addRigidBody(powerup, COL_POWERUP, powerupCollidesWith);
            */

            bodies.insert( body );

			o.body = body;

            return o;
        }

        void del( object &o )
        {
            destroy( o.body );
            o.body = 0;
        }

        void run( float dt = 1/60.f )
        {
            if( dynamicsWorld )
                dynamicsWorld->stepSimulation(dt,10);
        }

        void render() const
        {
            if( dynamicsWorld )
                dynamicsWorld->debugDrawWorld();
        }

        eve::vec3 hit, hit_normal;
        bool ray( const eve::vec3 &from, const eve::vec3 &to )
        {
            btCollisionWorld::ClosestRayResultCallback RayCallback(from, to);

            dynamicsWorld->rayTest(from, to, RayCallback);

            if(!RayCallback.hasHit())
                return false;

            hit = RayCallback.m_hitPointWorld;
            hit_normal = RayCallback.m_hitNormalWorld;

            return true;
        }

        protected:

        btBroadphaseInterface *broadphase;
        btDefaultCollisionConfiguration *collisionConfiguration;
        btCollisionDispatcher *dispatcher;
        btSequentialImpulseConstraintSolver *solver;
        btDiscreteDynamicsWorld *dynamicsWorld;

        std::set< btRigidBody * > bodies;

        void destroy( btRigidBody *body )
        {
            if( !body )
                return;

            if( bodies.find( body ) == bodies.end() )
                return;

            bodies.erase( body );

            dynamicsWorld->removeRigidBody( body );

            btMotionState *motion = body->getMotionState();
            delete motion;

            btCollisionShape *shape = body->getCollisionShape();
            delete shape;

            delete body;
        }
    };

}





// see also:
// http://bulletphysics.org/mediawiki-1.5.8/index.php/Bullet_binary_serialization

/*
    interesting methods:

    getMotionState() ; Get the motion state object, which is useful for rendering, network, etc.
    setLinearVelocity(btVector3) ; Makes the rigid body move
    setAngularVelocity(btVector3) ; Makes the rigid body spin
    setRestitution(btScalar) ; The spring or bounciness of a rigid body
    setLinearFactor(btVector3) ; Locks linear movement to specific axis
    setAngularFactor(btVector3) ; Locks angular movement to specific axis
    setDamping(btScalar, btScalar) ; ?
*/

/*
    I want to constrain an object to two dimensional movement, skipping one of the cardinal axes
    body->setLinearFactor(btVector3(1,0,1)); //bullet-xz
    body->setAngularFactor(btVector3(0,1,0)); //bullet-y
*/
/*
    I want to cap the speed of my spaceship
    void myTickCallback(const btDynamicsWorld *world, btScalar timeStep) {
        // mShipBody is the spaceship's btRigidBody
        btVector3 velocity = mShipBody->getLinearVelocity();
        btScalar speed = velocity.length();
        if(speed > mMaxSpeed) {
            velocity *= mMaxSpeed/speed;
            mShipBody->setLinearVelocity(velocity);
        }
    }
*/
/*
    shapes:

    Primitives
    Primitives have specialist collision algorithms that are very fast. One should always attempt to use a primitive if it is the right shape. Another advantage of using a primitive is that in the case of rounded shapes like spheres and cylinders, objects roll properly without the bumpy behaviour that a polyhedral approximation would have.
    *   btSphereShape ; A very fast and simple shape.
    *   btBoxShape ; A cuboid shape, the length in each dimension can be chosen arbitrarily.
        btCylinderShape ; A cylinder much like the box shape.
    *   btCapsuleShape ; An elongated sphere, useful for modelling humanoid characters as it abstracts from details like limbs and gait, but can still skip up stairs so long as the steps are not too high.
        btConeShape ; A cone pointing along the given axis with the point at (0,0,0)
        btMultiSphereShape ; A convex shape defined by spheres. Useful for representing bevelled boxes or other shapes without sharp corners. Also since the spheres can be independently scaled in each of the 3 dimensions, a btMultiSphereShape containing just one sphere can be useful to create ellipsoids (squashed spheres), which is not possible with btSphereShape.

    Meshes
        btConvexHull ; This is the fastest kind of arbitrary shape. It is defined by a cloud of vertexes but the shape formed is the smallest convex shape that encloses the vertexes. For making a convex dynamic shape like a TV, this is ideal. Make sure to reduce the number of vertices below say 100. Reducing the number of vertices can be done automatically using the btShapeHull vertex reduction utility.
        btConvexTriangleMeshShape ; This is similar to the above but is specified using a conventional mesh composed of vertexes and indices. A btConvexHull is more efficient.
        btBvhTriangleMeshShape ; A static mesh that is comprised of arbitrary triangles. These can have many triangles and still be fast because of the internal hierarchal implementation. Don't be afraid to batch static scenery geometry together to reduce the work in the Broadphase. Building the initial tree can take some time. If this becomes a problem the tree can be serialised to disk in advance and loaded quickly with btOptimizedBvh::deSerializeInPlace, see Demos/ConcaveDemo/ConcavePhysicsDemo.cpp
        btHeightfieldTerrainShape ; A static mesh that is optimised for and described by a height map.
    *   btStaticPlaneShape ; A static plane that is solid to infinity on one side. Several of these can be used to confine a convex space in a manner that completely prevents tunneling to the outside. The plane itself is specified with a normal and distance as is standard in mathematics.

    Compound
        btCompoundShape ; If a concave dynamic shape is required, then it may be specified by combining convex shapes (usually primitives and btConvexHull) in various configurations.
        E.g. using just boxes, one can create a chair, and using spheres and cylinders, one can approximate a bowling pin with reasonable precision. Do not create rigid bodies inside each other if their shapes are compound, since a shape can be jammed between the two components of another shape. Rather than springing apart, the shapes will therefore be jammed inside each other, and the various components will generate lots of collision pairs, slowing everything down.
        For those that want to specify arbitrary triangle geometry for dynamic collision shapes, The ConvexDecomposition demo shows how to procedurally break a concave shape up into a composition of convex shapes using some utility code in the Extras folder.

    Misc
        btEmptyShape ; A collision shape without actual collision detection shape, so most users should ignore this class.
*/

/*
    Bullet 2.72 re-enables CCD based motion clamping. It lets the developer enable clamping of motion if the motion per simulation step exceeds a given threshold. This feature works on a per-object basis.
    CCD motion clamping activates for convex objects that exceed a (squared to avoid taking square roots) velocity threshold. By default this threshold is zero, which means this feature is disabled for rigid bodies.
    For a cube of size 1 try:
        //enable CCD if the object moves more than 1 meter in one simulation frame
        body->setCcdMotionThreshold(1);
    CCD works on an embedded sphere of radius, make sure this radius is embedded inside the convex objects, preferably smaller: for an object of dimentions 1 meter, try
        body->setCcdSweptSphereRadius(0.2f);
    There is no CCD motion clamping between pairs of objects that are in penetration or that share contact points. This is done to avoid objects getting stuck forever.
*/


/*
Ok, near as I can tell, motion threshold is about efficiency. If the object didn't move much, then don't treat it as a continuously moving object, just test against it's current position. For example, if an airplane moves 1cm, you can fairly safely perform physics calculations of a person hitting it without doing exact calculations of movement over the time period for that 1cm. You can do the calculations based just upon the plane's new position and rotation.
 box_a_controller->GetRigidBody()->m_ccdSquareMotionTreshold = 1.0f;  // The object is static anyway in this case.
*/
