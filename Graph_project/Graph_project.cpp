// Graph_project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Graph.h"
#include <iostream>
#include <string>
#include <vector>


int main()
{

	graph::Graph G1;
	
	G1.addVertex();
	G1.addVertex();
	G1.addVertex();
	G1.addEdge(1, 2, 10);
	G1.addVertex();
	G1.addEdge(1, 3, 22);
	std::vector<std::string> vec=G1.verticesToString();

	for (const auto &it : vec)
		std::cout << it << std::endl;

	G1.removeEdge(1, 2);


    return 0;
}

