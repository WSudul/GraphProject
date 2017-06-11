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
#include "Tests.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"







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


	//graph_ptr.reset();


	graph::Graph g1;
	std::size_t n = 5;
	for(std::size_t i = 0; i < n; ++i)
		g1.addVertex(i);

	for (std::size_t i = 0; i < n; ++i)
		g1.addEdge(i % n, (i + 1) % 5, 1, true);

	g1.removeDirEdge(4, 0);
	
	std::vector<std::string> v = g1.verticesToString();
	std::string str;
	for (auto &it : v)
		str += it;
	std::cout <<str << std::endl;


	if (g1.BFS(0, 4).size()>0)
	{
		std::cout << "true";
	}
	else
		std::cout << "false";
	

	::testing::InitGoogleTest(&argc, argv);



	RUN_ALL_TESTS();

  

	
	//_CrtDumpMemoryLeaks();
	return 0;
}

