#pragma once
#include <algorithm>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <iterator>
#include <memory>
#include <string>
/*!
goals:

Node access -> O(1)
Add node	-> O(1)
Add edge	-> O(1)
Remove edge -> O(~1)
Remove node -> O(V+E)

Class representing graph via adjacency list
#TODO Implement all needed methods and throw away notion of storing thrice cursed grid in Matrix

#TODO NEED TO BE ABLE TO ADDRESS SPECIFIC NODES SOMEHOW. SHOULD BE CHEAP TO DO!
#TODO implement the class by assuming that 1 each vertex has unique_ID that has nothing to do with "position" in the grid
#TODO edge class

#TODO add node,edge and then methods for removing them safely in directed and undirected graphs
*/

namespace graph {

	class Graph
	{
	public:
		Graph();
		virtual ~Graph();



		std::size_t addVertex();
		void addVertex(std::size_t id);
		void addEdge(std::size_t from, std::size_t to, int cost);
	
		void removeEdges(std::size_t from, std::size_t to);

		void removeEdge(std::size_t from, std::size_t to);

		void removeNode(std::size_t node);

		std::size_t vertexCount();
		std::size_t edgeCount();

		
		std::string vertexToString(const std::size_t &name);
		std::string edgeToString();

		std::vector<std::string> verticesToString();
		std::vector<std::string> edgesToString();
	private:

		class Vertex;

		class Edge;

		//map of all vertices,key is unique ID assigned to each vertex
		std::unordered_map<std::size_t, std::unique_ptr<Vertex>> Vertices;
		

		std::string Graph::vertexToString(const std::pair <const std::size_t , std::unique_ptr<Vertex>>& it);

	};


	class Graph::Vertex
	{
	public:
		Vertex(std::size_t id);

		virtual ~Vertex();

		std::size_t getID();
		//std::list<T>::const_iterator edges();
		//std::list<T>::iterator edges();
		std::size_t countEdges();

		const graph::Graph::Edge*  findOutEdge(const Vertex*  to) const;
		const graph::Graph::Edge*  findInEdge(const Vertex*  from) const;

		void addInEdge(Edge* edge);
		void addOutEdge(Edge* edge);


		void removeInEdge(const std::size_t to, const Edge* edge);
		void removeOutEdge(const std::size_t from, const Edge* edge);

		void removeInEdge(const Vertex* to, const Edge* edge);
		void removeOutEdge(const Vertex* from, const Edge* edge);

	private:

		std::size_t uniq_id; //unique ID #TODO enforcing uniquness and reclamation of unused IDs without 



		std::vector<Edge*> inEdges; //list of all edges that are pointing to this node
		//std::vector<Edge*> outEdges; //list of all edges that are pointing from this node
		std::vector<std::unique_ptr<Edge>> outEdges;
	};


	class Graph::Edge
	{
	public:
		Edge();
		Edge(Vertex* source, Vertex* destination, int cost);
		virtual ~Edge();
		//Edge(std::size_t destination, int cost = 1);

		bool operator==(const Edge& rhs) const
		{
			return (this->cost == rhs.cost && this->source == rhs.source && this->destination == rhs.source);
		}

		//const Vertex<T>* getSource();
		//const Vertex<T>* getDestination();
		int getCost();
		void setCost(int cost);

		std::size_t getID();


		Vertex* getSource() const;
		Vertex* getDestination() const;


	private:
		int cost;
		bool directed;
		std::size_t id;
		Vertex* source;
		Vertex* destination;

	};



}