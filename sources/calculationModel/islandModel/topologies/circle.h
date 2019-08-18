#ifndef JXTOPHER_CIRCLE_H
#define JXTOPHER_CIRCLE_H

#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>

#include "topologies.h"


using namespace boost;

class Circle : public Topologies {
	public:
		Circle(unsigned int nbNodes) : 
            Topologies(nbNodes) {
            for (unsigned int i = 0 ; i < this->_nbNodes ; i++) {
                add_vertex(this->g);
                if (1 <= i) {
                    add_edge(i-1, i, EdgeProperties(0), g);
                }
            }
            if (1 < nbNodes)
                add_edge(nbNodes - 1, 0, EdgeProperties(0), g);
            
		}
		virtual ~Circle() {
			
		}

	protected:
        
};


#endif