#ifndef JXTOPHER_RANDOM_EDGE_H
#define JXTOPHER_RANDOM_EDGE_H

#include <memory>
#include <random>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/properties.hpp>

#include "topologies.hpp"

using namespace boost;

class RandomEdge : public Topologies {
  public:
	RandomEdge(std::shared_ptr<std::mt19937> mt_rand, unsigned int number_of_nodes, double p = 0.20);
	virtual ~RandomEdge();

  protected:
};

#endif