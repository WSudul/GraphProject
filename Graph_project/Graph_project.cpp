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
	const int size = 1000;
	std::ofstream file;
	file.open("maptest.txt");
	for (int n = 1; n < 1000; n+=10)
	{
	graph::Graph G1;
	
	G1.addVertex();
	G1.addVertex();
	G1.addVertex();
	
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < size * n; i++)
		{
			G1.addVertex(i);
		}

		for (int i = 0; i < size * n*4; i++)
		{
			G1.addEdge(i, i%size,10);
		}

	
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
	
	std::cout <<"size="<< size*n <<"time=" << time_span.count() << std::endl;
	file << size * n << "\t" << time_span.count() << "\n";
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

    return 0;
}

