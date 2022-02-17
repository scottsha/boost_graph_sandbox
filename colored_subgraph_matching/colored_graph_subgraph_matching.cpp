#include "colored_graph_subgraph_matching.h"

#include "boost/graph/mcgregor_common_subgraphs.hpp"

class CommonSubgraphCallback {
    private:
        const ColoredGraph &graph0_;
        const ColoredGraph &graph1_;
        std::vector<VertexMatchMap> match_maps_;

    public:
        CommonSubgraphCallback(const ColoredGraph &graph_0,
                       const ColoredGraph &graph_1) :
                graph0_(graph_0), graph1_(graph_1) {}

        std::vector<VertexMatchMap> get_match_maps() {return match_maps_;}

        template<typename CorrespondenceMapFirstToSecond,
                typename CorrespondenceMapSecondToFirst>
        bool operator()(CorrespondenceMapFirstToSecond correspondence_map_0_to_1,
                        CorrespondenceMapSecondToFirst correspondence_map_1_to_0,
                        typename boost::graph_traits<ColoredGraph>::vertices_size_type subgraph_size) {
            // Store all the vertex correspondences between the two subgraphs, replacing any that are not maximal.
//            if (subgraph_size < 2) return true;
            // First read out the matching:
            VertexMatchMap match_map;
            BGL_FORALL_VERTICES_T(vertex0, graph0_, ColoredGraph) {
                // Skip unmapped vertices
                if (get(correspondence_map_0_to_1, vertex0) != boost::graph_traits<ColoredGraph>::null_vertex()) {
                    int vertex1 = get(correspondence_map_0_to_1, vertex0);
                    match_map[vertex0] = vertex1;
                }
            }
            // Now check that this isn't a super- or subgraph of the last subgraph.

            if (!match_maps_.empty()) {
                // Check if the matches are actually just redundant inclusions:
                const VertexMatchMap& last_match = match_maps_.back();
                const auto& larger_match = match_map.size() > last_match.size() ? match_map : last_match;
                VertexMatchMap merger = last_match;
                merger.insert(match_map.begin(), match_map.end());
                if (merger.size() == larger_match.size()) {
                    match_maps_.back() = larger_match;
                } else {
                    match_maps_.push_back(match_map);
                }
            } else {
                match_maps_.push_back(match_map);
            }

//            match_maps_.push_back(match_map);
            std::cout << std::endl;
            return (true);
        }
};

std::vector<VertexMatchMap> common_connected_subgraphs(ColoredGraph g0, ColoredGraph g1) {
    // Define the vertex equivalence to work on vertex colors
    auto graph_color_equivalence = boost::make_property_map_equivalent(
            boost::get(boost::vertex_color, g0), boost::get(boost::vertex_color, g1));
    //
    CommonSubgraphCallback subgraph_callback(g0, g1);
    boost::property_map<ColoredGraph , boost::vertex_index_t>::type g0_vertex_index_map = boost::get(boost::vertex_index, g0);
    boost::property_map<ColoredGraph , boost::vertex_index_t>::type g1_vertex_index_map = boost::get(boost::vertex_index, g1);
    // Actual subgraph matching call:
    boost::mcgregor_common_subgraphs(g0,
                                                    g1,
                                                    g0_vertex_index_map,
                                                    g1_vertex_index_map,
                                                    boost::always_equivalent(),
                                                    graph_color_equivalence,
                                                    true,
                                                    std::ref(subgraph_callback));
    std::vector<VertexMatchMap> match_maps = subgraph_callback.get_match_maps();
    return match_maps;
}
