#ifndef JXTOPHER_COMPLETE_H
#define JXTOPHER_COMPLETE_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/properties.hpp>

#include "topologies.hpp"

using namespace boost;

class Complete : public Topologies {
  public:
	Complete(unsigned int number_of_nodes) : Topologies(number_of_nodes) {
		for(unsigned int i = 0; i < this->_number_of_nodes; i++) { add_vertex(this->g); }

		for(unsigned int i = 0; i < this->_number_of_nodes; i++) {
			for(unsigned int j = i + 1; j < this->_number_of_nodes; j++) {
				add_edge(i, j, EdgeProperties(0), g);
			}
		}
	}
	virtual ~Complete() {}

  protected:
};

#endif