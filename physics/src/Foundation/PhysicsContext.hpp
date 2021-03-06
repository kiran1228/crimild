/*
 * Copyright (c) 2013, Hernan Saez
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CRIMILD_PHYSICS_FOUNDATION_PHYSICS_CONTEXT_
#define CRIMILD_PHYSICS_FOUNDATION_PHYSICS_CONTEXT_

#include "Foundation/BulletUtils.hpp"

namespace crimild {

	namespace physics {

		class PhysicsContext : public DynamicSingleton< PhysicsContext > {
		public:
			PhysicsContext( void );
			~PhysicsContext( void );

			btDiscreteDynamicsWorld *getWorld( void ) { return crimild::get_ptr( _world ); }

			void setGravity( const Vector3f &gravity );
			const Vector3f getGravity( void ) const { return _gravity; }

			void step( float dt );

			void cleanup( void );

		private:
			void init( void );

		private:
			SharedPointer< btBroadphaseInterface > _broadphase;
    		SharedPointer< btDefaultCollisionConfiguration > _collisionConfiguration;
    		SharedPointer< btCollisionDispatcher > _dispatcher;
    		SharedPointer< btSequentialImpulseConstraintSolver > _solver;
    		SharedPointer< btDiscreteDynamicsWorld > _world;

    		Vector3f _gravity;
		};

	}
	
}

#endif

