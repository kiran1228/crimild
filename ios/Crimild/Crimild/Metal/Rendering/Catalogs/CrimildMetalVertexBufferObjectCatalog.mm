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

#import "CrimildMetalVertexBufferObjectCatalog.h"

#import "CrimildMetalRenderer.h"

#if !TARGET_OS_SIMULATOR

using namespace crimild;
using namespace crimild::metal;

VertexBufferObjectCatalog::VertexBufferObjectCatalog( MetalRenderer *renderer )
    : _nextBufferId( 0 ),
      _renderer( renderer )
{
    
}

VertexBufferObjectCatalog::~VertexBufferObjectCatalog( void )
{
    
}

int VertexBufferObjectCatalog::getNextResourceId( void )
{
    int bufferId = _nextBufferId++;
    return bufferId;
}

void VertexBufferObjectCatalog::bind( ShaderProgram *program, VertexBufferObject *vbo )
{
    Catalog< VertexBufferObject >::bind( vbo );
    
    if ( vbo->getCatalogId() < 0 ) {
        return;
    }
    
    [getRenderer()->getRenderEncoder() setVertexBuffer: _vbos[ vbo->getCatalogId() ]
                                                offset: 0
                                               atIndex: 0];
}

void VertexBufferObjectCatalog::unbind( ShaderProgram *program, VertexBufferObject *vbo )
{
    Catalog< VertexBufferObject >::unbind( vbo );
}

void VertexBufferObjectCatalog::load( VertexBufferObject *vbo )
{
    Catalog< VertexBufferObject >::load( vbo );
    
    if ( vbo->getData() != nullptr ) {
        id< MTLBuffer > vertexArray = [getRenderer()->getDevice() newBufferWithBytes: vbo->getData()
                                                                              length: vbo->getSizeInBytes()
                                                                             options: MTLResourceOptionCPUCacheModeDefault];
        _vbos[ vbo->getCatalogId() ] = vertexArray;
    }
}

void VertexBufferObjectCatalog::unload( VertexBufferObject *vbo )
{
    _vbos[ vbo->getCatalogId() ] = nullptr;
    
    Catalog< VertexBufferObject >::unload( vbo );
}

void VertexBufferObjectCatalog::cleanup( void )
{
    Catalog< VertexBufferObject >::cleanup();
}

#endif // TARGET_OS_SIMULATOR

