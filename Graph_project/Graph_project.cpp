// Graph_project.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include "Graph.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>



int main()
{
	
	
	 
	std::ofstream file;
	file.open("maptest.txt");

	for (int x = 1; x < 2; x++)
	for (int n = 1; n < 5;n+=4)
	{
		int v = 5;
		int e = 4;
		graph::Graph G1;
		v *= n;
		e *= x;
	{
		//force wait
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span;
		do {



			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
			time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		} while (time_span.count() < 0.40);

	}



	//G1.addVertex();
	//G1.addVertex();
	//G1.addVertex();
	
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < v; i++)
		{
			G1.addVertex(i);
		}

		for (int i = 0; i < e*v; i++)
		{
			G1.addEdge(i%v, (i+1)%v,i%v);
		}

		for (int i = 0; i < e*v; i++)
		{
			G1.removeDirEdge(i%v, (i + 1) % v, [](int cost) ->bool {return cost == 3; }); 
		}

		for (int i = 0; i < v; i++)
		{
			G1.removeVertex(i);
		}

	
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
	
		std::cout <<"vertices total="<< v <<"edges per vertice="<<e<<"time=" << time_span.count() << std::endl;
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
		} while (time_span.count() < 0.50);

	}
	_CrtDumpMemoryLeaks();


    return 0;
}

