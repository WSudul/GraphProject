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


std::unique_ptr<graph::Graph> grid_square(const unsigned int n)
{
	std::unique_ptr<graph::Graph> g(new graph::Graph);
	if (n == 0)
	{
		return std::move(g);
	}

	for (unsigned i = 0; i < n; ++i)
	{
		for (unsigned j = 0; j < n; ++j)
			g->addVertex(i*n + j);
	}
	//
	for (unsigned i = 1; i < n - 1; ++i)
	{
		for (unsigned j = 1; j < n - 1; ++j)
		{
			//add edges

			//first set
			std::size_t pos = i*n + j;
			g->addEdge(pos + 1, pos);
			g->addEdge(pos - 1, pos);
			g->addEdge(pos + n, pos);
			g->addEdge(pos - n, pos);

			//returning edges
			g->addEdge(pos, pos + 1);
			g->addEdge(pos, pos - 1);
			g->addEdge(pos, pos + n);
			g->addEdge(pos, pos - n);
		}
	}
		//border edges
		for (unsigned i = 0; i < n ; ++i)
		{
			std::size_t pos;
			//bottom border
			g->addEdge(i, i + n);
			g->addEdge(i + n, i);

			//top border
			g->addEdge((n - 1)*n + i, (n - 2)*n + i);
			g->addEdge((n - 2)*n + i, (n - 1)*n + i);

			//left border
			pos = n*i;
			g->addEdge(pos, (pos + 1));
			g->addEdge((pos + 1), pos);

			//right border
			pos = (n - 1) + n*i;
			g->addEdge(pos, pos - 1);
			g->addEdge(pos - 1, pos);

		};

	

	return std::move(g);
}

int main()
{
	 
	std::ofstream file;
	file.open("maptest.txt");


		
	graph::Graph* g2= new DataGraph<int>;
	graph::Graph* g3= new DataGraph<float>;

	delete g2;
	delete g3;

	const unsigned a = 2;
	//create a square net of vertices
	std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();

	std::unique_ptr<graph::Graph> graph_ptr = grid_square(a);

	std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> time_span3 = std::chrono::duration_cast<std::chrono::duration<double>>(t4 - t3);
	std::cout << "\t\t-square=" << a*a << "\ttime=" << time_span3.count() << std::endl;
	std::cout << "\t\t-edges=" << graph_ptr->edgeCount() << "\tt-vertices=" << graph_ptr->vertexCount()<< std::endl;
	graph_ptr.reset();

	for(int x=1;x<2;++x)
		for (int n = 1; n < 2; n += 1)
		{
			//start clock
			std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

			std::unique_ptr<graph::Graph> graph_ptr = createGraph(10, 4, n, x);

			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
				
			//get duration
			std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
			std::cout << "vertices total= " << n*10<< "\tedges per vertice= " << 4*x << "time= " << time_span.count() << std::endl;
			file << n * 10 << "\t" << 4 * x << "\t" << time_span.count() << "\n";


			graph_ptr->removeDirEdge(0, 2);
			graph_ptr->removeDirEdge(1, 2);

			std::cout<<graph_ptr->edgeCount();
	
		




	}
	

	////for (const auto &it : vec)
	//	//std::cout << it << std::endl;

	//G1.removeEdge(1, 2);
	//
	//vec = G1.verticesToString();

	////for (const auto &it : vec)
	//	//std::cout << it << std::endl;


	_CrtDumpMemoryLeaks();


    return 0;
}

