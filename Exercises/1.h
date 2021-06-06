#include <iostream>
#include <vector>   
#include <random>
#include <algorithm>
#include <time.h>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

class disjoint_path_finder {

private:

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef boost::graph_traits<Graph>::edge_descriptor edge_t;

    std::vector<int> _color;
    std::vector<std::pair<int, int>> _pairs;
    Graph _g;
    
    vertex_t get_vertex_by_number(int i, Graph& g) {
        /*
        Transform number of vertex |i| in Graph |g| into vertex_t 
        @param Order number of vertex in the Graph |g|
        @return vertex_t in the Graph |g|
        */
        return (*(vertices(g).first + i));
    }

    bool my_dfs(vertex_t start, vertex_t end, int c, Graph& g) { 
/*
        Finds a path from |start| to |end| in |g| while enumerating the neighbors of each vertex in an order chosen at random. 
        Marks all the nodes along the found path in the |this->_color| vector with |c|.

        @param start is the start node of the potential path
        @param end is the end node of the potential path
        @param c is the _color to mark the nodes in the path with
        @param g the graph to make the traversal
        @return true if a path was found and false otherwise
*/
        _color[start] = c;
        if (start == end) return true;
        std::vector<int> go(degree(start, g));
        for (int i = 0; i < go.size(); ++i) {
            go[i] = i;
        }
        random_shuffle(go.begin(), go.end());
        for (int i : go) {
            auto u = *(adjacent_vertices(start, g).first + i);
            if ((_color[u] == -1 || _color[u] == c) && my_dfs(u, end, c, g)) return true;
        }
        _color[start] = -1;
        return false;
    }

public:

    disjoint_path_finder(int n, std::vector<std::pair<int, int>> edges, std::vector<std::pair<int, int>> pairs) : _pairs(pairs) {
        /*
        Constructor generates Graph |_g| with |n| vertices and edges from |edges|
        @param |n| amount of verices
        @param |edges| - edges of the Graph |_g|
        @param |_pairs| - pairs of vertices that would form disjoint path decomposition 
        */
        srand(time(0));
        _color.assign(n, -1);
        _g = Graph(edges.begin(), edges.end(), n);
    }

    std::pair<bool, std::vector<int>> solve(int tries = 100) {
        /*
        Function tries |tries| times to find disjoint path decomposition of the graph |_g|
        @param |tries| is amount of tries
        @return pair.first is true if disjoint path decomposition was found else false 
        @return pair.second is coloring of graph |_g| where |color[v] == c| means that vertex |v| belongs to path from |_pairs[c].first| to |_pairs[c].second| 
        */
        for (int t = 0; t < tries; ++t) {
            _color.assign(_color.size(), -1);
            for (int i = 0; i < _pairs.size(); ++i) {
                _color[_pairs[i].first] = i;
                _color[_pairs[i].second] = i;
            }
            bool ok = 1;
            for (int i = 0; i < _pairs.size() && ok; ++i) {
                if (!my_dfs(get_vertex_by_number(_pairs[i].first, _g),
                    get_vertex_by_number(_pairs[i].second, _g),
                    i, _g)) ok = 0;
            }
            if (ok) {
                return make_pair(true, _color);
            }
            return std::make_pair(false, std::vector<int>(_color.size(), -1));
        } 
    }
};
