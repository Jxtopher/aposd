#ifndef JXTOPHER_STAR_H
#define JXTOPHER_STAR_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/properties.hpp>

#include "topologies.hpp"

using namespace boost;

class Star : public Topologies {
  public:
    Star(unsigned int number_of_nodes) : Topologies(number_of_nodes) {
        for(unsigned int i = 0; i < this->_number_of_nodes; i++) { add_vertex(this->g); }

        for(unsigned int i = 1; i < this->_number_of_nodes; i++) {
            add_edge(0, i, EdgeProperties(0), g);
        }
    }
    virtual ~Star() {
    }

  protected:
};

#endif