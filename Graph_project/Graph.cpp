#include "stdafx.h"
#include "Graph.h"
#include <iostream>
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

	const std::size_t Graph::Edge::getID()
	{
		return this->id;
	}
	const std::size_t Graph::Edge::getID() const
	{
		return this->id;
	}

	Graph::Vertex * Graph::Edge::getSource()
	{
		return source;
	}

	Graph::Vertex * Graph::Edge::getDestination()
	{
		return destination;
	}

	const graph::Graph::Vertex * Graph::Edge::getSource() const
	{
		return source;
	}

	const graph::Graph::Vertex * Graph::Edge::getDestination() const
	{
		return destination;
	}

	bool Graph::Edge::isDirected() const
	{
		return directed;
	}

	void Graph::Edge::setDirected(bool dir)
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
		//#TODO rework to add the first min value that does not exist in Vertices (max_element is already O(n) )
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


	}


	void Graph::addEdge(std::size_t from, std::size_t to, int cost, bool directed)
	{
		auto &it_from = Vertices.find(from);
		auto &it_to = Vertices.find(to);
		if (it_from == Vertices.end() || it_to == Vertices.end())
		{
			//missing Vertex/Vertices
			return;

		}
		Vertex* fromPtr = nullptr;
		Vertex* toPtr = nullptr;

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
		std::unique_ptr<Edge> edge = std::make_unique<Edge>(fromPtr, toPtr, cost, directed);

		//OutEdge is a sole owner of the edge. InEdge will store pointer
		//Undirected edges can be found by iterating over inEdges container.

		//needs to be called only once!
		fromPtr->addOutEdge(std::move(edge));






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
				//#TODO explicit const_cast -> removeOutEdge
				it_from->second->removeOutEdge(const_cast<Edge*>(edgePtr)); //remove the edge
			}
		}


	}

	void Graph::removeVertex(std::size_t id)
	{
		//find vertex
		auto it = Vertices.find(id);

		if (it != Vertices.end())
		{
			//remove the vertex
			auto& val = it->second;
			
			
			OutEdgeIterator edge_it = val->begin_outEdge();
			Vertex* dest=nullptr;
			
			//iterate over all outEdges and remove edges from Graph
			//for (auto edge_it = val->begin_outEdge(); edge_it != val->end_outEdge();)
			while(val->begin_outEdge()!= val->end_outEdge())
			{

				edge_it =std::prev(val->end_outEdge()); //get last element in container //can be begin_outEdge() but now assuming it's faster to pop last element

				//get pointer to destination 
				dest = edge_it->getDestination();
				
				//optional (?) check if dest belongs to this->Vertices
				auto dest_id = dest->getID();
				auto it = Vertices.find(id);
				if (it == Vertices.end())
				{
					//#TODO throw exception (?)
				}

				//check if destination is pointing to valid object
				if (dest != nullptr)
				{
					auto *temp = &*(edge_it);
					//remove  from pointer from destination vertex and then object from source vertex
					dest->removeInEdge(temp);


				}
				else
				{
					//throw exception
				}
			}


			//repeat procedure for inEdges
			//for (auto edge_it = val->begin_inEdge(); edge_it != val->end_inEdge();)

			InEdgeIterator edge_it2 = val->begin_inEdge();
			Vertex* src= nullptr;

			while (val->begin_inEdge() != val->end_inEdge())
			{
				edge_it2 = val->begin_inEdge();
				

				src = (edge_it2)->getSource(); //get source ID and find the source vertex
				

				//optional (?) check if source belongs to this->Vertices
				const auto src_id = src->getID();
				auto it = Vertices.find(id);
				if (it == Vertices.end())
				{
					//#TODO throw exception (?)
				}


				if (src != nullptr)
				{
					auto *temp = &*(edge_it2);
					//remove outEdge from source vertex and pointer from destiantion vertex (this)
					src->removeOutEdge(temp);

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

	inline std::string Graph::vertexToString(const std::pair<const std::size_t, std::unique_ptr<Vertex>>& it)
	{

		const auto &item = it.second;
		std::string str;
		str += "Name:";
		str += std::to_string(item->getID());
		str += "\tEdges total:";
		str += std::to_string(item->countEdges())+" ";
		str += item->edgestToString();
		return str;
	}

	bool Graph::traversable(std::size_t vert1, std::size_t vert2)
	{
		auto& v1_it = Vertices.find(vert1);
		auto& v2_it = Vertices.find(vert2);

		//check if vertices exists at all
		if (v1_it == Vertices.end() && v2_it == Vertices.end())
			return false;

		//get handles to Vertices
		auto* v1 = v1_it->second.get();
		auto* v2 = v2_it->second.get();

		//call DFS method #TODO implement DFS

		std::vector<std::size_t> pathDFS = DFS(v1, v2);

		if (pathDFS.empty())
			return false;

		return true;


	}

	std::vector<std::size_t> Graph::DFS(const Vertex * v1, const Vertex * v2)
	{

		//#TODO assert validity of v1,v2 ->not nullptr , belong to graph (find)

		//assuming a "large" graph #TODO case for small graphs !
		std::unordered_set<const Vertex*> visited; //stores all visited Vertices (identified by pointer address)
		std::stack<const Vertex*> stack;
		
		const Vertex* currVertex =v1;
		//Bstack.push(currVertex); //push starting vertex to stack
		visited.insert(currVertex);
		stack.push(currVertex);
		bool found_not_visited = false;

		while (currVertex != v2) {
			
			found_not_visited = false;
			
			for (auto const &edge : *currVertex) 
			{
				const Vertex* dest = edge.getDestination();
				auto p = visited.insert(dest);

				if (p.second) {
					//dest Vertex not present,gets marked as visited
					currVertex = dest; //explore new node
					stack.push(currVertex); //add new node to path

					
					found_not_visited = true;
					break; //break for loop #TODO rework tihs
				};

			}

			//loop through inEdges in search of undirected edges!
			if (!found_not_visited)
			{
				for (auto it = currVertex->begin_inEdge(); it != currVertex->end_inEdge(); ++it)
				{
					if (!it->isDirected()) {
						const Vertex* source = it->getSource();

						auto p = visited.insert(source);

						if (p.second) {
							//dest Vertex not present,gets marked as visited
							currVertex = source; //explore new node
							
							stack.push(currVertex); //add new node to path

							
							found_not_visited = true;
							break; //break for loop #TODO rework tihs
						};
					}
						
				}
			}

			if (!found_not_visited)
				stack.pop(); //pop edge and try with previous node


			if (stack.empty())
				return std::vector<std::size_t>();

			if (!found_not_visited)
				currVertex = stack.top(); //get top of stack if no new vertex was found

		};

		
		//path now contains solution-> v1, v_n,...v_m, v2
		//reverse it and you have v1->v2 (v1->v2 != v2->v1 !!)
		//or iterate backward from end to begin
		std::vector<std::size_t> path_vec;
		path_vec.reserve(stack.size());

		while (stack.size()){
			path_vec.push_back(stack.top()->getID());
			stack.pop();
		};

		std::reverse(path_vec.begin(),path_vec.end());

		return path_vec;
	}


	std::vector<std::size_t> Graph::BFS(const Vertex * v1, const Vertex * v2)
	{
		//#TODO assert validity of v1,v2 ->not nullptr , belong to graph (find)

		//assuming a "large" graph #TODO case for small graphs !
		std::unordered_set<const Vertex*> visited; //stores all visited Vertices (identified by pointer address)
		std::queue<const Vertex*> queue;

		const Vertex* currVertex = v1;
	
		visited.insert(currVertex);
		queue.push(currVertex);
		bool found_solution = false;


		while (currVertex != v2) {

			

			//for (auto const  &edge : *currVertex){
			//	const Vertex* dest = edge.getDestination(); //somethings is wrong with getDestination -> getSource() compiles fine
			//	//const Vertex* dest =edge->getDestination();
			//	auto p = visited.insert(dest);
			//	const Vertex* temp = dest;
			//	if (p.second) {
			//		//dest Vertex not present,gets marked as visited
			//		queue.push(std::move(temp));
			//		
			//		//check if added Vertex is solution
			//		if (dest == v2) {
			//			found_solution = true;
			//			break;
			//		}

			//	};

			//};

			//loop through inEdges in search of solutuon
			if (!found_solution) {
				for (auto it = currVertex->begin_inEdge(); it != currVertex->end_inEdge(); ++it) {
					//loop only through undirected set
					if (!(it->isDirected())) {
						const Vertex* source = it->getSource();

						auto p = visited.insert(source);

						if (p.second) {
							//source Vertex not present,gets marked as visited
							queue.push(source); //add new node to path

							//perform check if the added Vertex is the solution 
							if (source == v2)
								break;
						}
					}

				}
			}


			if (queue.empty())
				return std::vector<std::size_t>();

			//explore new Vertex
			currVertex = queue.front();
			queue.pop();

			
		}
		//start back-tracking from v2 to v1 by checking adjacent Vertices and comparing them via visited set
		std::vector<const Vertex*> path_backtrack = backtrack(v2, v1, visited);


		//prepare vector containing IDs with results
		std::vector<std::size_t> path_vec;
		path_vec.reserve(path_backtrack.size());


		//fill
		for (auto it : path_backtrack)
			path_vec.push_back(it->getID());

		//reverse
		std::reverse(path_vec.begin(), path_vec.end());

		return path_vec;

	}

	std::vector<std::size_t> Graph::DFS(const std::size_t v1, const std::size_t v2)
	{
		auto v1_it = Vertices.find(v1);
		auto v2_it = Vertices.find(v2);

		if (v1_it == Vertices.end() && v2_it == Vertices.end()) //check if vertices exist
			return std::vector<std::size_t>();

		return DFS(v1_it->second.get(), v2_it->second.get());
	}

	std::vector<std::size_t> Graph::BFS(const std::size_t v1, const std::size_t v2)
	{
		auto v1_it = Vertices.find(v1);
		auto v2_it = Vertices.find(v2);

		if (v1_it == Vertices.end() && v2_it == Vertices.end()) //check if vertices exist
			return std::vector<std::size_t>();

		return BFS(v1_it->second.get(), v2_it->second.get());
	}

	std::vector<const Graph::Vertex*> Graph::backtrack(const Vertex * start, const Vertex * target, const std::unordered_set<const Vertex*>& visited)
	{
		const Vertex* backtrackVertex = start;
		bool found_visited = false;
		std::vector<const Vertex*> path_backtrack;

		path_backtrack.push_back(start);

		while (backtrackVertex != target) {
			found_visited = false;
			//loop through inEdges in search of starting vertex

			for (auto it = backtrackVertex->begin_inEdge(); it != backtrackVertex->end_inEdge(); ++it) {
				if (!it->isDirected()) {
					const Vertex* source = it->getSource();

					auto p = visited.find(source);

					if (p != visited.end()) {
						backtrackVertex = source; //explore different node
						path_backtrack.push_back(backtrackVertex); //add new node to path
						found_visited = true;
						break;
					};
				}
			}


			//search outedges
			if (!found_visited) {
				for (auto edge : *backtrackVertex) {
					const Vertex* dest = edge.getDestination();
					auto p = visited.find(dest);

					if (p != visited.end()) {
						//dest Vertex not present,gets marked as visited
						backtrackVertex = dest; //explore new node
						path_backtrack.push_back(backtrackVertex); //add new node to path
						found_visited = true;
						break;
					};

				}

			}

			if (!found_visited) {
				path_backtrack.pop_back(); //get the oldest element of queue and start exploring it
				backtrackVertex = path_backtrack.back();
			}
		};

		return path_backtrack;
	}

	inline std::string Graph::vertexToString(const std::size_t &name)
	{
		std::string str;
		const auto &it = Vertices.find(name);
		if (it != Vertices.end())
			str = vertexToString(*it);

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
	}


	inline std::size_t Graph::Vertex::getID() const
	{
		return uniq_id;
	}



	std::size_t Graph::Vertex::countEdges() const
	{
		//count number of outEdges (directed and undirected)
		return (outEdges.size());
		//+ std::count_if(inEdges.begin(), inEdges.end(), [](const Edge* e)-> bool {return e->isDirected(); }));
	}


	const graph::Graph::Edge* Graph::Vertex::findEdgeTo(const Vertex * to) const
	{
		const Edge* edge_ptr = findOutEdge(to);

		if (edge_ptr == nullptr)
			edge_ptr = findInEdge(to, [](const Edge * e)->bool { return !(e->isDirected()); });
		
		return edge_ptr;

	}


	const graph::Graph::Edge* Graph::Vertex::findEdgeFrom(const Vertex * from) const
	{

		const Edge* edge_ptr = findInEdge(from);
		if (edge_ptr == nullptr)
			edge_ptr = findOutEdge(from, [](const std::unique_ptr<Edge>& e)->bool { return !(e->isDirected()); });
		

		return edge_ptr;
	}


	const graph::Graph::Edge * Graph::Vertex::findInEdge(const std::size_t id) const
	{

		auto it = std::find_if(inEdges.begin(), inEdges.end(),
			[&id](const Edge* E)->bool {auto* v = E->getSource(); return v->getID() == id; });

		if (it != inEdges.end())
			return *it;
		else
			return nullptr;


	}

	inline void Graph::Vertex::addInEdge(std::unique_ptr<Edge> edge)
	{
		if (edge->getDestination() == this)
		{
			auto src = edge->getSource();
			if (src != nullptr)
			{
				this->addInEdgeOnly(edge.get());

				src->addOutEdgeOnly(std::move(edge));


			}

		}
	}

	void Graph::Vertex::addOutEdge(std::unique_ptr<Edge> edge)
	{
		if (edge->getSource() == this)
		{

			auto dest = edge->getDestination();
			if (dest != nullptr)
			{
				dest->addInEdgeOnly(edge.get());
				this->addOutEdgeOnly(std::move(edge));
			}
		}
	}



	void Graph::Vertex::removeInEdge( Edge * edge)
	{
		if (edge != nullptr)
		{
			removeInEdgeOnly(edge);

			edge->getSource()->removeOutEdgeOnly(edge);
			//src->removeOutEdgeOnly(edge);
		}

		return;
	}

	inline void Graph::Vertex::removeOutEdge( Edge * edge)
	{

		if (edge != nullptr)
		{
			//const auto dest = edge->getDestination();

			edge->getDestination()->removeInEdgeOnly(edge);
			//remove inEdge in destination vertex
			//dest->removeInEdgeOnly(edge);

			//remove the object itself
			removeOutEdgeOnly(edge);
		}

	}


	Graph::OutEdgeIterator Graph::Vertex::begin_outEdge()
	{
		return OutEdgeIterator(outEdges.begin());
	}


	Graph::OutEdgeIterator Graph::Vertex::end_outEdge()
	{

		return OutEdgeIterator(outEdges.end());
	}


	inline Graph::InEdgeIterator Graph::Vertex::begin_inEdge()
	{
		return InEdgeIterator(inEdges.begin());
	}


	Graph::InEdgeIterator Graph::Vertex::end_inEdge()
	{
		return InEdgeIterator(inEdges.end());
	}

	Graph::const_InEdgeIterator Graph::Vertex::begin_inEdge() const
	{
		return const_InEdgeIterator(inEdges.begin());
	}

	Graph::const_InEdgeIterator Graph::Vertex::end_inEdge() const
	{
		return const_InEdgeIterator(inEdges.end());
	}

	Graph::const_OutEdgeIterator Graph::Vertex::begin_outEdge() const
	{
		return const_OutEdgeIterator(outEdges.begin());
	}


	Graph::const_OutEdgeIterator Graph::Vertex::end_outEdge() const
	{
		return const_OutEdgeIterator(outEdges.end());
	}

	Graph::OutEdgeIterator Graph::Vertex::begin()
	{
		return begin_outEdge();
	}

	Graph::const_OutEdgeIterator Graph::Vertex::begin() const
	{
		return begin_outEdge();
	}


	Graph::OutEdgeIterator Graph::Vertex::end()
	{
		return end_outEdge();
	}

	Graph::const_OutEdgeIterator Graph::Vertex::end() const
	{
		return end_outEdge();
	}

	std::string Graph::Vertex::edgestToString()
	{
		std::string str;
		const Vertex* s;
		const Vertex* d;
		for (auto const &edge : outEdges)
		{
			s = edge->getSource();
			d = edge->getDestination();
			str += std::to_string(s->getID())+ "-" + std::to_string(d->getID())+"\n";
		}
		
		return str;
	}

	void Graph::Vertex::removeOutEdgeOnly(const Edge * edge) 
	{

		//remove-erase idiom behaves wonkly. Exception thrown when edge was NOT in container (reason: probably giving wrong pointer (inEdge pointer)

		

		/*auto it = (std::find_if(outEdges.begin(), outEdges.end(),
			[&non_const_edge](const std::unique_ptr<Edge>&e)->bool {return non_const_edge == e.get(); }));*/


		//erase-remove_if idiom ; removes ALL elements that have same edge (by design it should always be 1 !) #TODO review this in future
		outEdges.erase(std::remove_if(outEdges.begin(), outEdges.end(), 
			[&edge](const std::unique_ptr<Edge>&e)->bool {return edge == e.get(); })
			, outEdges.end());

		//if (it != outEdges.end())
		//{
		//	//erase outgoing edge (erase the object itself).
		//	
		//	outEdges.erase(it);
		//}

	}

	void Graph::Vertex::removeInEdgeOnly(const Edge * edge)
	{

		//remove-erase idiom on vector container
		inEdges.erase(std::remove(inEdges.begin(), inEdges.end(), edge));
	}

	inline void Graph::Vertex::addInEdgeOnly(Edge * edge)
	{
		if (edge->getDestination() == this)
			this->inEdges.push_back(edge);
	}

	inline void Graph::Vertex::addOutEdgeOnly(std::unique_ptr<Edge> edge)
	{
		if (edge->getSource() == this)
		{
			this->outEdges.push_back(std::move(edge));
		}
	}

	inline const graph::Graph::Edge * Graph::Vertex::findOutEdge(const Vertex * to) const
	{
		auto it = std::find_if(outEdges.begin(), outEdges.end(),
			[&to](const std::unique_ptr<Edge>& E)->bool {return E->getDestination() == to; });

		if (it != outEdges.end())
			return it->get();
		else
			return nullptr;
	}

	inline const graph::Graph::Edge * Graph::Vertex::findInEdge(const Vertex * from) const
	{

		const auto &it = std::find_if(inEdges.begin(), inEdges.end(),
			[&from](const Edge* E)->bool {return E->getSource() == from; });


		if (it != inEdges.end())
			return *it;
		else
			return nullptr;
	}

	const graph::Graph::Edge * Graph::Vertex::findOutEdge(const std::size_t id) const
	{
		auto it = std::find_if(outEdges.begin(), outEdges.end(),
			[&id](const std::unique_ptr<Edge>& E)->bool {auto* v = E->getDestination(); return v->getID() == id; });

		if (it != outEdges.end())
			return it->get();
		else
			return nullptr;

	}

}