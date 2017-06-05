#pragma once

#include "Graph.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

//forces busy wait for at least val seconds
void forceWait(const double val)
{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span;
	do {
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	} while (time_span.count() <val);
}


std::unique_ptr<graph::Graph> createGraph(unsigned int v, unsigned int e, const unsigned n, const unsigned x)
{

	std::unique_ptr<graph::Graph>  G1(new graph::Graph);

	v *= n;
	e *= x;

	for (unsigned int i = 0; i < v; i++)
	{
		G1->addVertex(i);
	}

	for (unsigned int i = 0; i < e*v; i++)
	{
		G1->addEdge((i * 2) % v, (i + 2) % v, i%v);
	}

	return std::move(G1);
	////remove all edges that meet lambda predicament
	//for (int i = 0; i < e*v; i++)
	//{
	//	G1.removeDirEdge(i%v, (i + 1) % v, [](int cost) ->bool {return cost == 100; });
	//}


	////remove half of vertices
	//for (int i = 0; i < v / 2; i++)
	//{
	//	G1.removeVertex(i);
	//}

	//for (auto& nodes : G1)
	//{
	//	std::cout << nodes.getID() << " ";
	//}
}


//creates n x n grid containing n^2 edges that each is connected by undirected edge to create a grid
std::unique_ptr<graph::Graph> grid_square(const unsigned int n)
{
	std::unique_ptr<graph::Graph> g(new graph::Graph);


	for (unsigned i = 0; i < n*n; ++i)
	{
		g->addVertex(i);
	}

	if (n < 2) //simple fix for edge case when there is nothing to connect + avoid overlapping with unsigned counters
	{
		return std::move(g);
	}


	for (unsigned i = 0; i < n - 1; ++i)
	{
		for (unsigned j = 0; j < n - 1; ++j)
		{
			//first set - going up and right
			std::size_t pos = i*(n)+j;
			g->addEdge(pos, pos + 1, 1, false);

			g->addEdge(pos, pos + n, 1, false);
		}

		//top edge going right, i=0 i<n-1
		g->addEdge(n*(n - 1) + i, n*(n - 1) + 1 + i);

	}

	for (unsigned i = 1; i<n; ++i)
	{
		//right edge going up
		g->addEdge((i*n - 1), (i*n) + n - 1);
	}

	return std::move(g);
}




TEST(VertexEdgeCountTest, OnlyCreation)
{
	std::unique_ptr<graph::Graph> g_ptr = grid_square(10);
	ASSERT_EQ(100, g_ptr->vertexCount());
	ASSERT_EQ(180, g_ptr->edgeCount());

	g_ptr = grid_square(4);
	ASSERT_EQ(16, g_ptr->vertexCount());
	ASSERT_EQ(24, g_ptr->edgeCount());


	g_ptr = grid_square(1);
	ASSERT_EQ(1, g_ptr->vertexCount());
	ASSERT_EQ(0, g_ptr->edgeCount());

	g_ptr = grid_square(0);
	ASSERT_EQ(0, g_ptr->vertexCount());
	ASSERT_EQ(0, g_ptr->edgeCount());

}


TEST(VertexEdgeCountTest, RemovedEdges)
{
	std::unique_ptr<graph::Graph> g_ptr = grid_square(10);
	g_ptr->removeDirEdge(0, 1);
	ASSERT_EQ(100, g_ptr->vertexCount());
	ASSERT_EQ(179, g_ptr->edgeCount());


	g_ptr = grid_square(4);
	g_ptr->removeDirEdge(0, 1);
	ASSERT_EQ(16, g_ptr->vertexCount());
	ASSERT_EQ(23, g_ptr->edgeCount());


	g_ptr = grid_square(1);
	g_ptr->removeDirEdge(0, 1); //not existing edge
	ASSERT_EQ(1, g_ptr->vertexCount());
	ASSERT_EQ(0, g_ptr->edgeCount());

	g_ptr = grid_square(0);
	g_ptr->removeDirEdge(0, 1); //not exisitng edge
	ASSERT_EQ(0, g_ptr->vertexCount());
	ASSERT_EQ(0, g_ptr->edgeCount());

}


TEST(VertexEdgeCountTest, RemovedVertex)
{
	std::unique_ptr<graph::Graph> g_ptr = grid_square(10);
	g_ptr->removeVertex(9);
	ASSERT_EQ(99, g_ptr->vertexCount());
	ASSERT_EQ(178, g_ptr->edgeCount());

	g_ptr->removeVertex(1);
	ASSERT_EQ(98, g_ptr->vertexCount());
	ASSERT_EQ(175, g_ptr->edgeCount());


	g_ptr = grid_square(4);
	g_ptr->removeVertex(3);
	ASSERT_EQ(15, g_ptr->vertexCount());
	ASSERT_EQ(22, g_ptr->edgeCount());
	g_ptr->removeVertex(1);
	ASSERT_EQ(14, g_ptr->vertexCount());
	ASSERT_EQ(19, g_ptr->edgeCount());


	g_ptr = grid_square(1);
	g_ptr->removeVertex(10); //not exist vertex
	ASSERT_EQ(1, g_ptr->vertexCount());
	ASSERT_EQ(0, g_ptr->edgeCount());

	g_ptr->removeVertex(0);
	ASSERT_EQ(0, g_ptr->vertexCount());
	ASSERT_EQ(0, g_ptr->edgeCount());

	g_ptr = grid_square(0);
	g_ptr->removeVertex(0); //not exist vertex
	ASSERT_EQ(0, g_ptr->vertexCount());
	ASSERT_EQ(0, g_ptr->edgeCount());

}

TEST(VertexEdgeCountTest, AddVertex)
{
	std::unique_ptr<graph::Graph> g_ptr = grid_square(10);
	g_ptr->addVertex(100);
	ASSERT_EQ(101, g_ptr->vertexCount());
	ASSERT_EQ(180, g_ptr->edgeCount());


	g_ptr = grid_square(4);
	g_ptr->addVertex(16);
	ASSERT_EQ(17, g_ptr->vertexCount());
	ASSERT_EQ(24, g_ptr->edgeCount());


	g_ptr = grid_square(1);
	g_ptr->addVertex(10);
	ASSERT_EQ(2, g_ptr->vertexCount());
	ASSERT_EQ(0, g_ptr->edgeCount());


	g_ptr = grid_square(0);
	g_ptr->addVertex(1);
	ASSERT_EQ(1, g_ptr->vertexCount());
	ASSERT_EQ(0, g_ptr->edgeCount());

	//add existing vertex
	g_ptr = grid_square(3);
	g_ptr->addVertex(3);
	ASSERT_EQ(9, g_ptr->vertexCount());
	ASSERT_EQ(12, g_ptr->edgeCount());

}


TEST(VertexEdgeCountTest, AddVertexWithoutID)
{
	std::unique_ptr<graph::Graph> g_ptr = grid_square(10);
	g_ptr->addVertex();
	ASSERT_EQ(101, g_ptr->vertexCount());
	ASSERT_EQ(180, g_ptr->edgeCount());


	g_ptr = grid_square(4);
	g_ptr->addVertex();
	ASSERT_EQ(17, g_ptr->vertexCount());
	ASSERT_EQ(24, g_ptr->edgeCount());


	g_ptr = grid_square(1);
	g_ptr->addVertex();
	ASSERT_EQ(2, g_ptr->vertexCount());
	ASSERT_EQ(0, g_ptr->edgeCount());


	g_ptr = grid_square(0);
	g_ptr->addVertex();
	ASSERT_EQ(1, g_ptr->vertexCount());
	ASSERT_EQ(0, g_ptr->edgeCount());

	//add 2 vertices and then add 1 vertice with already existing ID
	g_ptr = grid_square(3);
	g_ptr->addVertex();
	g_ptr->addVertex();
	g_ptr->addVertex(0); //already existing ID
	ASSERT_EQ(11, g_ptr->vertexCount());
	g_ptr->removeVertex(0); //remove ID
	ASSERT_EQ(10, g_ptr->vertexCount());
	g_ptr->addVertex(0); //insert it again
	ASSERT_EQ(11, g_ptr->vertexCount());



}



class GraphPathTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		std::size_t n = 5;
		//setup g0
		//straight line 0->(n-2) (missing link n-2 -> n-1) (eg 0->1->2->3 for n=5 )
		//links between i->i+2 ( eg 0->2,1->3, 2->4, 3->0, 4->1)
		for (std::size_t i = 0; i < n; ++i)
			g0.addVertex(i);

		for (std::size_t i = 0; i < n; ++i)
			g0.addEdge(i % n, (i + 1) % n, 1, true);

		for (std::size_t i = 0; i < n; ++i)
			g0.addEdge(i % n, (i + 2) % n, 1, true);

		g0.removeDirEdge((n-2), (n-1));

		//setup g1
		//straight line directed graph
		n = 10;
		for (std::size_t i = 0; i < n; ++i)
			g1.addVertex(i);

		for (std::size_t i = 0; i < n; ++i)
			g1.addEdge(i % n, (i + 1) % n, 1, true);

		g1.removeDirEdge((n - 2), (n - 1));


		//setup g2
		//straight line creating loop with edge at  (n-1)->0
		n = 5;
		for (std::size_t i = 0; i < n; ++i)
			g2.addVertex(i);

		for (std::size_t i = 0; i < n; ++i)
			g2.addEdge(i % n, (i + 1) % n, 1, true);

		//setup g3
		//every vertex is connected to each other but last vertex is not connected to anything (n=4, 0->1,2 , 1->2,0 etc and 3->nothing}
		n = 4;
		for (std::size_t i = 0; i < n; ++i)
			g3.addVertex(i);

		for (std::size_t i = 0; i < n-1; ++i)
			for(std::size_t j=1;j<n-1;++j)
				g3.addEdge(i % n, (j+i)% (n-1), 1, true);

		//setup g0u
		//straight line in undirected graph (like doubly-linked list)
		n = 5;
		for (std::size_t i = 0; i < n; ++i)
			g0u.addVertex(i);

		for (std::size_t i = 0; i < n-1; ++i)
			g0u.addEdge(i % n, (i + 1) % n, 1, false);

		


		//setup g1u
		//Vertex 0 has connections to 1..n/2 , n/2 has connections to n/2+1..n-1  (n=10 , 0->1..5 , 5->6..9 )
		n = 10;
		for (std::size_t i = 0; i < n; ++i)
			g1u.addVertex(i);

		for (std::size_t i = 0; i < n / 2; ++i)
			g1u.addEdge(0, i + 1, 1, false);

		for (std::size_t i = (n/2)+1 ; i < n; ++i)
			g1u.addEdge(n / 2, i, 1, false);

		
	}

	graph::Graph g0;
	graph::Graph g1;
	graph::Graph g2;
	graph::Graph g3;

	graph::Graph g0u;
	graph::Graph g1u;
};

TEST_F(GraphPathTest, PathFinding_DFS_directed) {
	
	
	std::vector<std::size_t> test1 = { 0,1,2,4 };
	EXPECT_THAT(g0.DFS(0, 4), ::testing::ContainerEq(test1));

	//find straight path
	std::vector<std::size_t> test2 = { 0,1,2,3,4,5,6};
	EXPECT_THAT(g1.DFS(0, 6), ::testing::ContainerEq(test2));

	//try to find non-existing path
	std::vector<std::size_t> test3 = {};
	EXPECT_THAT(g1.DFS(6,0), ::testing::ContainerEq(test3));
	EXPECT_FALSE(g1.traversable(6, 0));

	std::vector<std::size_t> test4 = { 2,3,4,0 };
	EXPECT_THAT(g2.DFS(2, 0), ::testing::ContainerEq(test4));

	std::vector<std::size_t> test5 = {};
	EXPECT_THAT(g3.DFS(1, 3), ::testing::ContainerEq(test5));
	EXPECT_FALSE(g3.traversable(0, 3));
	EXPECT_FALSE(g3.traversable(1, 3));
	EXPECT_FALSE(g3.traversable(2, 3));
	EXPECT_FALSE(g3.traversable(3, 0));
	EXPECT_EQ(g3.at(3).countEdges(), 0);

}

TEST_F(GraphPathTest, PathFinding_DFS_undirected) {

	//traverse "doubly-linked" list
	std::vector<std::size_t> test1 = { 0,1,2,3,4 };
	EXPECT_THAT(g0u.DFS(0, 4), ::testing::ContainerEq(test1));

	//traverse "doubly-linked" list from the end
	std::vector<std::size_t> test2 = { 4,3,2,1,0 };
	EXPECT_THAT(g0u.DFS(4, 0), ::testing::ContainerEq(test2));

	//traverse "doubly-linked" list from middle to start
	std::vector<std::size_t> test3 = { 3,2,1,0};
	EXPECT_THAT(g0u.DFS(3, 0), ::testing::ContainerEq(test3));

	//traverse graph with multiple edges per Vertex from start to last Vertex
	std::vector<std::size_t> test4 = { 0,5,9 };
	EXPECT_THAT(g1u.DFS(0, 9), ::testing::ContainerEq(test4));

	//traverse graph with multiple edges per Vertex from low level edge in reverse
	std::vector<std::size_t> test5 = { 7,5,0,2 };
	EXPECT_THAT(g1u.DFS(7, 2), ::testing::ContainerEq(test5));

	
}