#ifndef JXTOPHER_COMPLETE_H
#define JXTOPHER_COMPLETE_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/properties.hpp>

#include "topologies.hpp"

using namespace boost;

class Complete : public Topologies {
  public:
	Complete(unsigned int number_of_nodes);
	virtual ~Complete();

  protected:
};

#endif