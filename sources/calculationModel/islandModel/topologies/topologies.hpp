#ifndef JXTOPHER_TOPOLOGIES_H
#define JXTOPHER_TOPOLOGIES_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/properties.hpp>

using namespace boost;

///
/// @brief Type de graphe
///
struct VertexProperties {
	float d;
	int predecessor;
	VertexProperties() : d(std::numeric_limits<float>::infinity()), predecessor(-1) {
	}
	VertexProperties(float d, int predecessor) : d(d), predecessor(-1) {
	}
};

struct EdgeProperties {
	int weight;
	EdgeProperties() : weight(0) {
	}
	EdgeProperties(int weight) : weight(weight) {
	}
};

struct EdgeInfoPropertyTag {
	typedef edge_property_tag kind;
	static std::size_t const num; // ???
};

std::size_t const EdgeInfoPropertyTag::num = (std::size_t)&EdgeInfoPropertyTag::num;
typedef property<EdgeInfoPropertyTag, EdgeProperties> edge_info_prop_type;

typedef adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
					   VertexProperties,   // Type vertex
					   edge_info_prop_type // Type edge
					   >
	Graph;

class Topologies {
  public:
	static constexpr const char* CIRCLE = "Circle";
	static constexpr const char* COMPLETE = "Complete";
	static constexpr const char* RANDOMEDGE = "RandomEdge";
	static constexpr const char* STAR = "Star";

	Topologies(unsigned int number_of_nodes) : _number_of_nodes(number_of_nodes) {
	}
	virtual ~Topologies() {
	}
	// virtual void operator()() = 0;

	void print() {
		print_graph(g);
	}

	const adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
						 VertexProperties,   // Type vertex
						 edge_info_prop_type // Type edge
						 >&
		graph() const {
		return g;
	}

  protected:
	Graph g; /// < graphe g
	unsigned int _number_of_nodes;
};

#endif