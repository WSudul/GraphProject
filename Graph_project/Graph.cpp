#include "stdafx.h"
#include "Graph.h"

namespace graph {

	Graph::Edge::Edge()
	{
	}


	Graph::Edge::Edge(Vertex* source, Vertex* destination, int cost, bool directed) :
		source(source), destination(destination), cost(cost), directed(directed)
	{

	}

	Graph::Edge::~Edge()
	{
		//std::cout << "Edge dtor!" << std::endl;
	}


	short Graph::Edge::getCost() const
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

	inline bool Graph::Edge::isDirected() const
	{
		return directed;
	}

	inline void Graph::Edge::setDirected(bool dir)
	{
		this->directed = dir;
	}




	Graph::Graph()
	{
		//std::cout << "Graph ctor!" << std::endl;
	}


	Graph::~Graph()
	{
		//std::cout << "Graph dtor!" << std::endl;
	}


	std::size_t Graph::addVertex()
	{
		using pairtype = std::pair<const std::size_t, std::unique_ptr<Vertex>>;
		std::size_t id;
		auto &it_id = std::max_element(Vertices.begin(), Vertices.end(), [](const pairtype &V1, const pairtype &V2) ->bool {return V1.first < V2.first; });

		if (it_id != Vertices.end())
		{
			id = it_id->first;
			id++; 
		}
		else
			id = 0; //when container was empty

		
		
		addVertex(id);

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
		if (it_from == Vertices.end() || it_to==Vertices.end())
		{
			//missing Vertex/Vertices
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
		



		//#TODO REWORK THIS, try avoiding explicit new (at least without unique_ptr ) - create factory for various edges?
		Edge * edge = new Edge(fromPtr, toPtr, cost,directed);
		
		//OutEdge is a sole owner of the edge. InEdge will store pointer
		//Undirected edges can be found by iterating over inEdges container.

		toPtr->addInEdge(edge);
		fromPtr->addOutEdge(edge); //this will keep the ownership of edge
		if (!directed)
		{
			//add pointer to edge to  inEdges that is marked as undirected 
								//edge = new Edge(fromPtr, toPtr, cost,directed);
			//fromPtr->addInEdge(edge);
			//toPtr->addOutEdge(edge);
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
			if (edgePtr != nullptr) 
			{
				it_to->second->removeInEdge(edgePtr); //remove pointer to edge from receiving vertex
				it_from->second->removeOutEdge(edgePtr); //remove the edge itself
			}
		}


	}

	void Graph::removeVertex(std::size_t id)
	{
		//find vertex
		auto it=Vertices.find(id);

		if (it != Vertices.end())
		{
			//remove the vertex

			auto& val = it->second;
			
			//iterate over all outEdges and remove pointers from inEdges container of destination vertices
			auto edge_it = val->begin_outEdge();// val->begin_outEdge();
			

			
			//std::cout << "start looping"; //start looping
			for (; edge_it != val->end_outEdge();++edge_it)
			{
				//get destination ID and find the destination vertex
				auto dest = edge_it->getDestination()->getID();
				auto dest_it = Vertices.find(dest);


				if (dest_it != Vertices.end())
				{
					const auto *temp = &*(edge_it);
					//remove pointer to inEdge from destination vertex
					dest_it->second->removeInEdge(temp);

					dest_it->second->countEdges();
				}
				else
				{
					//throw exception
				}
			}


			//repeat procedure for inEdges
			for (auto edge_it = val->begin_inEdge(); edge_it != val->end_inEdge(); ++edge_it)
			{
				//get source ID and find the source vertex
				
				auto source = (edge_it)->getSource()->getID();
				auto &source_it = Vertices.find(source);


				if (source_it != Vertices.end())
				{
					const auto *temp = &*(edge_it);
					//remove outEdge from source vertex
					source_it->second->removeOutEdge(temp);

					source_it->second->countEdges();
				}
				else
				{
					//throw exception
				}
			}



			//erase the vertex from container
			Vertices.erase(id);

		}


	}


	std::size_t Graph::edgeCount()
	{
		std::size_t count = 0;

		for (auto &it : Vertices)
			count += it.second->countEdges();

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

	Graph::GraphIterator Graph::begin()
	{
		return *(std::move(std::unique_ptr<GraphIterator>(new GraphIterator(Vertices.begin()))));
	}

	Graph::GraphIterator Graph::end()
	{
		return *(std::move(std::unique_ptr<GraphIterator>(new GraphIterator(Vertices.end()))));
	}

	std::size_t Graph::vertexCount()
	{
		return Vertices.size();

	}


	Graph::Vertex::Vertex(std::size_t id) :
		uniq_id(id)
	{

	}


	Graph::Vertex::~Vertex()
	{
		//std::cout << "Vertex dtor!" << std::endl;

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
		else
		{
			//search inEdges for undirected connection 
			auto &it2 = std::find_if(inEdges.begin(), inEdges.end(), 
				[&to](const Edge* E)->bool {return  !(E->isDirected() ) && E->getSource() == to; }); //looks for UNDIRECTED edge that has Source same as Vertex to.

		}

		return nullptr;
	}




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

	inline void Graph::Vertex::addOutEdge(Edge * edge)
	{
		if (edge->getSource() == this)
		{
			this->outEdges.push_back(std::move(std::unique_ptr<Edge>(edge)));
		}
	
			
	}

	
	
	inline void Graph::Vertex::removeInEdge(const  Edge * edge)
	{


		//remove-erase idiom on vector container
		inEdges.erase(std::remove(inEdges.begin(), inEdges.end(), edge));
				
		return;
	}

	inline void Graph::Vertex::removeOutEdge( const Edge * edge)
	{ 

		//remove-erase idiom behaves wonkly. Exception thrown when edge was NOT in container (reason: probably giving wrong pointer (inEdge pointer)

		auto it=(std::find_if(outEdges.begin(), outEdges.end(), [&edge](const std::unique_ptr<Edge>&e)->bool {return edge == e.get(); }));
		
		
		if(it!=outEdges.end())
			outEdges.erase(it);
		
		return;
		//outEdges.erase(std::remove_if(outEdges.begin(), outEdges.end(), [&edge](const std::unique_ptr<Edge>&e)->bool {return edge == e.get(); }));
	}

}