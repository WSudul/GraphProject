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
			Template method.
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

			//auto &it_edge = std::find_if(Edges.begin(), Edges.end(), [&it_from, &it_to](const std::unique_ptr<Edge> &e)->bool {return (it_from->second.get() == e->getSource() && it_to->second.get() == e->getDestination()); });



			//check if both ends are existing
			if (it_from != it_end && it_to != it_end)
			{
				//finds matching edge (O(deg(V)))
				const Edge*  edgePtr = nullptr;
				Vertex*  vertexFromPtr = it_from->second.get();
				Vertex*  vertexToPtr = it_to->second.get();
				edgePtr = vertexFromPtr->findOutEdge(vertexToPtr,Pred);

				it_to->second->removeInEdge(it_from->second.get(), edgePtr); //remove pointer to edge from receiving vertex
				it_from->second->removeOutEdge(it_to->second.get(), edgePtr); //remove the edge itself
			}


		}



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

		std::size_t countEdges();

	private:
		/*!
			returns a pointer to first Edge that reaches to vertex based on specified predicament
		*/
		template<typename _Pr>
		const graph::Graph::Edge*  findOutEdge(const Vertex*  to,_Pr& Pred) const
		{
			auto &it = std::find_if(outEdges.begin(), outEdges.end(), [&to](const std::unique_ptr<Edge>& E)->bool {return E->getDestination() == to; });

			if (it != outEdges.end())
			{
				return it->get();
			}

			return nullptr;
		}


		/*!
			returns a pointer to first Edge that arrives from vertex based on specified predicament
		*/
		template<typename _Pr>
		const graph::Graph::Edge*  findInEdge(const Vertex*  from,_Pr& Pred) const
		{

			const auto &it = std::find_if(inEdges.begin(), inEdges.end(), [&from](const Edge* E)->bool {return E->getDestination() == from; });


			if (it != inEdges.end())
			{
				return *it;
			}

			return nullptr;
		}

		void addInEdge(Edge* edge);
		void addOutEdge(Edge* edge);


		void removeInEdge(const std::size_t to, const Edge* edge);
		void removeOutEdge(const std::size_t from, const Edge* edge);

		void removeInEdge(const Vertex* to, const Edge* edge);
		void removeOutEdge(const Vertex* from, const Edge* edge);



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