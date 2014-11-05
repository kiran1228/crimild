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

#include "Rendering/RenderPass.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/VisibilitySet.hpp"
#include "Rendering/FrameBufferObject.hpp"
#include "Rendering/RenderQueue.hpp"
#include "SceneGraph/Geometry.hpp"
#include "Components/RenderStateComponent.hpp"
#include "Components/SkinComponent.hpp"
#include "Components/JointComponent.hpp"
#include "Primitives/QuadPrimitive.hpp"

using namespace crimild;

RenderPass::RenderPass( void )
	: _screen( new QuadPrimitive( 2.0f, 2.0f, VertexFormat::VF_P3_UV2, Vector2f( 0.0f, 1.0f ), Vector2f( 1.0f, -1.0f ) ) )
{

}

RenderPass::~RenderPass( void )
{

}

void RenderPass::render( Renderer *renderer, RenderQueue *renderQueue, Camera *camera )
{
    renderQueue->getOpaqueObjects().each( [&]( Geometry *geometry, int ) {
        render( renderer, geometry, camera );
    });

    renderQueue->getTranslucentObjects().each( [&]( Geometry *geometry, int ) {
        render( renderer, geometry, camera );
    });

    renderScreenObjects( renderer, renderQueue, camera );
}

void RenderPass::render( Renderer *renderer, VisibilitySet *vs, Camera *camera )
{
	vs->foreachGeometry( [&]( Geometry *geometry ) mutable {
		render( renderer, geometry, camera );
	});
}

void RenderPass::render( Renderer *renderer, Geometry *geometry, Camera *camera )
{
	RenderStateComponent *renderState = geometry->getComponent< RenderStateComponent >();
	if ( renderState->hasMaterials() ) {
		geometry->foreachPrimitive( [&]( Primitive *primitive ) mutable {
			renderState->foreachMaterial( [&]( Material *material ) mutable {
				render( renderer, geometry, primitive, material, camera );
			});
		});
	}
}

void RenderPass::render( Renderer *renderer, Geometry *geometry, Primitive *primitive, Material *material, Camera *camera )
{
	if ( !material || !primitive ) {
		return;
	}

	ShaderProgram *program = material->getProgram() ? material->getProgram() : renderer->getFallbackProgram( material, geometry, primitive );
	if ( !program ) {
		return;
	}

	RenderStateComponent *renderState = geometry->getComponent< RenderStateComponent >();

	// bind shader program first
	renderer->bindProgram( program );

	// bind material properties
	renderer->bindMaterial( program, material );

	// bind lights
	if ( renderState->hasLights() ) {
		renderState->foreachLight( [&]( Light *light ) {
			renderer->bindLight( program, light );
		});
	}

	// bind joints and other skinning information
	SkinComponent *skinning = geometry->getComponent< SkinComponent >();
	if ( skinning != nullptr && skinning->hasJoints() ) {
		skinning->foreachJoint( [&]( Node *node, unsigned int index ) {
			JointComponent *joint = node->getComponent< JointComponent >();
			renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::JOINT_WORLD_MATRIX_UNIFORM + index ), joint->getWorldMatrix() );
			renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::JOINT_INVERSE_BIND_MATRIX_UNIFORM + index ), joint->getInverseBindMatrix() );
		});
	}

	// bind vertex and index buffers
	renderer->bindVertexBuffer( program, primitive->getVertexBuffer() );
	renderer->bindIndexBuffer( program, primitive->getIndexBuffer() );

	// apply transformations
	renderer->applyTransformations( program, geometry, camera );

	// draw primitive
	renderer->drawPrimitive( program, primitive );

	// restore transformation stack
	renderer->restoreTransformations( program, geometry, camera );

	// unbind primitive buffers
	renderer->unbindVertexBuffer( program, primitive->getVertexBuffer() );
	renderer->unbindIndexBuffer( program, primitive->getIndexBuffer() );

	// unbind lights
	if ( renderState->hasLights() ) {
		renderState->foreachLight( [&]( Light *light ) {
			renderer->unbindLight( program, light );
		});
	}

	// unbind material properties
	renderer->unbindMaterial( program, material );

	// lastly, unbind the shader program
	renderer->unbindProgram( program );
}

void RenderPass::render( Renderer *renderer, Texture *texture, ShaderProgram *program )
{
    if ( program == nullptr ) {
        program = renderer->getFallbackProgram( nullptr, nullptr, nullptr );
        if ( program == nullptr ) {
            return;
        }
    }
     
    // bind shader program first
    renderer->bindProgram( program );
    
    // bind framebuffer texture
    renderer->bindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::MATERIAL_COLOR_MAP_UNIFORM ), texture );
     
    // bind vertex and index buffers
    renderer->bindVertexBuffer( program, _screen->getVertexBuffer() );
    renderer->bindIndexBuffer( program, _screen->getIndexBuffer() );

    Matrix4f mMatrix;
    mMatrix.makeIdentity();
    renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::MODEL_MATRIX_UNIFORM ), mMatrix );
     
    // draw primitive
    renderer->drawPrimitive( program, _screen.get() );
     
    // unbind primitive buffers
    renderer->unbindVertexBuffer( program, _screen->getVertexBuffer() );
    renderer->unbindIndexBuffer( program, _screen->getIndexBuffer() );
     
    // unbind framebuffer texture
    renderer->unbindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::MATERIAL_COLOR_MAP_UNIFORM ), texture );
     
    // lastly, unbind the shader program
    renderer->unbindProgram( program );
}

void RenderPass::render( Renderer *renderer, FrameBufferObject *fbo, ShaderProgram *program )
{

}

void RenderPass::renderScreenObjects( Renderer *renderer, RenderQueue *renderQueue, Camera *camera )
{
	const Matrix4f &projection = camera->getOrthographicMatrix();
    Matrix4f view;
    view.makeIdentity();
    
    renderQueue->getScreenObjects().each( [&]( Geometry *geometry, int ) {
	    Matrix4f model = geometry->getWorld().computeModelMatrix();
	    Matrix4f normal = model;
		normal[ 12 ] = 0.0f;
		normal[ 13 ] = 0.0f;
		normal[ 14 ] = 0.0f;

		RenderStateComponent *renderState = geometry->getComponent< RenderStateComponent >();
		if ( renderState->hasMaterials() ) {
			geometry->foreachPrimitive( [&]( Primitive *primitive ) mutable {
				renderState->foreachMaterial( [&]( Material *material ) mutable {
					if ( !material || !primitive ) {
						return;
					}

					ShaderProgram *program = material->getProgram() ? material->getProgram() : renderer->getFallbackProgram( material, geometry, primitive );
					if ( !program ) {
						return;
					}

					renderer->bindProgram( program );

					renderer->bindMaterial( program, material );

					renderer->bindVertexBuffer( program, primitive->getVertexBuffer() );
					renderer->bindIndexBuffer( program, primitive->getIndexBuffer() );

					renderer->applyTransformations( program, projection, view, model, normal );

					renderer->drawPrimitive( program, primitive );

					renderer->unbindVertexBuffer( program, primitive->getVertexBuffer() );
					renderer->unbindIndexBuffer( program, primitive->getIndexBuffer() );

					renderer->unbindMaterial( program, material );

					renderer->unbindProgram( program );
				});
			});
		}
    });
}

