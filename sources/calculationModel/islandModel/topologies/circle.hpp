#ifndef JXTOPHER_CIRCLE_H
#define JXTOPHER_CIRCLE_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/properties.hpp>

#include "topologies.hpp"

using namespace boost;

class Circle : public Topologies {
  public:
	Circle(unsigned int number_of_nodes) : Topologies(number_of_nodes) {
		for(unsigned int i = 0; i < this->_number_of_nodes; i++) {
			add_vertex(this->g);
			if(1 <= i) { add_edge(i - 1, i, EdgeProperties(0), g); }
		}
		if(1 < number_of_nodes) add_edge(number_of_nodes - 1, 0, EdgeProperties(0), g);
	}
	virtual ~Circle() {}

  protected:
};

#endif