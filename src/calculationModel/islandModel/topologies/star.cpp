#include "calculationModel/islandModel/topologies/star.hpp"

Star::Star(unsigned int number_of_nodes) : Topologies(number_of_nodes) {
	for(unsigned int i = 0; i < this->_number_of_nodes; i++) { add_vertex(this->g); }

	for(unsigned int i = 1; i < this->_number_of_nodes; i++) {
		add_edge(0, i, EdgeProperties(0), g);
	}
}

Star::~Star() {}