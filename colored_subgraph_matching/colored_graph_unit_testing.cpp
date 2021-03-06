#include "gtest/gtest.h"

#include <boost/graph/graphviz.hpp>

#include "colored_graph_subgraph_matching.h"

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
            {       0, 1, 0}
    };
}

void match_example_graphs(ColoredGraph g0, ColoredGraph g1) {
    auto match_maps = common_connected_subgraphs(g0, g1);
    std::cout << "Matching reports " << match_maps.size() << " matching subgraphs." << std::endl;
    for (const auto &match_map: match_maps) {
        std::cout << "Common subgraph: " << std::endl;
        for (const auto &vertex_match: match_map) {
            std::cout << vertex_match.first
                      << " |-> "
                      << vertex_match.second
                      << std::endl;
        }
    }
}


void example_flower_search() {
    auto flower_data = make_flower_graph(5);
    std::vector<std::pair<int, int>> flower_edges = flower_data.first;
    std::vector<ColorType> flower_colors = flower_data.second;
    auto flower = make_colored_graph(flower_data.first, flower_data.second);
    auto petal_data = make_petal_graph();
    auto petal = make_colored_graph(petal_data.first, petal_data.second);
    match_example_graphs(flower, petal);
}

void example_five_and_four() {
    std::pair<std::vector<std::pair<int, int>>, std::vector<ColorType>> g_two_subs({
                                                                                           {
                                                                                                   {0, 1},
                                                                                                     {1, 2},
                                                                                                       {2, 3},
                                                                                                         {3, 4},
                                                                                                           {4, 0},
                                                                                                   //
                                                                                                             {5, 6},
                                                                                                               {6, 7},
                                                                                                                 {7, 8},
                                                                                                                   {8, 5}
                                                                                           },
                                                                                           {       0,0,0,0,0,0,0,0,0}
                                                                                   });
    auto albert = make_colored_graph(g_two_subs.first, g_two_subs.second);
    //
    std::pair<std::vector<std::pair<int, int>>, std::vector<ColorType>> g_connected({
                                                                                            {
                                                                                                    {0, 1},
                                                                                                    {1, 2},
                                                                                                    {2, 3},
                                                                                                    {3, 4},
                                                                                                     {4, 0},
                                                                                                    //
                                                                                                       {5, 6},
                                                                                                         {6, 7},
                                                                                                           {7, 8},
                                                                                                             {8, 5},
                                                                                                    //
                                                                                                    {0,5}
                                                                                            },
                                                                                            {1,1,1,1,1,1,1,1,1}
                                                                                    });
    auto bertie = make_colored_graph(g_two_subs.first, g_two_subs.second);
    match_example_graphs(albert, bertie);
}


TEST(colored_graph, singleton_nonmatch) {
    std::vector<std::pair<int,int>> albert_edges = {};
    std::vector<ColorType> albert_colors = {0};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {};
    std::vector<ColorType> bertie_colors = {1};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    auto matches = common_connected_subgraphs(albert, bertie);
    EXPECT_TRUE(matches.empty());
}

TEST(colored_graph, singleton_match) {
    std::vector<std::pair<int,int>> albert_edges = {};
    std::vector<ColorType> albert_colors = {0};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {};
    std::vector<ColorType> bertie_colors = {0};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    auto matches = common_connected_subgraphs(albert, bertie);
    EXPECT_EQ(matches.size(), 1);
}

TEST(colored_graph, edge_directish) {
    std::vector<std::pair<int,int>> albert_edges = {{0,1}};
    std::vector<ColorType> albert_colors = {0,1};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {{0,1}};
    std::vector<ColorType> bertie_colors = {0,1};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    auto matches = common_connected_subgraphs(albert, bertie);
    EXPECT_EQ(matches.size(), 1);
    auto match = matches.front();
    EXPECT_EQ(match[0], 0);
    EXPECT_EQ(match[1], 1);
}

void print_matchings (const std::vector<VertexMatchMap> matchings) {
    for (const auto& matching : matchings) {
        std::cout << "Matching:" << std::endl;
        for (const auto& vv : matching) {
            std::cout << vv.first << "<->" << vv.second << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "# matchings: " << matchings.size() << std::endl;
}

TEST(colored_graph, edge_reversable) {
    std::vector<std::pair<int,int>> albert_edges = {{0,1}};
    std::vector<ColorType> albert_colors = {5, 5};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {{0,1}};
    std::vector<ColorType> bertie_colors = {5, 5};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    auto matches = common_connected_subgraphs(albert, bertie);
//    print_matchings(matches);
    EXPECT_EQ(matches.size(), 2);
    auto match = matches.front();
    EXPECT_EQ(match.size(), 2);
    EXPECT_EQ(match[0], 0);
    EXPECT_EQ(match[1], 1);
    auto match2 = matches.back();
    EXPECT_EQ(match.size(), 2);
    EXPECT_EQ(match2[0], 1);
    EXPECT_EQ(match2[1], 0);
}

TEST(colored_graph, triangle_and_edge) {
    std::vector<std::pair<int,int>> albert_edges = {
            {0,1},
            {1,2},
            {2,0},
            {3,4},
            {4,5},
            {5,3}
    };
    std::vector<ColorType> albert_colors = {42, 42, 42, 42, 42, 42};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {
            {0,1},
            {1,2},
            {2,0},
    };
    std::vector<ColorType> bertie_colors = {42, 42, 42};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    auto matches = common_connected_subgraphs(albert, bertie);
//    print_matchings(matches);
    EXPECT_EQ(matches.size(), 12);
}

TEST(colored_graph, flippable_triangle) {
    std::vector<std::pair<int,int>> albert_edges = {
            {0,1},
            {1,2},
            {2,0}
    };
    std::vector<ColorType> albert_colors = {42, 42, 10};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {
            {0,1},
            {1,2},
            {2,0},
    };
    std::vector<ColorType> bertie_colors = {10, 42, 42};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    auto matchings = common_connected_subgraphs(albert, bertie);
//    print_matchings(matchings);
    EXPECT_EQ(matchings.size(), 2);
    for (const auto& matching : matchings) {
        EXPECT_EQ(matching.at(2), 0);
    }
}

TEST(colored_graph, disconnected) {
    std::vector<std::pair<int,int>> albert_edges = {
            {0,1},
            {1,2},
            {2,0},
            {3,4},
            {4,5},
            {5,6},
            {6,3},
            {0,6}
    };
    std::vector<ColorType> albert_colors = {0,1,2,3,4,5,5};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {
            {0,1},
            {1,2},
            {2,0},
            {3,4},
            {4,5},
            {5,6},
            {6,3},
    };
    std::vector<ColorType> bertie_colors = {0,1,2,5,5,4,3};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    auto matchings = common_connected_subgraphs(albert, bertie);
//    print_matchings(matchings);
    EXPECT_EQ(matchings.size(), 3);
    std::vector<int> expected_perm = {0, 1, 2, 6, 5, 4, 3};
    auto match0 = matchings[0];
    for (const auto& foo : match0) {
        EXPECT_EQ(expected_perm[foo.first], foo.second);
    }
    auto match1 = matchings[1];
    for (const auto& foo : match1) {
        EXPECT_EQ(expected_perm[foo.first], foo.second);
    }
}

TEST(colored_graph, k4_uniform) {
    std::vector<std::pair<int,int>> albert_edges = {
            {0,1},
            {0,2},
            {0,3},
            {1,2},
            {1,3},
            {2,3},
    };
    std::vector<ColorType> albert_colors = {0,0,0,0};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {
            {0,1},
            {0,2},
            {0,3},
            {1,2},
            {1,3},
            {2,3},
    };
    std::vector<ColorType> bertie_colors = {0,0,0,0};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    auto matchings = common_connected_subgraphs(albert, bertie);
    EXPECT_EQ(matchings.size(), 24);
}

TEST(colored_graph, k4_distinct_vertices) {
    std::vector<std::pair<int,int>> albert_edges = {
            {0,1},
            {0,2},
            {0,3},
            {1,2},
            {1,3},
            {2,3},
    };
    std::vector<ColorType> albert_colors = {0,1,2,3};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {
            {0,1},
            {0,2},
            {0,3},
            {1,2},
            {1,3},
            {2,3},
    };
    std::vector<ColorType> bertie_colors = {3,2,1,0};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    auto matchings = common_connected_subgraphs(albert, bertie);
    EXPECT_EQ(matchings.size(), 1);
}

TEST(colored_graph, disconnected_vertices) {
    std::vector<std::pair<int,int>> albert_edges = {};
    std::vector<ColorType> albert_colors = {0,0,0,0};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {};
    std::vector<ColorType> bertie_colors = {0,0,0,0};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    auto matchings = common_connected_subgraphs(albert, bertie);
    EXPECT_EQ(matchings.size(), 16);
}

ColoredGraph generate_dodecahedron(){
    std::vector<std::pair<int,int>> dodecahedron_edges = {
            {0,1},
            {1,2},
            {2,3},
            {3,4},
            {4,0},
            {0,5},
            {1,6},
            {2,7},
            {3,8},
            {4,9},
            {5,14},
            {5,10},
            {6,10},
            {6,11},
            {7,11},
            {7,12},
            {8,12},
            {8,13},
            {9,13},
            {9,14},
            {10,15},
            {11,16},
            {12,17},
            {13,18},
            {14,19},
            {15,16},
            {16,17},
            {17,18},
            {18,19},
            {19,15},
    };
    std::vector<ColorType> dodecahedron_colors = {42,42,42,42,42,42,42,42,42,42,42,42};
    return make_colored_graph(dodecahedron_edges, dodecahedron_colors);
}

ColoredGraph generate_icosahedron(){
    std::vector<std::pair<int,int>> icosahedron_edges = {
            {0,1},
            {0,2},
            {0,3},
            {0,4},
            {0,5},
            {1,2},
            {2,3},
            {3,4},
            {4,5},
            {5,1},
            {1,6},
            {1,10},
            {2,6},
            {2,7},
            {3,7},
            {3,8},
            {4,8},
            {4,9},
            {5,9},
            {5,10},
            {6,7},
            {7,8},
            {8,9},
            {9,10},
            {10,6},
            {6,11},
            {7,11},
            {8,11},
            {9,11},
            {10,11},
    };
    std::vector<ColorType> icosahedron_colors = {42,42,42,42,42,42,42,42,42,42,42,42,};
    return make_colored_graph(icosahedron_edges, icosahedron_colors);
}

TEST(colored_graph, tees_to_tees) {
    //
    std::vector<std::pair<int,int>> tee0_edges = {{0, 1}, {0, 2}, {0, 3},};
    std::vector<ColorType> tee0_colors = {42, 42, 42, 42};
    auto tee0 = make_colored_graph(tee0_edges, tee0_colors);
    //
    std::vector<std::pair<int,int>> tee1_edges = {{0, 1}, {0, 2}, {0, 3},};
    std::vector<ColorType> tee1_colors = {42, 42, 42, 42};
    auto tee1 = make_colored_graph(tee1_edges, tee1_colors);
    //
    auto matchings = common_connected_subgraphs(tee0, tee1);
    print_matchings(matchings);
    EXPECT_EQ(matchings.size(), 6);
}

TEST(colored_graph, dodecahedron_tees) {
    //
    auto dodec = generate_dodecahedron();
    std::vector<std::pair<int,int>> tee_edges = {{0, 1}, {0, 2}, {0, 3},};
    std::vector<ColorType> tee_colors = {42, 42, 42, 42};
    auto bertie = make_colored_graph(tee_edges, tee_colors);
    //
    auto matchings = common_connected_subgraphs(bertie, dodec);
    print_matchings(matchings);
    EXPECT_EQ(matchings.size(), 20 * 6);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
