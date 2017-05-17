#include "stdafx.h"
#include "Graph.h"
#include <iostream>
namespace graph {

	Graph::Edge::Edge()
	{
	}


	Graph::Edge::Edge(Vertex* source, Vertex* destination, int cost) :
		source(source), destination(destination), cost(cost), directed(false)
	{

	}

	Graph::Edge::~Edge()
	{
		std::cout << "Edge dtor!" << std::endl;
	}


	int Graph::Edge::getCost()
	{
		return this->cost;
	}

	void Graph::Edge::setCost(int cost)
	{
		this->cost = cost;
	}


	inline std::size_t Graph::Edge::getID()
	{
		return this->id;
	}

	inline Graph::Vertex * Graph::Edge::getSource() const
	{
		return source;
	}

	inline Graph::Vertex * Graph::Edge::getDestination() const
	{
		return destination;
	}




	Graph::Graph()
	{
		std::cout << "Graph ctor!" << std::endl;
	}


	Graph::~Graph()
	{
		std::cout << "Graph dtor!" << std::endl;
	}


	std::size_t Graph::addVertex()
	{
		using pairtype = std::pair<const std::size_t, std::unique_ptr<Vertex>>;
		std::size_t id;
		auto &it_id = std::max_element(Vertices.begin(), Vertices.end(), [](const pairtype &V1, const pairtype &V2) ->bool {return V1.first < V2.first; });
		
		if (it_id != Vertices.end())
			id = it_id->first;
		else
			id = 0;

		id++;
		auto it = Vertices.insert(std::make_pair(id,std::make_unique<Vertex>(id)));
		if (it.second)
		{
			//object was inserted
			//it->second = ; //allocate memory for Vertex return
		}

		return id; //return unique id ,that will identify this object in future
	}

	void Graph::addVertex(std::size_t id)
	{
		auto it = Vertices.insert(std::make_pair(id, std::make_unique<Vertex>(id)));
		if (it.second)
		{
			//object was inserted
			//it->second = ; //allocate memory for Vertex return
		}
		else
		{
			//object already exists!
		}

		 //return unique id ,that will identify this object in future
	}


	void Graph::addEdge(std::size_t from, std::size_t to, int cost, bool directed)
	{
		auto &it_from = Vertices.find(from);
		auto &it_to = Vertices.find(to);
		if (it_from == Vertices.end())
		{
			//Node does not exist
			return;
		}

		if (it_to == Vertices.end())
		{
			//Node does not exist
			return;
		}
		Vertex* fromPtr=nullptr;
		Vertex* toPtr=nullptr;
		
		fromPtr = it_from->second.get();
		toPtr = it_to->second.get();

		if (fromPtr == nullptr)
		{
			return;
		};
		if (toPtr == nullptr)
		{
			return;
		};
		


		//create new edge and move it to container
		//std::unique_ptr<Edge> edge(new Edge(fromPtr, toPtr, cost));


		//#TODO REWORK THIS, try avoiding explicit new (at least without unique_ptr )
		Edge * edge = new Edge(fromPtr, toPtr, cost);
		//fromPtr->addOutEdge(edge.get());
		toPtr->addInEdge(edge);
		fromPtr->addOutEdge(edge);
		if (!directed)
		{
			//add 2nd edge to represent 2 way edge
			edge = new Edge(fromPtr, toPtr, cost);
			fromPtr->addInEdge(edge);
			toPtr->addOutEdge(edge);
		}






	}

	void Graph::removeEdges(std::size_t from, std::size_t to)
	{
	}

	

	void Graph::removeDirEdge(std::size_t from, std::size_t to)
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
			edgePtr = vertexFromPtr->findOutEdge(vertexToPtr);

			it_to->second->removeInEdge(it_from->second.get(), edgePtr); //remove pointer to edge from receiving vertex
			it_from->second->removeOutEdge(it_to->second.get(), edgePtr); //remove the edge itself
		}


	}

	inline void Graph::removeVertex(std::size_t node)
	{
	}


	inline std::size_t Graph::edgeCount()
	{
		std::size_t count = 0;

		/*for (auto  &ent1 : Vertices)
		{
		auto  &key = ent1.first;
		auto  &val = ent1.second;
		count += val.countEdges();
		}
		return count;*/


		for (auto &it : Vertices)
		{

			count += it.second->countEdges();

		}
		return count;

	}

	inline std::string Graph::vertexToString(const std::pair<const std::size_t,std::unique_ptr<Vertex>>& it)
	{
	
		const auto &item = it.second;
		std::string str;
		str += "Name:";
		str += std::to_string(item->getID());
		str += "\tEdges total:";
		str += std::to_string(item->countEdges());

		return str;
	}

	inline std::string Graph::vertexToString(const std::size_t &name )
	{
		std::string str;
		const auto &it= Vertices.find(name);
		if (it != Vertices.end())
		{
			str = vertexToString(*it);
		}
		return str;
	}

	std::string Graph::edgeToString()
	{
		return std::string();
	}

	std::vector<std::string> Graph::verticesToString()
	{
		std::vector<std::string> out;
		out.reserve(24u * Vertices.size());
		for (const auto &it : Vertices)
		{
			out.push_back(vertexToString(it));
		}
		return out;
	}

	std::vector<std::string> Graph::edgesToString()
	{
		return std::vector<std::string>();
	}


	inline std::size_t Graph::vertexCount()
	{
		return Vertices.size();

	}


	Graph::Vertex::Vertex(std::size_t id) :
		uniq_id(id)
	{

	}


	Graph::Vertex::~Vertex()
	{
		std::cout << "Vertex dtor!" << std::endl;

	}


	inline std::size_t Graph::Vertex::getID()
	{
		return uniq_id;
	}



	std::size_t Graph::Vertex::countEdges()
	{
		return outEdges.size();
	}


	inline const graph::Graph::Edge * Graph::Vertex::findOutEdge(const Vertex * to) const
	{
		auto &it = std::find_if(outEdges.begin(), outEdges.end(), [&to](const std::unique_ptr<Edge>& E)->bool {return E->getDestination() == to; });

		if (it != outEdges.end())
		{
			return it->get();
		}

		return nullptr;
	}


	

	

	//#TODO add edge needs only a pointer to edge



	/*!
	returns a pointer to first Edge that arrives from vertex based on specified predicament
	*/

	inline const graph::Graph::Edge * Graph::Vertex::findInEdge(const Vertex * from) const
	{

		const auto &it = std::find_if(inEdges.begin(), inEdges.end(), [&from](const Edge* E)->bool {return E->getDestination() == from; });


		if (it != inEdges.end())
		{
			return *it;
		}

		return nullptr;
	}

	

	inline void Graph::Vertex::addInEdge(Edge * edge)
	{
		if (edge->getDestination() == this)
			this->inEdges.push_back(edge);
	}

	inline void graph::Graph::Vertex::addOutEdge(Edge * edge)
	{
		if (edge->getSource() == this)
		{
			std::unique_ptr<Edge> e;
			e.reset(edge);
			this->outEdges.push_back(std::move(e));
		}
	
			
	}

	inline void Graph::Vertex::removeInEdge(const std::size_t from, const Edge * edge)
	{
		//std::find_if(Vertices.begin(), Vertices.end(), [&from](const std::unique_ptr<Vertex> & v)->bool {return v->getID()==from;});
		//Vertices.find();


	}

	inline void Graph::Vertex::removeOutEdge(const std::size_t to, const  Edge * edge)
	{
	}


	inline void Graph::Vertex::removeInEdge(const Vertex * from, const  Edge * edge)
	{

		inEdges.erase(std::remove(inEdges.begin(), inEdges.end(), edge));
		
	}

	inline void Graph::Vertex::removeOutEdge(const Vertex * to, const Edge * edge)
	{
		auto &it = std::find_if(outEdges.begin(), outEdges.end(), [&edge](const std::unique_ptr<Edge>&e)->bool {return edge == e.get(); });
		if (it != outEdges.end())
			outEdges.erase(it);
		//outEdges.erase(std::remove_if(outEdges.begin(), outEdges.end(), [&edge](const std::unique_ptr<Edge>&e)->bool {return edge == e.get(); }));
	}

}