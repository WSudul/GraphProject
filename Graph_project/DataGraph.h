#pragma once
#include "Graph.h"

//#TODO scrap this
//The idea of derived classes (for Vertices and Edges) that can coexist can work on paper , until the need for getters/setters arrives.
//They only probable solution is 1) have base class that has virtual setters/getters that go via some specialized(?) template function and convert input/output to common type (string?)
// and this might break a lot of things, SUGGESTION: forget pure inheritance. Slap template on Graph AND THEN derive from it (Result-> base datatype but different kinds of nodes)
//2) NEVER TO BE USED -> void* which seems horrible in C++ 

template<typename T>
class DataGraph :
	public graph::Graph
{
public:
	DataGraph();
	virtual ~DataGraph();

private:

	//forward declarations
	
	class DataVertex
	{
	public:

		DataVertex();
		~DataVertex();

		void setData(const T& data)
		{
			this->data = data;
		}

		T& getData()
		{
			return *(this->data);
		}

	private:

		T data;


	
	};
	
	class DataEdge;

	

};


template<typename T>
DataGraph<T>::DataGraph()
{
}

template<typename T>
DataGraph<T>::~DataGraph()
{
}


template<typename T>
DataGraph<T>::DataVertex::DataVertex()
{
}

template<typename T>
DataGraph<T>::DataVertex::~DataVertex()
{
}

