#ifndef JXTOPHER_STAR_H
#define JXTOPHER_STAR_H

#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>

#include "topologies.h"


using namespace boost;

class Star : public Topologies {
	public:
		Star(unsigned int nbNodes) : 
            Topologies(nbNodes) {
            for (unsigned int i = 0 ; i < this->_nbNodes ; i++) {
                add_vertex(this->g);
            }

            for (unsigned int i = 1 ; i < this->_nbNodes ; i++) {
                 add_edge(0, i, EdgeProperties(0), g);
            }
            
            
		}
		virtual ~Star() {
			
		}

	protected:
        
};


#endif