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
//plans to conform to new C++17 scheme, aka boiler plate typedefs, pending //work started

Space complexity:

O(V+E)

space requirements for Edges is O(n+sizeof(raw_ptr) because of the needs to store information in 2 containers. Shared ptr might be considered
2017-05-25 Update regarding shared_ptr:

Initial rework with shared_ptr showed that creating graph is 20-29% slower and takes aprox 40% more space (can be optimized to be lower).
Shared_ptr version will be developed in develepoment_branch separetely


Goals:

1. Creating base structure for weighted graph using Edges and Vertices //working, draft atm
2. Creating digraphs (directed graphs) with  weights //working , need to implement flag usage to avoid 2 separate of undirected edge (lower memory by increasing find/remove complexity). 
3. Creating multigraphs //working
4. Implementing removing vertices/edges with user specified predicaments (templated functions) //done, working 
5. Improving overall performance // insert unordered_map works fastest when providing ID (O(1)) but is 'slightly' slower than map (O(log(n))) when not specifing ID.
6. Overloading operators and providing move constructor 
7. Preparing class to serve as base class (avoid templating atm <reason: mixing derived objects in single graph>) //Requires major rework and will result in huge performance hit for get/set on Data contained in Vertex/Edge (need to keep them in uniform format, ex XML encoded and this will create a lot of overhead for simple get/set operations). Template will be provided and then deriving from Graphs that share the same type of data field will be easily possible.
8. Providing iterators for internal containers (inEdge,outEdge) // done,albeit requires review