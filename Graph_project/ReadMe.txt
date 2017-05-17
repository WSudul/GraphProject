========================================================================
    CONSOLE APPLICATION : Graph_project Project Overview
========================================================================

Attempt to implement a Graph class that would  represent a graph structure via adjacency list.


Goals:

1. Creating base structure for weighted graph using Edges and Vertices //working draft atm
2. Creating digraphs (directed graphs) with  weights //working
3. Creating multigraphs //working
4. Implementing removing vertices/edges with user specified predicaments (templated functions) //started
5. Improving overall performance // insert unordered_map works fastest when providing ID (O(1)) but is 'slightly' slower than map (O(log(n))) when not specifing ID.
6. Overloading operators and providing move constructor
7. Preparing class to serve as base class (avoid templating atm <reason: mixing derived objects in single graph>)