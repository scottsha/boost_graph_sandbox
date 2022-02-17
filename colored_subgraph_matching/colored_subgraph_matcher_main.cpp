#include <iostream>
#include <vector>

#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/mcgregor_common_subgraphs.hpp>
#include "colored_graph.h"

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> SimpleGraph;
typedef boost::property_map<SimpleGraph, boost::vertex_index_t>::type IndexMap;

template<typename GraphFirst,
        typename GraphSecond>
struct print_callback {
    print_callback(const GraphFirst &graph1,
                   const GraphSecond &graph2) :
            m_graph1(graph1), m_graph2(graph2) {}

    template<typename CorrespondenceMapFirstToSecond,
            typename CorrespondenceMapSecondToFirst>
    bool operator()(CorrespondenceMapFirstToSecond correspondence_map_1_to_2,
                    CorrespondenceMapSecondToFirst correspondence_map_2_to_1,
                    typename boost::graph_traits<GraphFirst>::vertices_size_type subgraph_size) {

        // Print out correspondences between vertices
        BGL_FORALL_VERTICES_T(vertex1, m_graph1, GraphFirst) {

                // Skip unmapped vertices
                if (get(correspondence_map_1_to_2, vertex1) != boost::graph_traits<GraphSecond>::null_vertex()) {
                    std::cout << vertex1 << " <-> " << get(correspondence_map_1_to_2, vertex1) << std::endl;
                }

            }
        std::cout << "Count " << cnt++ << std::endl;
        std::cout << "---" << std::endl;
        return (true);
    }

private:
    const GraphFirst &m_graph1;
    const GraphSecond &m_graph2;
    int cnt = 0;
};


void example_no_color_subgraph_search() {
    std::vector<std::pair<int, int>> edges;
    int num_tris = 5;
    for (auto foo = 0; foo < num_tris; foo++) {
        int tristart = 3 * foo + 1;
        edges.push_back({0, tristart});
        edges.push_back({tristart, tristart + 1});
        edges.push_back({tristart + 1, tristart + 2});
        edges.push_back({tristart + 2, tristart});
    }
    int num_verts = num_tris * 3 + 1;
//    std::vector<int> colors = {0, 0, 0, 0, 1, 0, 1, 0, 0};
//    for (auto foo = colors.size(); foo < num_verts; foo++) {
//        colors.push_back(foo % 3);
//    }
    SimpleGraph flower(edges.begin(), edges.end(), num_verts);
    // Describe the vertices
    IndexMap index = get(boost::vertex_index, flower);
    std::cout << "Vertices: " << std::endl;
    boost::graph_traits<SimpleGraph>::vertex_iterator vi;
    for (vi = boost::vertices(flower).first; vi != boost::vertices(flower).second; ++vi) {
        std::cout << " vertex: " << index[*vi] << std::endl;
//        std::cout << " vertex: " << index[*vi] << " is color " << colors[index[*vi]] << std::endl;
    }
    // Describe the edges
    for (auto edge = boost::edges(flower); edge.first != edge.second; ++edge.first) {
        std::cout << "edge: " << index[boost::source(*edge.first, flower)] << " "
                  << index[boost::target(*edge.first, flower)] << std::endl;
    }
    // Lilgraph
    std::vector<std::pair<int, int>> petal_edges = {
            {0, 1},
            {1, 2},
            {2, 0}
    };
    SimpleGraph petal(petal_edges.begin(), petal_edges.end(), 3);
    // Try to match
    print_callback<SimpleGraph, SimpleGraph> my_callback(flower, petal);
    boost::mcgregor_common_subgraphs_maximum_unique(flower, petal, true, my_callback);
}


std::pair<std::vector<std::pair<int, int>>, std::vector<ColorType>> make_flower_graph(int num_petals) {
    std::vector<std::pair<int, int>> edges;
    for (auto foo = 0; foo < num_petals; foo++) {
        int tristart = 3 * foo + 1;
        edges.push_back({0, tristart});
        edges.push_back({tristart, tristart + 1});
        edges.push_back({tristart + 1, tristart + 2});
        edges.push_back({tristart + 2, tristart});
    }
    int num_verts = num_petals * 3 + 1;
    std::vector<ColorType> colors = {};
    for (int foo = colors.size(); foo < num_verts; foo++) {
        int base_pow = (foo % 3);
        int base = std::pow(3, base_pow);
        colors.push_back((foo / base) % 3);
    }
    return {edges, colors};
}


std::pair<std::vector<std::pair<int, int>>, std::vector<ColorType>> make_petal_graph() {
    return {
            {
                    {0, 1},
                       {1, 2},
                          {2, 0}
            },
            {       0, 0, 0}
    };
}


void color_subgraph_search() {
    auto flower_data = make_flower_graph(5);
    std::vector<std::pair<int, int>> flower_edges = flower_data.first;
    std::vector<ColorType> flower_colors = flower_data.second;
    auto flower = make_colored_graph(flower_data.first, flower_data.second);
    auto petal_data = make_petal_graph();
    auto petal = make_colored_graph(petal_data.first, petal_data.second);
    auto graph_color_equivalence = make_property_map_equivalent(
            boost::get(boost::vertex_color, flower), boost::get(boost::vertex_color, petal));
    print_callback<ColoredGraph, ColoredGraph> my_callback(flower, petal);
    boost::property_map<ColoredGraph , boost::vertex_index_t>::type flower_vertex_index_map = boost::get(boost::vertex_index, flower);
    boost::property_map<ColoredGraph , boost::vertex_index_t>::type petal_vertex_index_map = boost::get(boost::vertex_index, petal);
    boost::mcgregor_common_subgraphs_maximum_unique(flower,
                                                    petal,
                                                    flower_vertex_index_map,
                                                    petal_vertex_index_map,
                                                    boost::always_equivalent(),
                                                    graph_color_equivalence,
                                                    true,
                                                    my_callback);
//    void mcgregor_common_subgraphs_maximum_unique(const GraphFirst& graph1,
//                                                  const GraphSecond& graph2, const VertexIndexMapFirst vindex_map1,
//                                                  const VertexIndexMapSecond vindex_map2,
//                                                  EdgeEquivalencePredicate edges_equivalent,
//                                                  VertexEquivalencePredicate vertices_equivalent,
//                                                  bool only_connected_subgraphs, SubGraphCallback user_callback)
}


int main(int argc, const char **argv) {
    color_subgraph_search();
    return 0;
}