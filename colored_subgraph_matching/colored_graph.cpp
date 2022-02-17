#include "colored_graph.h"

ColoredGraph make_colored_graph(const std::vector<std::pair<int, int>> &edges, const std::vector<ColorType> &colors) {
//    ColoredGraph graph(edges.begin(), edges.end(),colors.size());
    ColoredGraph graph;
//    graph[boost::vertex_color_t] = colors.begin();
    for (const auto &foo: colors) {
        boost::add_vertex(colored_graph_vertex_property(foo), graph);
    }
    for (const auto &edge: edges) {
        boost::add_edge(edge.first, edge.second, graph);
    }
    return graph;
}
