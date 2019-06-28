#ifndef JXTOPHER_COMPLETE_H
#define JXTOPHER_COMPLETE_H

#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>

#include "topologies.h"

using namespace std;
using namespace boost;

class Complete : public Topologies {
	public:
		Complete(unsigned int nbNodes) : 
            Topologies(nbNodes) {
            for (unsigned int i = 0 ; i < this->_nbNodes ; i++) {
                add_vertex(this->g);
            }

            for (unsigned int i = 0 ; i < this->_nbNodes ; i++) {
                for (unsigned int j = i + 1 ; j < this->_nbNodes ; j++) {
                    add_edge(i, j, EdgeProperties(0), g);
                }
            }
            
            
		}
		virtual ~Complete() {
			
		}

	protected:
        
};


#endif