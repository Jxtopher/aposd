#ifndef JXTOPHER_CIRCLE_H
#define JXTOPHER_CIRCLE_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/properties.hpp>

#include "topologies.hpp"

using namespace boost;

class Circle : public Topologies {
  public:
	Circle(unsigned int number_of_nodes);
	virtual ~Circle();
};

#endif