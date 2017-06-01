// Graph_project.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include "Graph.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include "DataGraph.h"
#include "gtest/gtest.h"

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


std::unique_ptr<graph::Graph> createGraph(unsigned int v, unsigned int e, const unsigned n,const unsigned x)
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


	for (unsigned i = 0; i < n-1; ++i)
	{
		for (unsigned j = 0; j < n -1; ++j)
		{
			//first set - going up and right
			std::size_t pos = i*(n) + j;
			g->addEdge(pos , pos+1, 1,false);

			g->addEdge(pos , pos+n, 1, false);
		}

		//top edge going right, i=0 i<n-1
		g->addEdge(n*(n - 1) + i, n*(n - 1) + 1 + i);

	}

	for(unsigned i=1;i<n;++i)
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

TEST(PathFinding, DFS_directed)
{
	graph::Graph g;
	
	for (std::size_t i = 0; i < 5; ++i)
		g.addVertex(i);

	for (std::size_t i = 0; i < 5; ++i)
		g.addEdge(i%5, (i+1)% 5, 1, false);

	for (std::size_t i = 0; i < 5; ++i)
		g.addEdge(i % 5, (i + 2) % 5, 1, false);

	g.removeDirEdge(3, 4);

	std::vector<std::size_t> test1 = { 0,1,2,4 };

	std::string test1str = "0124"; //temporary for checking result
	std::vector<std::size_t> res = g.DFS(0, 4);
	
	std::string resstr;
		
	for (auto it : res)
		resstr += it;

	EXPECT_EQ(0, res.size());


	//#TODO add google mock to project!
	//EXPECT_THAT(g.DFS(0, 4), ::testing::ContainerEq(test1));
	//EXPECT_STRCASEEQ(test1str.c_str(), resstr.c_str());



}


int main(int argc, _TCHAR* argv[])
{
	 
	//std::ofstream file;
	//file.open("maptest.txt");


	
	//const unsigned a = 3;
	////create a square net of vertices
	//std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();

	//std::unique_ptr<graph::Graph> graph_ptr = grid_square(a);

	//std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();

	//std::chrono::duration<double> time_span3 = std::chrono::duration_cast<std::chrono::duration<double>>(t4 - t3);
	//std::cout << "\t\t-square=" << a*a << "\ttime=" << time_span3.count() << std::endl;
	//std::cout << "\t\t-edges=" << graph_ptr->edgeCount() << "\tt-vertices=" << graph_ptr->vertexCount()<< std::endl;
	////graph_ptr.reset();

	//for(int x=1;x<1;++x)
	//	for (int n = 1; n < 2; n += 1)
	//	{
	//		//start clock
	//		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	//		std::unique_ptr<graph::Graph> graph_ptr = createGraph(10, 4, n, x);

	//		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	//			
	//		//get duration
	//		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	//		std::cout << "vertices total= " << n*10<< "\tedges per vertice= " << 4*x << "time= " << time_span.count() << std::endl;
	//		file << n * 10 << "\t" << 4 * x << "\t" << time_span.count() << "\n";


	//		graph_ptr->removeDirEdge(0, 2);
	//		graph_ptr->removeDirEdge(1, 2);

	//		std::cout<<graph_ptr->edgeCount();
	//
	//	




	//}
	//

	///*std::cout << graph_ptr->edgeCount() << "\t" << graph_ptr->vertexCount() << std::endl;
	//
	//std::vector<std::string> vec = graph_ptr->verticesToString();
	//for(auto it:vec)
	//	std::cout <<it << std::endl;
	//*/

	//graph_ptr->removeVertex(1);
	//std::cout << "removed vert1";
	//graph_ptr->removeVertex(0);
	//
	//for (auto &it : *graph_ptr.get())
	//{

	//	std::cout << it.getID() << "\tout of:" << graph_ptr->vertexCount() << std::endl;
	//	auto edge = (it.findOutEdge(&it));
	//	if (edge != nullptr)
	//		std::cout << "loop edge=" << edge->getDestination() << std::endl;

	//	//auto outEdge_it = it.begin_outEdge();
	//	
	//	//it.findInEdge(it, [](const int cost)->bool {return cost == 100; });
	//	//it.findOutEdge(it, [](short cost) ->bool {return cost == 100; });
	//	// [](const int cost)->bool {return cost == 100; }

	//	for (auto it2 : it)
	//	{

	//		//std::cout << it2.getDestination() << " directed:" << it2.isDirected() << std::endl;
	//		it2.setCost(10);
	//	}

	//	auto inEdge_it = it.begin_inEdge();
	//	//inEdge_it->isDirected();
	//	if (inEdge_it != it.end_inEdge())
	//	{
	//		std::cout << inEdge_it->getCost() << "\t" <<
	//			inEdge_it->getDestination() << "\t" <<
	//			inEdge_it->getSource() << "\t" <<
	//			inEdge_it->getID() << "\t" <<
	//			inEdge_it->isDirected() << "\n";
	//	}
	//
	//}


	//

	//graph_ptr.reset();
	

::testing::InitGoogleTest(&argc, argv);




    return RUN_ALL_TESTS();

	getchar();
	_CrtDumpMemoryLeaks();

}

