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

#ifndef CRIMILD_SIMULATION_TASK_
#define CRIMILD_SIMULATION_TASK_

#include "Mathematics/Time.hpp"
#include "Foundation/SharedObject.hpp"

namespace crimild {

	class RunLoop;

	class Task : public SharedObject {
	protected:
		Task( int priority = 0 );

	public:
		virtual ~Task( void );

		int getPriority( void ) const { return _priority; }

	private:
		int _priority;

	public:
		virtual void start( void ) { }
		virtual void stop( void ) { }
		virtual void suspend( void ) { }
		virtual void resume( void ) { }
		virtual void update( void ) { }

	public:
        void setRunLoop( SharedPointer< RunLoop > const &runLoop ) { _runLoop = runLoop; }
        SharedPointer< RunLoop > getRunLoop( void ) { return _runLoop.lock(); }

	private:
        std::weak_ptr< RunLoop > _runLoop;
	};
    
    using TaskPtr = SharedPointer< Task >;

}

#endif

