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

#include "Visitors/NodeVisitor.hpp"
#include "SceneGraph/GroupNode.hpp"

#include "Utils/MockVisitor.hpp"

#include "gtest/gtest.h"

using namespace Crimild;

TEST( NodeVisitorTest, traversal )
{
	//		node0
	//		/   \
	//	node1	node2
	//			/	\
	//		node3	node4

	GroupNodePtr node0( new GroupNode( "node0" ) );
	GroupNodePtr node2( new GroupNode( "node2" ) );
	NodePtr node1( new Node( "node1" ) );
	NodePtr node3( new Node( "node3" ) );
	NodePtr node4( new Node( "node4" ) );

	node0->attachNode( node1 );
	node0->attachNode( node2 );
	node2->attachNode( node3 );
	node2->attachNode( node4 );

	MockVisitor visitor;

	EXPECT_CALL( visitor, reset() )
		.Times( ::testing::Exactly( 1 ) );
	EXPECT_CALL( visitor, visitNode( ::testing::_ ) )
		.Times( ::testing::Exactly( 3 ) );
	EXPECT_CALL( visitor, visitGroupNode( ::testing::_ ) )
		.Times( ::testing::Exactly( 2 ) )
		.WillRepeatedly( ::testing::Invoke( &visitor, &MockVisitor::NodeVisitor_visitGroupNode ) );
	node0->perform( visitor );

	EXPECT_CALL( visitor, reset() )
		.Times( ::testing::Exactly( 1 ) );
	EXPECT_CALL( visitor, visitNode( ::testing::_ ) )
		.Times( ::testing::Exactly( 2 ) );
	EXPECT_CALL( visitor, visitGroupNode( ::testing::_ ) )
		.Times( ::testing::Exactly( 1 ) )
		.WillRepeatedly( ::testing::Invoke( &visitor, &MockVisitor::NodeVisitor_visitGroupNode ) );
	node2->perform( visitor );
}

class PrependParentNameVisitor : public NodeVisitor {
public:
	virtual void visitNode( Node *node ) override
	{
		if ( node->hasParent() ) {
			node->setName( node->getParent()->getName() + "_" + node->getName() );
		}
	}

	virtual void visitGroupNode( GroupNode *group ) override
	{
		visitNode( group );
		NodeVisitor::visitGroupNode( group );
	}
};

TEST( NodeVisitorTest, prependParentName )
{
	//		node0
	//		/   \
	//	node1	node2
	//			/	\
	//		node3	node4

	GroupNodePtr node0( new GroupNode( "node0" ) );
	GroupNodePtr node2( new GroupNode( "node2" ) );
	NodePtr node1( new Node( "node1" ) );
	NodePtr node3( new Node( "node3" ) );
	NodePtr node4( new Node( "node4" ) );

	node0->attachNode( node1 );
	node0->attachNode( node2 );
	node2->attachNode( node3 );
	node2->attachNode( node4 );

	node0->perform( PrependParentNameVisitor() );

	EXPECT_EQ( node0->getName(), "node0" );
	EXPECT_EQ( node1->getName(), "node0_node1" );
	EXPECT_EQ( node2->getName(), "node0_node2" );
	EXPECT_EQ( node3->getName(), "node0_node2_node3" );
	EXPECT_EQ( node4->getName(), "node0_node2_node4" );
}


