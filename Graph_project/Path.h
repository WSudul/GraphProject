#pragma once

#include <list>

template<typename T>
class Path
{
public:
	Path() :
		two_way(false), tot_cost(0)
	{

	};
	virtual ~Path() {};


	virtual std::size_t getTotalCost()
	{
		return tot_cost;
	};

	virtual bool isTwoWay()
	{
		return two_way;
	};

	virtual void push_front(T& el) {
		path.push_front(el);
	}

	virtual void push_back(T& el) {
		path.push_back(el);

	}

	virtual void pop_back() {
		path.pop_back();
	};

	virtual void pop_front() {
		path.pop_front();
	};

private:



	std::list<T> path;
	bool two_way;
	std::size_t tot_cost;

};