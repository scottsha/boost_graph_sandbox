#ifndef BOOST_GRAPH_SANDBOX_COLORED_GRAPH_H
#define BOOST_GRAPH_SANDBOX_COLORED_GRAPH_H

#include <boost/graph/adjacency_list.hpp>

typedef int64_t ColorType;

typedef boost::property<boost::vertex_color_t, ColorType, boost::property<boost::vertex_index_t, int>> colored_graph_vertex_property;

// This boost template creates a simple graph with edges and vertices stored as std::vectors with an index and int valued color for each vertex.
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, colored_graph_vertex_property> ColoredGraph;

ColoredGraph make_colored_graph(const std::vector<std::pair<int, int>> &edges, const std::vector<ColorType> &colors);


#endif //BOOST_GRAPH_SANDBOX_COLORED_GRAPH_H
