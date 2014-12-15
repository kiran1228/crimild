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

#ifndef CRIMILD_GL3_RENDERING_IMAGE_EFFECT_SSAO_
#define CRIMILD_GL3_RENDERING_IMAGE_EFFECT_SSAO_

#include <Crimild.hpp>

namespace crimild {
    
    namespace gl3 {
    
        class SSAOImageEffect : public ImageEffect {
        public:
            SSAOImageEffect( void );
            virtual ~SSAOImageEffect( void );
            
            /**
                Expected inputs:
                    - Full scene
                    - G-Buffer color
                    - G-Buffer positions
                    - G-Buffer normals
             */
            virtual void apply( crimild::RendererPtr const &renderer, int inputCount, Texture **inputs, PrimitivePtr const &primitive, FrameBufferObjectPtr const &output ) override;
            
        private:
            void buildSSAOBuffer( int width, int height );
            void computeSSAO( crimild::Renderer *renderer, Texture *srcPositions, Texture *srcNormals, Primitive *primitive );
            void applySSAO( crimild::Renderer *renderer, Texture *srcImage, Texture *ssaoMap, Primitive *primitive, FrameBufferObject *output );
            
            FrameBufferObjectPtr _ssaoBuffer;
            TexturePtr _ssaoBufferOutput;
        };
        
    }
    
}

#endif

