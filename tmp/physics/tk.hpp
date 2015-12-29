#pragma once

#include <tokamak.h>
#include <moon9/math/rand.hpp>

namespace tk
{
    enum tkShape { tkSphere, tkBox, tkCapsule, tkRandom, tkPlane, tkUndefined }; //ragdoll, car

    class object
    {
        public:

        neAnimatedBody *abody;  // static body
        neRigidBody *body;      // rigid body

        tkShape shape;

        object() : abody(0), body(0), shape(tkUndefined)
        {}

        glm::mat4 transform() const
        {
            neT3 frame = ( body ? body->GetTransform() : abody->GetTransform() );

            return glm::make_mat4( (GLfloat*)&frame );
        }

        void render() const
        {
            moon9::disable::lighting no_lights;
            moon9::style::color color( body->IsIdle() ? 0 : 1, 0.25f, body->IsIdle() ? 1 : 0 );
            moon9::style::wireframe wf;

            moon9::matrix::push p;
            moon9::matrix::transform f( transform() );

            if( shape == tkSphere )
                moon9::geometry::sphere sph( 1 );
            if( shape == tkBox )
                moon9::geometry::cube cb;
            if( shape == tkCapsule )
                moon9::geometry::capsule cp( 1, 1 );
        }

        void push( const moon9::vec3 &force, const moon9::vec3 &relative_position = moon9::vec3(0,0,0) )
        {
            if( body )
                body->ApplyImpulse( force, relative_position + body->GetPos() );
        }

        void lock( bool x, bool y, bool z );

        moon9::vec3 position() const // for debugging purposes!
        {
            glm::mat4 mat4 = transform();
            return moon9::vec3( mat4[3].x, mat4[3].y, mat4[3].z );
        }
    };

    class world
    {
        static const int N_RIGIDBODY = 20;
        static const int N_ANIMATED_BODY = 1;
        static const int N_GEOMETRY = (N_RIGIDBODY + N_ANIMATED_BODY);

        //globals
        neSimulator *sim;
        neRigidBody *boxBody[N_RIGIDBODY];
        neAllocatorDefault all;

        private:

        world( const world & );
        world &operator=( const world & );

        public:

        world() : sim(0)
        {}

        void clear()
        {
            if( sim )
                neSimulator::DestroySimulator(sim);

            sim = 0;
        }

        void setup( const moon9::vec3 &gravity = moon9::vec3(0,0,-10.f) )
        {
            clear();

            // create simulator
            neSimulatorSizeInfo sizeInfo;

            sizeInfo.rigidBodiesCount = N_RIGIDBODY;
            sizeInfo.animatedBodiesCount = N_ANIMATED_BODY;
            sizeInfo.geometriesCount = N_GEOMETRY;
            sizeInfo.overlappedPairsCount = (N_RIGIDBODY * N_ANIMATED_BODY) + N_RIGIDBODY * (N_RIGIDBODY - 1) / 2;
            sizeInfo.rigidParticleCount = 0;
            sizeInfo.constraintsCount = 0;
            sizeInfo.terrainNodesStartCount = 0;

            neV3 g;
            g.Set( gravity.x, gravity.y, gravity.z );

            sim = neSimulator::CreateSimulator( sizeInfo, &all, &g );
        }

        template< tkShape S >
        object add( float kg_mass = 1.f, const moon9::vec3 &position = moon9::vec3(0,0,0) )
        {
            object o;

            static const float DOMINO_WIDTH = 2.0f;
            static const float DOMINO_HEIGHT = 4.f;
            static const float DOMINO_DEPTH = 0.5f;

            tkShape shape = S;

            while( shape == tkRandom )
                shape = tkShape( moon9::rand( 0, tkRandom - 1 ) );

            if( shape == tkPlane )
            {
                neAnimatedBody * body = sim->CreateAnimatedBody();
                neGeometry * geom = body->AddGeometry();

                geom->SetBoxSize( 100, 100, 1.0f );

                body->UpdateBoundingInfo();
                //body->SetMass( 0.f );
                body->SetPos( position );

                o.shape = shape;
                o.abody = body;
            }
            else
            {
                neRigidBody *body = sim->CreateRigidBody();
                neGeometry *geom = body->AddGeometry();

                if( shape == tkSphere )
                {
                    geom->SetSphereDiameter(DOMINO_HEIGHT);
                    body->SetInertiaTensor(neSphereInertiaTensor(DOMINO_HEIGHT, kg_mass));
                }
                else
                if( shape == tkBox )
                {
                    geom->SetBoxSize(DOMINO_DEPTH, DOMINO_WIDTH, DOMINO_HEIGHT);
                    body->SetInertiaTensor(neBoxInertiaTensor(DOMINO_DEPTH, DOMINO_WIDTH, DOMINO_HEIGHT, kg_mass));
                }
                else
                if( shape == tkCapsule )
                {
                    geom->SetCylinder(DOMINO_WIDTH,DOMINO_HEIGHT);
                    body->SetInertiaTensor(neCylinderInertiaTensor(DOMINO_WIDTH, DOMINO_HEIGHT, kg_mass));
                }

                body->UpdateBoundingInfo();
                body->SetMass( kg_mass );
                body->SetPos( position );

                o.shape = shape;
                o.body = body;
            }

            return o;
        }

        void *del( object &o )
        {
            if( o.body )
                sim->FreeRigidBody( o.body );

            if( o.abody )
                sim->FreeAnimatedBody( o.abody );

            o.body = 0;
            o.abody = 0;
        }

        void run( float dt = 1/60.f )
        {
            if( sim )
                sim->Advance(dt);
        }
    };
}
