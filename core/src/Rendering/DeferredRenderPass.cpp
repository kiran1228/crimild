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

#include "Rendering/DeferredRenderPass.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/VisibilitySet.hpp"
#include "Rendering/FrameBufferObject.hpp"
#include "Rendering/RenderQueue.hpp"
#include "Rendering/ImageEffect.hpp"

#include "SceneGraph/Geometry.hpp"

#include "Components/RenderStateComponent.hpp"
#include "Components/SkinComponent.hpp"
#include "Components/JointComponent.hpp"

#include "Primitives/QuadPrimitive.hpp"

#include "Foundation/Log.hpp"

using namespace crimild;

DeferredRenderPass::DeferredRenderPass( void )
{
    
}

DeferredRenderPass::~DeferredRenderPass( void )
{
    
}

void DeferredRenderPass::render( RendererPtr const &renderer, RenderQueuePtr const &renderQueue, CameraPtr const &camera )
{
    computeShadowMaps( renderer, renderQueue, camera );
    
    renderToGBuffer( renderer, renderQueue, camera );
    composeFrame( renderer, renderQueue, camera );
    
#if 0
    auto gBuffer = getGBuffer( renderer );
    
    renderer->setAlphaState( AlphaState::ENABLED_SRC_COLOR_ONLY );
    renderer->setDepthState( DepthState::DISABLED );
    
    renderer->setViewport( Rectf( 0.0f, 0.75f, 0.25f, 0.25f ) );
    RenderPass::render( renderer, gBuffer->getRenderTargets()->get( G_BUFFER_DEPTH_TARGET_NAME )->getTexture(), nullptr );
    
    if ( _shadowMaps.size() > 0 ) {
        auto firstShadow = _shadowMaps.begin()->second;
        renderer->setViewport( Rectf( 0.0f, 0.5f, 0.25f, 0.25f ) );
        RenderPass::render( renderer, firstShadow->getTexture(), nullptr );
    }
    
    renderer->setViewport( Rectf( 0.5f, 0.75f, 0.25f, 0.25f ) );
    RenderPass::render( renderer, gBuffer->getRenderTargets()->get( G_BUFFER_COLOR_TARGET_NAME )->getTexture(), nullptr );
    
    renderer->setViewport( Rectf( 0.75f, 0.75f, 0.25f, 0.25f ) );
    RenderPass::render( renderer, gBuffer->getRenderTargets()->get( G_BUFFER_POSITION_TARGET_NAME )->getTexture(), nullptr );
    
    renderer->setViewport( Rectf( 0.5f, 0.5f, 0.25f, 0.25f ) );
    RenderPass::render( renderer, gBuffer->getRenderTargets()->get( G_BUFFER_NORMAL_TARGET_NAME )->getTexture(), nullptr );
    
    renderer->setViewport( Rectf( 0.75f, 0.5f, 0.25f, 0.25f ) );
    RenderPass::render( renderer, gBuffer->getRenderTargets()->get( G_BUFFER_VIEW_SPACE_NORMAL_TARGET_NAME )->getTexture(), nullptr );
    
    auto sceneFBO = renderer->getFrameBuffer( "scene" );
    renderer->setViewport( Rectf( 0.0f, 0.0f, 0.5f, 0.5f ) );
    RenderPass::render( renderer, sceneFBO->getRenderTargets()->get( "color" )->getTexture(), nullptr );
    
    getImageEffects()->each( [&]( ImageEffectPtr const &effect, int ) {
        effect->compute( renderer, camera );
        renderer->setViewport( Rectf( 0.5f, 0.0f, 0.5f, 0.5f ) );
        effect->apply( renderer, camera );
    });
    
    renderer->setViewport( Rectf( 0.25f, 0.75f, 0.25f, 0.25f ) );
    renderTranslucentObjects( renderer, renderQueue, camera );

    renderer->setViewport( Rectf( 0.25f, 0.5f, 0.25f, 0.25f ) );
    renderScreenObjects( renderer, renderQueue, camera );

    // reset viewport
    renderer->setViewport( Rectf( 0.0f, 0.0f, 1.0f, 1.0f ) );

#else
    if ( getImageEffects()->isEmpty() ) {
        auto sceneFBO = renderer->getFrameBuffer( "scene" );
        auto colorTarget = sceneFBO->getRenderTargets()->get( "color" );
        RenderPass::render( renderer, colorTarget->getTexture(), nullptr );
    }
    else {
        getImageEffects()->each( [&]( ImageEffectPtr const &effect, int ) {
            effect->compute( renderer, camera );
            effect->apply( renderer, camera );
        });
    }
    
    renderTranslucentObjects( renderer, renderQueue, camera );
    renderScreenObjects( renderer, renderQueue, camera );
#endif
}

FrameBufferObjectPtr DeferredRenderPass::getGBuffer( const RendererPtr &renderer )
{
    auto gBuffer = renderer->getFrameBuffer( G_BUFFER_NAME );
    if ( gBuffer != nullptr ) {
        return gBuffer;
    }
    
    int width = renderer->getScreenBuffer()->getWidth();
    int height = renderer->getScreenBuffer()->getHeight();
    gBuffer = crimild::alloc< FrameBufferObject >( width, height );
    
    gBuffer->getRenderTargets()->add( G_BUFFER_DEPTH_TARGET_NAME, crimild::alloc< RenderTarget >( RenderTarget::Type::DEPTH_32, RenderTarget::Output::RENDER_AND_TEXTURE, width, height, true ) );
    gBuffer->getRenderTargets()->add( G_BUFFER_COLOR_TARGET_NAME, crimild::alloc< RenderTarget >( RenderTarget::Type::COLOR_RGBA, RenderTarget::Output::TEXTURE, width, height, true ) );
    gBuffer->getRenderTargets()->add( G_BUFFER_POSITION_TARGET_NAME, crimild::alloc< RenderTarget >( RenderTarget::Type::COLOR_RGBA, RenderTarget::Output::TEXTURE, width, height, true ) );
    gBuffer->getRenderTargets()->add( G_BUFFER_NORMAL_TARGET_NAME, crimild::alloc< RenderTarget >( RenderTarget::Type::COLOR_RGBA, RenderTarget::Output::TEXTURE, width, height, true ) );
    gBuffer->getRenderTargets()->add( G_BUFFER_VIEW_SPACE_NORMAL_TARGET_NAME, crimild::alloc< RenderTarget >( RenderTarget::Type::COLOR_RGBA, RenderTarget::Output::TEXTURE, width, height, true ) );
    
    renderer->addFrameBuffer( G_BUFFER_NAME, gBuffer );
    
    return  gBuffer;
}

void DeferredRenderPass::renderToGBuffer( RendererPtr const &renderer, RenderQueuePtr const &renderQueue, CameraPtr const &camera )
{
    auto gBuffer = getGBuffer( renderer );
    
    renderer->bindFrameBuffer( gBuffer );
    
    auto program = renderer->getShaderProgram( "deferred" );
    
    // bind program
    renderer->bindProgram( program );
    
    const Matrix4f &projection = camera->getProjectionMatrix();
    const Matrix4f &view = camera->getViewMatrix();
    
    renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::LINEAR_DEPTH_CONSTANT_UNIFORM ), camera->getFrustum().computeLinearDepth() );
    
    renderer->setAlphaState( AlphaState::DISABLED );
    renderer->setDepthState( DepthState::ENABLED );
    
    renderQueue->each( renderQueue->getOpaqueObjects(), [&]( MaterialPtr const &material, RenderQueue::PrimitiveMap const &primitives ) {
        // bind material properties
        renderer->bindMaterial( program, material );
        
        for ( auto it : primitives ) {
            auto primitive = it.first;
            
            // bind vertex and index buffers
            renderer->bindVertexBuffer( program, primitive->getVertexBuffer() );
            renderer->bindIndexBuffer( program, primitive->getIndexBuffer() );
            
            for ( auto geometryIt : it.second ) {
                auto world = geometryIt.second;
                renderer->applyTransformations( program, projection, view, world.computeModelMatrix() );
                renderer->drawPrimitive( program, primitive );
            }
            
            // unbind primitive buffers
            renderer->unbindVertexBuffer( program, primitive->getVertexBuffer() );
            renderer->unbindIndexBuffer( program, primitive->getIndexBuffer() );
        }
        
        // unbind material properties
        renderer->unbindMaterial( program, material );
        
    });
    
    renderer->unbindProgram( program );
    
    renderer->unbindFrameBuffer( gBuffer );
}

void DeferredRenderPass::composeFrame( RendererPtr const &renderer, RenderQueuePtr const &renderQueue, CameraPtr const &camera )
{
    auto sceneBuffer = renderer->getFrameBuffer( "scene" );
    if ( sceneBuffer == nullptr ) {
        int width = renderer->getScreenBuffer()->getWidth();
        int height = renderer->getScreenBuffer()->getHeight();
        
        sceneBuffer = crimild::alloc< FrameBufferObject >( width, height );
        sceneBuffer->getRenderTargets()->add( "depth", crimild::alloc< RenderTarget >( RenderTarget::Type::DEPTH_24, RenderTarget::Output::RENDER_AND_TEXTURE, width, height ) );
        sceneBuffer->getRenderTargets()->add( "color", crimild::alloc< RenderTarget >( RenderTarget::Type::COLOR_RGBA, RenderTarget::Output::TEXTURE, width, height ) );
        
        renderer->addFrameBuffer( "scene", sceneBuffer );
    }
    
    renderer->bindFrameBuffer( sceneBuffer );
    
    auto program = renderer->getShaderProgram( "deferred_compose" );
    
    renderer->bindProgram( program );
    
    renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::USE_SHADOW_MAP_UNIFORM ), _shadowMaps.size() > 0 );
    for ( auto it : _shadowMaps ) {
        if ( it.second != nullptr ) {
            renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::LIGHT_SOURCE_PROJECTION_MATRIX_UNIFORM ), it.second->getLightProjectionMatrix() );
            renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::LIGHT_SOURCE_VIEW_MATRIX_UNIFORM ), it.second->getLightViewMatrix() );
            renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::USE_SHADOW_MAP_UNIFORM ), true );
            renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::LINEAR_DEPTH_CONSTANT_UNIFORM ), it.second->getLinearDepthConstant() );
            renderer->bindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::SHADOW_MAP_UNIFORM ), it.second->getTexture() );
        }
    }
    
    // bind lights
    renderQueue->each( [&]( LightPtr const &light, int ) {
        renderer->bindLight( program, light );
    });
    
    // bind framebuffer texture
    auto gBuffer = renderer->getFrameBuffer( G_BUFFER_NAME );
    renderer->bindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::G_BUFFER_DEPTH_MAP_UNIFORM ), gBuffer->getRenderTargets()->get( G_BUFFER_DEPTH_TARGET_NAME )->getTexture() );
    renderer->bindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::G_BUFFER_COLOR_MAP_UNIFORM ), gBuffer->getRenderTargets()->get( G_BUFFER_COLOR_TARGET_NAME )->getTexture() );
    renderer->bindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::G_BUFFER_POSITION_MAP_UNIFORM ), gBuffer->getRenderTargets()->get( G_BUFFER_POSITION_TARGET_NAME )->getTexture() );
    renderer->bindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::G_BUFFER_NORMAL_MAP_UNIFORM ), gBuffer->getRenderTargets()->get( G_BUFFER_NORMAL_TARGET_NAME )->getTexture() );
    renderer->bindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::G_BUFFER_EMISSIVE_MAP_UNIFORM ), gBuffer->getRenderTargets()->get( G_BUFFER_VIEW_SPACE_NORMAL_TARGET_NAME )->getTexture() );
    
    renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::VIEW_MATRIX_UNIFORM ), camera->getViewMatrix() );
    
    renderer->drawScreenPrimitive( program );

    // unbind framebuffer texture
    renderer->unbindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::G_BUFFER_DEPTH_MAP_UNIFORM ), gBuffer->getRenderTargets()->get( G_BUFFER_DEPTH_TARGET_NAME )->getTexture() );
    renderer->unbindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::G_BUFFER_COLOR_MAP_UNIFORM ), gBuffer->getRenderTargets()->get( G_BUFFER_COLOR_TARGET_NAME )->getTexture() );
    renderer->unbindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::G_BUFFER_POSITION_MAP_UNIFORM ), gBuffer->getRenderTargets()->get( G_BUFFER_POSITION_TARGET_NAME )->getTexture() );
    renderer->unbindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::G_BUFFER_NORMAL_MAP_UNIFORM ), gBuffer->getRenderTargets()->get( G_BUFFER_NORMAL_TARGET_NAME )->getTexture() );
    renderer->unbindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::G_BUFFER_EMISSIVE_MAP_UNIFORM ), gBuffer->getRenderTargets()->get( G_BUFFER_VIEW_SPACE_NORMAL_TARGET_NAME )->getTexture() );
    
    // unbind lights
    renderQueue->each( [&]( LightPtr const &light, int ) {
        renderer->unbindLight( program, light );
    });
    
    for ( auto it : _shadowMaps ) {
        if ( it.second != nullptr ) {
            renderer->unbindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::SHADOW_MAP_UNIFORM ), it.second->getTexture() );
        }
    }
    
    renderer->unbindProgram( program );
    
    renderer->unbindFrameBuffer( sceneBuffer );
}

void DeferredRenderPass::computeShadowMaps( RendererPtr const &renderer, RenderQueuePtr const &renderQueue, CameraPtr const &camera )
{
    auto program = renderer->getShaderProgram( "depth" );
    if ( program == nullptr ) {
        return;
    }
    
    // bind shader program first
    renderer->bindProgram( program );
    
    renderQueue->each( [&]( LightPtr const &light, int ) {
        
        if ( !light->shouldCastShadows() ) {
            return;
        }
        
        auto map = _shadowMaps[ light ];
        if ( map == nullptr ) {
            map = crimild::alloc< ShadowMap >( light );
            map->getBuffer()->setClearColor( RGBAColorf( 1.0f, 1.0f, 1.0f, 1.0f ) );
            map->setLightProjectionMatrix( light->computeProjectionMatrix() );
            _shadowMaps[ light ] = map;
        }
        
        map->setLightViewMatrix( light->computeViewMatrix() );
        
        renderer->bindFrameBuffer( map->getBuffer() );
        
        renderer->setAlphaState( AlphaState::DISABLED );
        renderer->setDepthState( DepthState::ENABLED );
        
        renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::LINEAR_DEPTH_CONSTANT_UNIFORM ), map->getLinearDepthConstant() );
        
        renderQueue->each( renderQueue->getOpaqueObjects(), [&]( MaterialPtr const &material, RenderQueue::PrimitiveMap const &primitives ) {
            for ( auto it : primitives ) {
                auto primitive = it.first;
                
                renderer->bindVertexBuffer( program, primitive->getVertexBuffer() );
                renderer->bindIndexBuffer( program, primitive->getIndexBuffer() );
                
                for ( auto geometryIt : it.second ) {
                    renderer->applyTransformations( program, map->getLightProjectionMatrix(), map->getLightViewMatrix(), geometryIt.second.computeModelMatrix(), geometryIt.second.computeNormalMatrix() );
                    renderer->drawPrimitive( program, primitive );
                }
                
                // unbind primitive buffers
                renderer->unbindVertexBuffer( program, primitive->getVertexBuffer() );
                renderer->unbindIndexBuffer( program, primitive->getIndexBuffer() );
            }
        });
        
        renderer->unbindFrameBuffer( map->getBuffer() );
    });
    
    // unbind the shader program
    renderer->unbindProgram( program );
}

