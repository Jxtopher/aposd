#include "calculationModel/islandModel/topologies/randomEdge.hpp"

RandomEdge::RandomEdge(std::shared_ptr<std::mt19937> mt_rand, unsigned int number_of_nodes, double p)
	: Topologies(number_of_nodes) {
	assert(p <= 1 && p >= 0);
	std::uniform_real_distribution<> urd(0, 1);
	for(unsigned int i = 0; i < this->_number_of_nodes; i++) { add_vertex(this->g); }

	for(unsigned int i = 0; i < this->_number_of_nodes; i++) {
		for(unsigned int j = i + 1; j < this->_number_of_nodes; j++) {
			if(urd(*mt_rand) < p) { add_edge(i, j, EdgeProperties(0), g); }
		}
	}
}

RandomEdge::~RandomEdge() {}