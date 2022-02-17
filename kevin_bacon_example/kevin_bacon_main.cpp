//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee,
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <map>


template < typename DistanceMap >
class bacon_number_recorder : public boost::default_bfs_visitor
{
public:
    bacon_number_recorder(DistanceMap dist) : d(dist) {}

    template < typename Edge, typename Graph >
    void tree_edge(Edge e, const Graph& g) const
    {
        typename boost::graph_traits< Graph >::vertex_descriptor u = source(e, g),
                v = target(e, g);
        d[v] = d[u] + 1;
    }

private:
    DistanceMap d;
};

// Convenience function
template < typename DistanceMap >
bacon_number_recorder< DistanceMap > record_bacon_number(DistanceMap d)
{
    return bacon_number_recorder< DistanceMap >(d);
}

int main(int argc, const char** argv)
{
    std::ifstream datafile(argc >= 2 ? argv[1] : "kevin-bacon.dat");
    if (!datafile)
    {
        std::cerr << "No kevin-bacon.dat file" << std::endl;
        return EXIT_FAILURE;
    }

    typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS,
    boost::property< boost::vertex_name_t, std::string >,
    boost::property< boost::edge_name_t, std::string > >
            Graph;
    Graph g;

    typedef boost::property_map< Graph, boost::vertex_name_t >::type actor_name_map_t;
    actor_name_map_t actor_name = get(boost::vertex_name, g);
    typedef boost::property_map< Graph, boost::edge_name_t >::type movie_name_map_t;
    movie_name_map_t connecting_movie = get(boost::edge_name, g);

    typedef boost::graph_traits< Graph >::vertex_descriptor Vertex;
    typedef std::map< std::string, Vertex > NameVertexMap;
    NameVertexMap actors;

    for (std::string line; std::getline(datafile, line);)
    {
        boost::char_delimiters_separator< char > sep(false, "", ";");
        boost::tokenizer<> line_toks(line, sep);
        boost::tokenizer<>::iterator i = line_toks.begin();
        std::string actors_name = *i++;
        NameVertexMap::iterator pos;
        bool inserted;
        Vertex u, v;
        boost::tie(pos, inserted)
                = actors.insert(std::make_pair(actors_name, Vertex()));
        if (inserted)
        {
            u = add_vertex(g);
            actor_name[u] = actors_name;
            pos->second = u;
        }
        else
            u = pos->second;

        std::string movie_name = *i++;

        boost::tie(pos, inserted) = actors.insert(std::make_pair(*i, Vertex()));
        if (inserted)
        {
            v = add_vertex(g);
            actor_name[v] = *i;
            pos->second = v;
        }
        else
            v = pos->second;

        boost::graph_traits< Graph >::edge_descriptor e;
        boost::tie(e, inserted) = add_edge(u, v, g);
        if (inserted)
            connecting_movie[e] = movie_name;
    }

    std::vector< int > bacon_number(num_vertices(g));

    Vertex src = actors["Kevin Bacon"];
    bacon_number[src] = 0;

    breadth_first_search(
            g, src, visitor(record_bacon_number(&bacon_number[0])));

    boost::graph_traits< Graph >::vertex_iterator i, end;
    for (boost::tie(i, end) = vertices(g); i != end; ++i)
    {
        std::cout << actor_name[*i] << " has a Bacon number of "
                  << bacon_number[*i] << std::endl;
    }

    return 0;
}