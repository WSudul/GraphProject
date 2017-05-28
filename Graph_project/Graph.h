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

Node access -> O(1) //depends upon container, unordered_map offers average  amortized O(1)
Add node	-> O(1) //depends upon container, but O(1) amortized for average case or O(nlogn) without node
Add edge	-> O(1) //requires finding both Nodes O(1) and then adding relevant info O(1) in both of them
Remove edge -> O(~1) //requires finding both Nodes O(1), then performing removal on edge ( O(deg(V) ) in this case), worst case O(2(n+n))=O(n)
Remove node -> O(V+E) //requires iterating over edge containers (2x O(degV)) and removing inedges and outedges from other Vertices (O(1) for finding vertice, O(deg(V_1)) , O(deg(V_2))).

Class representing graph via  2 way adjacency list (Node knows both about edges that point from and to the Node)

*/

namespace graph {

	class Graph
	{
	public:
		Graph();
		virtual ~Graph();


		//forward declaration for iterators
		class GraphIterator;
		class OutEdgeIterator;
		class InEdgeIterator;


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
				//looks for the edge in outEdges (for directed) and then in inEdges (for undirected)
				edgePtr = vertexFromPtr->findOutEdge(vertexToPtr,Pred);

				if (edgePtr != nullptr)
				{
					
					//it_to->second->removeInEdge(edgePtr); //remove pointer to edge from receiving vertex //NEEDS TO CALL removeIn/Out only once!
					it_from->second->removeOutEdge(edgePtr); //remove the edge itself
					

				}
			}


		}


		/*!
			remove single vertex with specified id
		*/
		void removeVertex(std::size_t id);

		/*!
			returns number of vertices stored in graph
		*/
		std::size_t vertexCount();

		/*!
			returns number of edges stored in graph.
			Potential overflow value might occur if total number of edges is greater than size of size_t
		*/
		std::size_t edgeCount();

		
		/*!
			returns string representation of Vertex identified by given name (ID)
		*/
		std::string vertexToString(const std::size_t &name);

		/*!
			returns string representation of Edge #TODO implement
		*/
		std::string edgeToString();

		/*!
			returns string representation of all vertices inside Graph by calling vertexToString() on each Vertex
		*/
		std::vector<std::string> verticesToString();
		std::vector<std::string> edgesToString();


		GraphIterator begin();

		GraphIterator end();

	protected:
		
		//#TODO make Vertex/Edge public?

		//forward declarations

		class Vertex;

		class Edge;

	
	public:


		//#TODO move implementation outside class?
		//semi custom iterators (wrappers)

		class GraphIterator
		{
		public:

			using	value_type = Vertex;
			using	difference_type = ptrdiff_t;
			using	pointer = Vertex*;
			using	reference = Vertex&;
			/*using	const_reference		= const Edge&;*/
			using	iterator_category = std::forward_iterator_tag;



			GraphIterator(std::unordered_map<std::size_t,std::unique_ptr<Vertex>>::iterator& pos) :
				pos_(pos)
			{};

			GraphIterator(const GraphIterator& O) :
				pos_(O.pos_)
			{};

			virtual ~GraphIterator()
			{
				//std::cout << "OutEdgeIterator dtor" << std::endl;
			};

			bool operator==(const GraphIterator& rhs) const { return pos_ == rhs.pos_; }
			bool operator!=(const GraphIterator& rhs) const { return pos_ != rhs.pos_; }
			

			GraphIterator& operator=(const GraphIterator& rhs)
			{
				this->pos_ = rhs.pos_;
			}


			//#TODO rework pre/post incr operators
			GraphIterator& operator ++() { ++pos_; return *this; }
			GraphIterator operator++ (int) { /*make assertions*/ GraphIterator tmp(*this); ++pos_; return tmp; }

			GraphIterator& operator --() { --pos_; return *this; }
			GraphIterator operator-- (int) { /*make assertions*/ GraphIterator tmp(*this); --pos_; return tmp; }

			const reference operator*() {
				return *(pos_->second); //double-dereferencing
			}


			const pointer operator->() {
				return pos_->second.get();
			}

		private:
			std::unordered_map<std::size_t, std::unique_ptr<Vertex>>::iterator pos_;


		};
		
		class OutEdgeIterator
		{
		public:

			//semi custom iterator which is basically a thin wrap for vector iterator
			//boiler plate typedefs (using) provided for future expansion with accordance to C++17

			using	value_type			=  Edge  ;
			using	difference_type		=  ptrdiff_t;
			using	pointer				=  Edge*;
			using	reference			=  Edge& ;
			/*using	const_reference		=  Edge&;*/
			using	iterator_category = std::forward_iterator_tag;
	


			OutEdgeIterator(std::vector<std::unique_ptr<Edge>>::iterator& pos) :
				pos_(pos)
			{};

			OutEdgeIterator(const OutEdgeIterator& O) :
				pos_(O.pos_)
			{};

			virtual ~OutEdgeIterator()
			{
				//std::cout << "OutEdgeIterator dtor" << std::endl;
			};

			bool operator==(const OutEdgeIterator& rhs) const { return pos_ == rhs.pos_; }
			bool operator!=(const OutEdgeIterator& rhs) const { return pos_ != rhs.pos_; }
			bool operator<=(const OutEdgeIterator& rhs) const { return pos_ <= rhs.pos_; }
			bool operator>=(const OutEdgeIterator& rhs) const { return pos_ >= rhs.pos_; }
			bool operator<(const OutEdgeIterator& rhs) const { return pos_ < rhs.pos_; }
			bool operator>(const OutEdgeIterator& rhs) const { return pos_ > rhs.pos_; }

			OutEdgeIterator& operator=(const OutEdgeIterator& rhs)
			{
				this->pos_ = rhs.pos_;
			}


			//#TODO rework pre/post incr operators
			OutEdgeIterator& operator ++() { ++pos_; return *this ; }
			OutEdgeIterator operator++ (int) { /*make assertions*/ OutEdgeIterator tmp(*this); ++pos_; return tmp; }

			OutEdgeIterator& operator --() { --pos_; return *this; }
			OutEdgeIterator operator-- (int) { /*make assertions*/ OutEdgeIterator tmp(*this); --pos_; return tmp; }
			
			reference operator*() {
				return **pos_; //double-dereferencing
			}

			
			pointer operator->() {
				return pos_->get();
			}

		private:
			std::vector<std::unique_ptr<Edge>>::const_iterator pos_;
		};


		class InEdgeIterator
		{
		public:

			//semi custom iterator which is basically a thin wrap for vector iterator
			//boiler plate typedefs (using) provided for future expansion with accordance to C++17

			using	value_type =  Edge;
			using	difference_type = ptrdiff_t;
			using	pointer =   Edge*;
			using	reference =  Edge&;
			/*using	const_reference		= const Edge&;*/
			using	iterator_category = std::forward_iterator_tag;



			InEdgeIterator(std::vector<Edge*>::iterator& pos) :
				pos_(pos)
			{};

			InEdgeIterator(const InEdgeIterator& O) :
				pos_(O.pos_)
			{};

			virtual ~InEdgeIterator()
			{
				//std::cout << "InEdgeIterator dtor" << std::endl;
			};

			bool operator==(const InEdgeIterator& rhs) const { return pos_ == rhs.pos_; }
			bool operator!=(const InEdgeIterator& rhs) const { return pos_ != rhs.pos_; }
			bool operator<=(const InEdgeIterator& rhs) const { return pos_ <= rhs.pos_; }
			bool operator>=(const InEdgeIterator& rhs) const { return pos_ >= rhs.pos_; }
			bool operator<(const InEdgeIterator& rhs) const { return pos_ < rhs.pos_; }
			bool operator>(const InEdgeIterator& rhs) const { return pos_ > rhs.pos_; }
			
			InEdgeIterator& operator=(const InEdgeIterator& rhs)
			{
				this->pos_ = rhs.pos_;
			}
			InEdgeIterator& operator ++() { ++pos_; return *this; }
			InEdgeIterator operator++ (int) { /*make assertions*/ InEdgeIterator tmp(*this); ++pos_ ; return tmp; }
			InEdgeIterator& operator --() { --pos_; return *this; }
			InEdgeIterator operator-- (int) { /*make assertions*/ InEdgeIterator tmp(*this); --pos_; return tmp; }


			reference operator*()  {
				return **pos_; //double-dereferencing
			}

	
			pointer operator->()  {
				return *pos_;
			}

		private:
			std::vector<Edge*>::const_iterator pos_;
		};


		/*!
		returns string representation of single vertice
		*/
		virtual std::string Graph::vertexToString(const std::pair <const std::size_t, std::unique_ptr<Vertex>>& it);

	protected:

		/*
			map of all vertices,key is unique ID assigned to each vertex
			reasoning behind storing smart pointer: Various containers don't guarantee to 
			keep iterators/pointers valid (e.g. vectors) and keeping the design to be based upon a form of ID (for eg, a string or unsigned int)
			that could restrict design (?). With smart pointers, I am storing addresses of dynamically created objects.
			For client's use each Vertex needs to be identified, therefore it's used with unique key value
		*/
		std::unordered_map<std::size_t, std::unique_ptr<Vertex>> Vertices;
		
		

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
		std::size_t getID() const;

		/*!
			returns the  total number of edges that go out of this vertex
		*/
		std::size_t countEdges() const;


		/*!
			returns a const pointer to first Edge, connecting this Vertex and to Vertex,
			that starts at this Vertex and reaches to Vertex or starts at to Vertex and is undirected
		*/
		const graph::Graph::Edge*  findEdgeTo(const Vertex*  to) const;


		/*!
			returns const pointer to first edge , connecting from Vertex and this Vertex,
			that starts at from Vertex or starts at this Vertex and is undirected
		*/
		const graph::Graph::Edge*  findEdgeFrom(const Vertex*  from) const;



		/*!
			Template method for finding  outEdge
			returns const pointer to outedge that starts at from Vertex that meets the predicament
		*/
		template<typename _Pr>
		const graph::Graph::Edge*  findOutEdge(const Vertex*  to, _Pr& Pred) const
		{
			auto &it = std::find_if(outEdges.begin(), outEdges.end(),
				[&to, &Pred](const std::unique_ptr<Edge>& E)->bool {return E->getDestination() == to && Pred(E); });

			if (it != outEdges.end())
			{
				return it->get();
			}


			return nullptr;
		}


		/*!
			Template method for finding  outEdge
			returns const pointer to outedge that starts at from Vertex that meets the predicament
			Pred1 compares std::unique_ptr<Edge> with defined predicament.
			Pred2 looks specifically at the cost of Edge and thus compares Edge's cost to Pred1.  Default Pred2 always yields true
			Returns const pointer to outedge that starts at from Vertex that meets the predicaments
		*/
		template<typename _Pr1, typename _Pr2>
		const graph::Graph::Edge*  findOutEdge(const Vertex*  to, _Pr1& Pred1, _Pr2& Pred2 = [] {return true; }) const
		{
			auto &it = std::find_if(outEdges.begin(), outEdges.end(),
				[&to, &Pred1, &Pred2](const std::unique_ptr<Edge>& E)->bool {return E->getDestination() == to  && Pred1(E) && Pred2(E->getCost); });

			if (it != outEdges.end())
				return it->get();

			return nullptr;
		}


		/*!
			returns a pointer to first Edge that arrives from vertex based on specified predicament
		*/
		template<typename _Pr>
		const graph::Graph::Edge*  findInEdge(const Vertex*  from, _Pr& Pred) const
		{

			const auto &it = std::find_if(inEdges.begin(), inEdges.end(),
				[&from, &Pred](const Edge* E)->bool {return E->getSource() == from && Pred(E); });

			if (it != inEdges.end())
				return *it;

			return nullptr;
		}

		/*!
			Template method for finding  outEdge
			returns const pointer to outedge that starts at from Vertex that meets the predicament
			Pred1 compares Edge* with defined predicament.
			Pred1 looks specifically at the cost of Edge and thus compares Edge's cost to Pred2.  Default Pred2 always yields true
			Returns const pointer to outedge that starts at from Vertex that meets the predicaments
		*/
		template<typename _Pr1, typename _Pr2>
		const graph::Graph::Edge*  findInEdge(const Vertex*  to, _Pr1& Pred1, _Pr2& Pred2 = [] {return true; }) const
		{
			auto &it = std::find_if(inEdges.begin(), inEdges.end(),
				[&to, &Pred1, &Pred2](const Edge* E)->bool {return E->getSource() == to && Pred1(E) && Pred2(E->getCost) ; });

			if (it != outEdges.end())
				return it->get();

			return nullptr;
		}


		const graph::Graph::Edge* findOutEdge(const Vertex * to) const;


		const graph::Graph::Edge* findInEdge(const Vertex * from) const;

		



		virtual void setData() {};
		virtual std::size_t getData() { return std::size_t(); }


		/*!
			adds inedge to vertex
		*/
		void addInEdge(std::unique_ptr<Edge> edge);

		/*!
			adds outedge to vertex
			By design it stores the object as smart pointer.
			The edge must be dynamically allocated object  
		*/
		void addOutEdge(std::unique_ptr<Edge> edge); //#TODO refractor this for passing smart pointer?

	
		/*!
			removes inedge that belongs to this Vertex'es inEdges and from Vertex that owns the edge itself (the Source Vertex) 
		*/
		void removeInEdge(const Edge* edge);

		/*!
			removes outedge that belongs to this Vertex'es outEdges and from Vertex that was destination of the edge (the Destination Vertex)
		*/
		void removeOutEdge( const Edge* edge);


		/*!
		
			Returns an iterator pointing to the first element of outEdges
		*/
		OutEdgeIterator begin_outEdge();

		/*!

			Returns an iterator pointing to the past the last element of outEdges
		*/
		OutEdgeIterator end_outEdge();




		/*!

			Returns an iterator pointing to the first element of inEdges
		*/
		InEdgeIterator begin_inEdge();;

		/*!

			Returns an iterator pointing to the past the last element of inEdges
		*/
		InEdgeIterator end_inEdge();


		/*!
			Returns an iterator pointing to the first element of edges starting from this Vertex.
		*/
		OutEdgeIterator begin();

		/*!
			Returns an iterator pointing to the past the last element of edges starting from this Vertex.
		*/
		OutEdgeIterator end();

	protected:

		std::size_t uniq_id; //unique ID #TODO enforcing uniquness and reclamation of unused IDs without 



		std::vector<Edge*> inEdges; //container of all edges that are pointing to this node
		
		std::vector<std::unique_ptr<Edge>> outEdges;  //container of all edges that are pointing from this node


		/*!
			removes outEdge edge from this Vertex. Destroys the object itself
		*/
		void removeOutEdgeOnly(const Edge* edge);
		
		/*!
			removes inEdge edge from this Vertex. Removes only pointer from container to the object
		*/
		void removeInEdgeOnly(const Edge* edge);


		void addInEdgeOnly(Edge* edge);

		void addOutEdgeOnly(std::unique_ptr<Edge> edge);


	};


	class Graph::Edge
	{
	public:

		Edge();
		Edge(Vertex* source, Vertex* destination, int cost, bool directed=true);
		virtual ~Edge();
		//Edge(std::size_t destination, int cost = 1);

		bool operator==(const Edge& rhs) const
		{
			return (this->cost == rhs.cost && this->source == rhs.source && this->destination == rhs.source);
		}

		//#TODO move definition to header file (fixing unresolved ext symbol for inlined functions when callig in main() )

		short getCost() const;
		void setCost(int cost);

		std::size_t getID();


		Vertex* getSource() const;
		Vertex* getDestination() const;

		bool isDirected() const;

		void setDirected(bool dir);



	protected:
		int cost;
		Vertex* source;
		Vertex* destination;
		bool directed;
		std::size_t id;
	};



}