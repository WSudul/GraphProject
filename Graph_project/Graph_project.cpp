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

	std::vector<std::string> vec=G1.verticesToString();

	for (const auto &it : vec)
		std::cout << it << std::endl;

    return 0;
}

