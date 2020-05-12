#ifndef JXTOPHER_STAR_H
#define JXTOPHER_STAR_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>	// print_graph
#include <boost/graph/properties.hpp>

#include "topologies.hpp"

using namespace boost;

class Star : public Topologies {
  public:
	Star(unsigned int number_of_nodes);
	virtual ~Star();
};

#endif