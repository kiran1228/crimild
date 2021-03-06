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

#ifndef CRIMILD_PARTICLE_GENERATOR_TIME_
#define CRIMILD_PARTICLE_GENERATOR_TIME_

#include "../ParticleSystemComponent.hpp"

namespace crimild {

    class TimeParticleGenerator : public ParticleSystemComponent::ParticleGenerator {
		CRIMILD_IMPLEMENT_RTTI( crimild::TimeParticleGenerator )

    public:
        TimeParticleGenerator( void );
        virtual ~TimeParticleGenerator( void );

		void setMinTime( crimild::Real32 value ) { _minTime = value; }
		crimild::Real32 getMinTime( void ) const { return _minTime; }

		void setMaxTime( crimild::Real32 value ) { _maxTime = value; }
		crimild::Real32 getMaxTime( void ) const { return _maxTime; }

		virtual void configure( Node *node, ParticleData *particles ) override;
        virtual void generate( Node *node, crimild::Real64 dt, ParticleData *particles, ParticleId startId, ParticleId endId ) override;

    private:
		crimild::Real32 _minTime;
		crimild::Real32 _maxTime;

		ParticleAttribArray *_times = nullptr;
		ParticleAttribArray *_lifeTimes = nullptr;

		/** 
		 	\name Coding support
		*/
		//@{

	public:
		virtual void encode( coding::Encoder &encoder ) override;
		virtual void decode( coding::Decoder &decoder ) override;

		//@}
    };

}

#endif

