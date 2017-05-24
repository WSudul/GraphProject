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



int main()
{
	 
	std::ofstream file;
	file.open("maptest.txt");

	for (int x = 1; x < 4; x++)
	for (int n = 1; n < 50000;n+=1000)
	{
		int v = 10;
		int e = 4;
		graph::Graph G1;
		v *= n;
		e *= x;
		
		graph::Graph* g2= new DataGraph<int>;
		graph::Graph* g3= new DataGraph<float>;

		g2->addVertex(1);
		g2->addVertex(2);

		

		std::vector<graph::Graph*> gvec;
		gvec.push_back(g2);
		gvec.push_back(g3);

		//forceWait(0.50);

		//start clock
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < v; i++)
		{
			G1.addVertex(i);
		}

		for (int i = 0; i < e*v; i++)
		{
			G1.addEdge((i*2)%v, (i+2)%v,i%v);
		}

		//remove all edges that meet lambda predicament
		for (int i = 0; i < e*v; i++)
		{
			G1.removeDirEdge(i%v, (i + 1) % v, [](int cost) ->bool {return cost == 100; }); 
		}

		

		//remove half of vertices
		for (int i = 0; i < v/2; i++)
		{
			G1.removeVertex(i);
		}

		for (auto& nodes : G1)
		{
			//std::cout<<nodes.getID()<<" ";
		}

	
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
	
		std::cout <<"vertices total= "<< v <<"\tedges per vertice= "<<e<<"time= " << time_span.count() << std::endl;
		file << v << "\t" <<e<<"\t"<< time_span.count() << "\n";




	}
	//G1.addEdge(1, 2, 10);
	//G1.addVertex();
	//G1.addEdge(1, 3, 22);
	//std::vector<std::string> vec=G1.verticesToString();

	////for (const auto &it : vec)
	//	//std::cout << it << std::endl;

	//G1.removeEdge(1, 2);
	//
	//vec = G1.verticesToString();

	////for (const auto &it : vec)
	//	//std::cout << it << std::endl;

	{
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span;
		do {



			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
			time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		} while (time_span.count() < 0.00);

	}
	_CrtDumpMemoryLeaks();


    return 0;
}

