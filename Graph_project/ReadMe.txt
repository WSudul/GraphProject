========================================================================
    CONSOLE APPLICATION : Graph_project Project Overview
========================================================================

Attempt to implement a Graph class that would  represent a graph structure via modified version of adjacency list.

Graph class has following nested classes:
1. Vertex
2. Edge
3. InEdgeIterator
4. OutEdgeIterator

Vertex

-has unique id
-has 2 edge containers
	-outEdges	- edges pointing from this Vertex to other Vertices (stored as smart pointers)
	-inEdges	- edges pointing to this Vertex from other Vertices (stored currently as raw pointers) // review,and refractoring pending


Edge

-represents a link between 2 Vertices
-can point to source Vertex
-is directed (2 way connection requires 2 separate edges)
-currently has no unique ID 
-comparision between edges (for finding/removing) is based on raw pointer address //review and refractoring pending

InEdgeIterator, OutEdgeIterator

- wrap for outEdges,inEdges interators
- has basic facilities for forward iterator 
- random access planned and const iterator versions
//plans to conform to new C++17 scheme, aka boiler plate typedefs, pending

Space complexity:

O(V+E)

space requirements for Edges is O(n+sizeof(raw_ptr) because of the needs to store information in 2 containers. Shared ptr might be considered


Goals:

1. Creating base structure for weighted graph using Edges and Vertices //working, draft atm
2. Creating digraphs (directed graphs) with  weights //working
3. Creating multigraphs //working
4. Implementing removing vertices/edges with user specified predicaments (templated functions) //done, working 
5. Improving overall performance // insert unordered_map works fastest when providing ID (O(1)) but is 'slightly' slower than map (O(log(n))) when not specifing ID.
6. Overloading operators and providing move constructor 
7. Preparing class to serve as base class (avoid templating atm <reason: mixing derived objects in single graph>)
8. Providing iterators for internal containers (inEdge,outEdge) // done,albeit requires review