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

#include "SceneGraph/Group.hpp"
#include "Exceptions/HasParentException.hpp"

#include "gtest/gtest.h"

using namespace crimild;

TEST( GroupNodeTest, construction)
{
	GroupPtr group( new Group( "a group" ) );

	EXPECT_EQ( group->getName(), "a group" );
	EXPECT_FALSE( group->hasNodes() );
}

TEST( GroupNodeTest, destruction )
{
	NodePtr child1( new Node( "child 1" ) );
	NodePtr child2( new Node( "child 2" ) );

	{
		GroupPtr parent( new Group( "parent" ) );

		parent->attachNode( child1 );
		EXPECT_TRUE( child1->hasParent() );
		EXPECT_EQ( child1->getParent(), parent.get() );

		parent->attachNode( child2 );
		EXPECT_TRUE( child2->hasParent() );
		EXPECT_EQ( child2->getParent(), parent.get() );
	}

	EXPECT_FALSE( child1->hasParent() );
	EXPECT_EQ( child1->getParent(), nullptr );
	EXPECT_FALSE( child2->hasParent() );
	EXPECT_EQ( child2->getParent(), nullptr );
}

TEST( GroupNodeTest, attachNode )
{
	GroupPtr parent( new Group( "parent" ) );

	EXPECT_FALSE( parent->hasNodes() );

	NodePtr child( new Node( "child" ) );
	EXPECT_FALSE( child->hasParent() );
	EXPECT_EQ( child->getParent(), nullptr );

	parent->attachNode( child );

	EXPECT_TRUE( child->hasParent() );
	ASSERT_EQ( child->getParent(), parent.get() );
	EXPECT_TRUE( parent->hasNodes() );

	bool found = false;
	int count = 0;
	parent->foreachNode( [&]( NodePtr node ) mutable {
		++count;
		if ( node == child ) {
			found = true;
		}
	});

	EXPECT_EQ( count, 1 );
	EXPECT_TRUE( found );
}

TEST( GroupNodeTest, attachMultipleNodes )
{
	GroupPtr parent( new Group( "parent" ) );
	
	NodePtr child1( new Node( "child 1" ) );
	NodePtr child2( new Node( "child 2" ) );
	NodePtr child3( new Node( "child 3" ) );

	parent->attachNode( child1 );
	parent->attachNode( child2 );
	parent->attachNode( child3 );

	EXPECT_TRUE( parent->hasNodes() );

	int count = 0;
	parent->foreachNode( [&]( NodePtr node ) mutable {
		++count;
	});
	EXPECT_EQ( count, 3 );
}

TEST( GroupNodeTest, reattachNodeToSameParent )
{
	GroupPtr parent( new Group( "parent" ) );
	NodePtr child( new Node( "child" ) );

	parent->attachNode( child );

	EXPECT_NO_THROW( parent->attachNode( child ) );

	bool found = false;
	int count = 0;
	parent->foreachNode( [&]( NodePtr node ) mutable {
		++count;
		if ( node == child ) {
			found = true;
		}
	});

	EXPECT_EQ( count, 1 );
	EXPECT_TRUE( found );
}

TEST( GroupNodeTest, reattachNodeToDifferentParent )
{
	GroupPtr parent1( new Group( "parent" ) );
	GroupPtr parent2( new Group( "another parent" ) );
	NodePtr child( new Node( "child" ) );

	parent1->attachNode( child );

	ASSERT_THROW( parent2->attachNode( child ), HasParentException );
}

TEST( GroupNodeTest, detachNode )
{
	GroupPtr parent( new Group( "parent" ) );
	NodePtr child( new Node( "child" ) );

	parent->attachNode( child );
	parent->detachNode( child );

	EXPECT_FALSE( parent->hasNodes() );
	EXPECT_FALSE( child->hasParent() );
	EXPECT_EQ( child->getParent(), nullptr );
}

TEST( GroupNodeTest, detachMultipleNodes )
{
	GroupPtr parent( new Group( "parent" ) );
	
	NodePtr child1( new Node( "child 1" ) );
	NodePtr child2( new Node( "child 2" ) );
	NodePtr child3( new Node( "child 3" ) );

	parent->attachNode( child1 );
	parent->attachNode( child2 );
	parent->attachNode( child3 );

	parent->detachNode( child1 );
	parent->detachNode( child2 );

	EXPECT_TRUE( parent->hasNodes() );

	int count = 0;
	parent->foreachNode( [&]( NodePtr node ) {
		EXPECT_EQ( node->getName(), child3->getName() );
		++count;
	});
	EXPECT_EQ( count, 1 );
}

TEST( GroupNodeTest, detachNodeFromDifferentParent )
{
	GroupPtr parent1( new Group( "parent" ) );
	GroupPtr parent2( new Group( "another parent" ) );
	NodePtr child( new Node( "child" ) );

	parent1->attachNode( child );
	parent2->detachNode( child );

	EXPECT_TRUE( child->hasParent() );
	ASSERT_EQ( child->getParent(), parent1.get() );
	EXPECT_TRUE( parent1->hasNodes() );
	EXPECT_FALSE( parent2->hasNodes() );
}

TEST( GroupNodeTest, detachAllNodes )
{
	GroupPtr parent( new Group( "parent" ) );
	
	NodePtr child1( new Node( "child 1" ) );
	NodePtr child2( new Node( "child 2" ) );
	NodePtr child3( new Node( "child 3" ) );

	parent->attachNode( child1 );
	parent->attachNode( child2 );
	parent->attachNode( child3 );

	parent->detachAllNodes();

	EXPECT_FALSE( parent->hasNodes() );

	EXPECT_FALSE( child1->hasParent() );
	EXPECT_EQ( child1->getParent(), nullptr );
	EXPECT_FALSE( child2->hasParent() );
	EXPECT_EQ( child2->getParent(), nullptr );
	EXPECT_FALSE( child3->hasParent() );
	EXPECT_EQ( child3->getParent(), nullptr );

	int count = 0;
	parent->foreachNode( [&]( NodePtr node ) {
		++count;
	});

	EXPECT_EQ( count, 0 );
}

TEST( GroupNodeTest, buildHierarchy )
{
	//		node0
	//		/   \
	//	node1	node2
	//			/	\
	//		node3	node4

	GroupPtr node0( new Group( "node0" ) );
	GroupPtr node2( new Group( "node2" ) );
	NodePtr node1( new Node( "node1" ) );
	NodePtr node3( new Node( "node3" ) );
	NodePtr node4( new Node( "node4" ) );

	node0->attachNode( node1 );
	node0->attachNode( node2 );
	node2->attachNode( node3 );
	node2->attachNode( node4 );

	EXPECT_TRUE( node0->hasNodes() );
	EXPECT_EQ( node1->getParent(), node0.get() );
	EXPECT_EQ( node2->getParent(), node0.get() );

	EXPECT_TRUE( node2->hasNodes() );
	EXPECT_EQ( node3->getParent(), node2.get() );
	EXPECT_EQ( node4->getParent(), node2.get() );
}
