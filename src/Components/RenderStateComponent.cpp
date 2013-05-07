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

#include "RenderStateComponent.hpp"
#include "Rendering/Material.hpp"

using namespace Crimild;

const char *RenderStateComponent::NAME = "renderState";

RenderStateComponent::RenderStateComponent( void )
	: NodeComponent( NAME )
{
}

RenderStateComponent::~RenderStateComponent( void )
{
	detachAllMaterials();
	detachAllLights();
}

void RenderStateComponent::attachMaterial( Material *material )
{
	_materials.push_back( material );
}

void RenderStateComponent::detachAllMaterials( void )
{
	_materials.clear();
}

void RenderStateComponent::foreachMaterial( std::function< void( Material * ) > callback )
{
	std::for_each( std::begin( _materials ), std::end( _materials ), callback );
}

void RenderStateComponent::attachLight( Light *light )
{
	_lights.push_back( light );
}

void RenderStateComponent::detachAllLights( void )
{
	_lights.clear();
}

void RenderStateComponent::foreachLight( std::function< void( Light * ) > callback )
{
	std::for_each( std::begin( _lights ), std::end( _lights ), callback );
}
