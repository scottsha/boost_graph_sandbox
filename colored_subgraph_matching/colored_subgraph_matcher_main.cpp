#include <iostream>
#include <vector>

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


void example_simplest_nonmatch() {
    std::vector<std::pair<int,int>> albert_edges = {};
    std::vector<ColorType> albert_colors = {0};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {};
    std::vector<ColorType> bertie_colors = {1};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    match_example_graphs(albert, bertie);
}

void example_simplest_match() {
    std::vector<std::pair<int,int>> albert_edges = {};
    std::vector<ColorType> albert_colors = {5};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    //
    std::vector<std::pair<int,int>> bertie_edges = {};
    std::vector<ColorType> bertie_colors = {5};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    //
    match_example_graphs(albert, bertie);
}

void example_single_edges() {
    std::vector<std::pair<int,int>> albert_edges = {{0,1}};
    std::vector<ColorType> albert_colors = {2, 2};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    write_graphviz(std::cout, albert);
    std::cout << "- - -" << std::endl;
    //
    std::vector<std::pair<int,int>> bertie_edges = {{0,1}};
    std::vector<ColorType> bertie_colors = {2, 2};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    write_graphviz(std::cout, bertie);
    std::cout << "- - -" << std::endl;
    //
    match_example_graphs(albert, bertie);
}

void example_triangles() {
    std::vector<std::pair<int,int>> albert_edges = {{0,1}, {1,2}, {2,0}};
    std::vector<ColorType> albert_colors = {4,3,3};
    auto albert = make_colored_graph(albert_edges, albert_colors);
    write_graphviz(std::cout, albert);
    std::cout << "- - -" << std::endl;
    //
    std::vector<std::pair<int,int>> bertie_edges = {{0,1}, {1,2}, {2,0}};
    std::vector<ColorType> bertie_colors = {3,3,3};
    auto bertie = make_colored_graph(bertie_edges, bertie_colors);
    write_graphviz(std::cout, bertie);
    std::cout << "- - -" << std::endl;
    //
    match_example_graphs(albert, bertie);
}

int main(int argc, const char **argv) {
    example_triangles();
    return 0;
}