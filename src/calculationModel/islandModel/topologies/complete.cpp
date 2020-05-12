#include "calculationModel/islandModel/topologies/complete.hpp"

Complete::Complete(unsigned int number_of_nodes) : Topologies(number_of_nodes) {
	for(unsigned int i = 0; i < this->_number_of_nodes; i++) { add_vertex(this->g); }

	for(unsigned int i = 0; i < this->_number_of_nodes; i++) {
		for(unsigned int j = i + 1; j < this->_number_of_nodes; j++) {
			add_edge(i, j, EdgeProperties(0), g);
		}
	}
}
Complete::~Complete() {}