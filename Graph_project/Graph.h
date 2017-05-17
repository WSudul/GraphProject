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



		/*!
			add a Vertex by automatically assigning next free ID after current max ID
			returns assigned ID
		
		*/
		std::size_t addVertex();

		/*!
			adds a Vertex with specified ID. If Graph contains the Vertex with same ID it will do nothing
		*/
		void addVertex(std::size_t id);

		/*!
			adds new Edge between 2 Vertices (from,to) with a certain cost of traversal.
			If edge is undirected, it will create 2 edges (from->to and to->from)
		*/
		void addEdge(std::size_t from, std::size_t to, int cost=1, bool directed = true);
	

		
		void removeEdges(std::size_t from, std::size_t to);

		/*!
			removes an edge between from and to vertices.
			Removes the first edge found without regard for directed flag and cost value
		*/
		void removeDirEdge(std::size_t from, std::size_t to);


		/*!
			Template method for removing single edge.
			Removes a first edge that meets the conditions defined by Pred
		*/
		template<typename _Pr>
		void removeDirEdge(std::size_t from, std::size_t to,_Pr& Pred )
		{
			//#TODO refractor this

			//finds from,to vertices
			auto &it_from = Vertices.find(from);
			auto &it_to = Vertices.find(to);
			auto &it_end = Vertices.end();

			//check if both ends are existing
			if (it_from != it_end && it_to != it_end)
			{
				//finds matching edge (O(deg(V)))
				const Edge*  edgePtr = nullptr;
				Vertex*  vertexFromPtr = it_from->second.get();
				Vertex*  vertexToPtr = it_to->second.get();
				edgePtr = vertexFromPtr->findOutEdge(vertexToPtr,Pred);
				if (edgePtr != nullptr)
				{
					it_to->second->removeInEdge(it_from->second.get(), edgePtr); //remove pointer to edge from receiving vertex
					it_from->second->removeOutEdge(it_to->second.get(), edgePtr); //remove the edge itself
				}
			}


		}


		

		/*!
			remove single vertex with specified id
		*/
		void removeVertex(std::size_t node);

		/*!
			returns number of vertices stored in graph
		*/
		std::size_t vertexCount();

		/*!
			returns number of edges stored in graph.
			Potential overflow value might occur if total number of edges is greater than size of size_t
		*/
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
		
		/*!
			returns string representation of single vertice
		*/
		std::string Graph::vertexToString(const std::pair <const std::size_t , std::unique_ptr<Vertex>>& it);

	};


	class Graph::Vertex
	{
	public:

		/*!
			parametrized ctor
		*/
		Vertex(std::size_t id);

		/*!
			dtor
		*/
		virtual ~Vertex();


		/*!
			returns ID of vertex
		*/
		std::size_t getID();

		/*!
			returns the  total number of edges that go out of this vertex
		*/
		std::size_t countEdges();


		/*!
			returns a pointer to first Edge that reaches to vertex based on specified predicament
		*/
		const graph::Graph::Edge*  findOutEdge(const Vertex*  to) const;

		
		/*!
			Template method for findind outedge
			returns const pointer to outedge that starts at from Vertex that meets the predicament
		*/
		template<typename _Pr>
		const graph::Graph::Edge*  findOutEdge(const Vertex*  to,_Pr& Pred) const
		{
			auto &it = std::find_if(outEdges.begin(), outEdges.end(), [&to, &Pred](const std::unique_ptr<Edge>& E)->bool {return E->getDestination() == to && Pred(E->getCost()); });

			if (it != outEdges.end())
			{
				return it->get();
			}

			return nullptr;
		}


		/*!
			returns const pointer to inedge that starts at from Vertex
		*/
		const graph::Graph::Edge*  findInEdge(const Vertex*  from) const;


		/*!
			returns a pointer to first Edge that arrives from vertex based on specified predicament
		*/
		template<typename _Pr>
		const graph::Graph::Edge*  findInEdge(const Vertex*  from,_Pr& Pred) const
		{

			const auto &it = std::find_if(inEdges.begin(), inEdges.end(), [&from,&Pred](const Edge* E)->bool {return E->getDestination() == from && Pred(E->getCost()); });


			if (it != inEdges.end())
			{
				return *it;
			}

			return nullptr;
		}


		/*!
			adds inedge to vertex
		*/
		void addInEdge(Edge* edge);

		/*!
			adds outedge to vertex
			By design it stores the object as smart pointer.
			The edge must be dynamically allocated object  
		*/
		void addOutEdge(Edge* edge); //#TODO refractor this for passing smart pointer?

		//#TODO remove functions do not need Vertex/ID data for operation
		// 

		/*!
			removes inedge that is starts at Vertex specifed by from ID and 
		*/
		void removeInEdge(const std::size_t from, const Edge* edge);
		void removeOutEdge(const std::size_t to, const Edge* edge);

		void removeInEdge(const Vertex* from, const Edge* edge);
		void removeOutEdge(const Vertex* to, const Edge* edge);

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