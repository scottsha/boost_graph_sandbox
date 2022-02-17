#ifndef BOOST_GRAPH_SANDBOX_COLORED_GRAPH_SUBGRAPH_MATCHING_H
#define BOOST_GRAPH_SANDBOX_COLORED_GRAPH_SUBGRAPH_MATCHING_H

#include <unordered_map>

#include "colored_graph.h"

typedef std::unordered_map<int,int> VertexMatchMap;

std::vector<VertexMatchMap> common_connected_subgraphs(ColoredGraph g0, ColoredGraph g1);

#endif //BOOST_GRAPH_SANDBOX_COLORED_GRAPH_SUBGRAPH_MATCHING_H
