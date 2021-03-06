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

#ifndef CRIMILD_OPENGL_VERTEX_BUFFER_OBJECT_CATALOG_
#define CRIMILD_OPENGL_VERTEX_BUFFER_OBJECT_CATALOG_

#include <Rendering/Catalog.hpp>

namespace crimild {
    
    class VertexBufferObject;    

	namespace opengl {

		class VertexBufferObjectCatalog : public Catalog< VertexBufferObject > {
		public:
			VertexBufferObjectCatalog( void );
			virtual ~VertexBufferObjectCatalog( void );

			virtual int getNextResourceId( void ) override;

			virtual void bind( ShaderProgram *program, VertexBufferObject *vbo ) override;
			virtual void unbind( ShaderProgram *program, VertexBufferObject *vbo ) override;

			virtual void load( VertexBufferObject *vbo ) override;
            virtual void unload( VertexBufferObject *vbo ) override;
            
            virtual void cleanup( void ) override;

		private:
			int composeId( unsigned int vaoId, unsigned int vboId );
			bool extractId( int compositeId, unsigned int &vaoId, unsigned int &vboId );
            
        private:
            std::list< int > _unusedVBOIds;
		};

	}

}

#endif

