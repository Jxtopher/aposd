#ifndef JXTOPHER_RANDOM_H
#define JXTOPHER_RANDOM_H

#include <random>

#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>

#include "topologies.h"

using namespace std;
using namespace boost;

class RandomEdge : public Topologies {
	public:
		RandomEdge(std::mt19937 &mt_rand, unsigned int nbNodes, double p = 0.20) : 
            Topologies(nbNodes) {
			assert(p <= 1 && p >= 0);
			uniform_real_distribution<> urd(0, 1);
            for (unsigned int i = 0 ; i < this->_nbNodes ; i++) {
                add_vertex(this->g);
            }

            for (unsigned int i = 0 ; i < this->_nbNodes ; i++) {
                for (unsigned int j = i + 1 ; j < this->_nbNodes ; j++) {
					if (urd(mt_rand) < p) {
						add_edge(i, j, EdgeProperties(0), g);
					}
                    
                }
            }
		}
		virtual ~RandomEdge() {
			
		}

	protected:
        
};


#endif