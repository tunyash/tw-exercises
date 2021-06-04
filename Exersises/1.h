#include <iostream>
#include <string>
#include <vector>   
#include <random>
#include <algorithm>
#include <time.h>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

using namespace std;
using namespace boost;

class disjoint_path_finder {

private:
    struct Vertex {
        int id = -1;
    };

    typedef adjacency_list<vecS, vecS, undirectedS,
        Vertex, no_property> Graph;
    typedef graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef graph_traits<Graph>::edge_descriptor edge_t;

    vector<int> color;
    vector<pair<int, int>> pairs;
    Graph g;
    
    vertex_t get_vertex_by_number(int i, Graph& g) {
        return (*(vertices(g).first + i));
    }

    bool my_dfs(vertex_t start, vertex_t end, int c, Graph& g) { 
        // returns true if path from s_i to t_i was found
        // else false
        color[g[start].id] = c;
        if (start == end) return true;
        int size = adjacent_vertices(start, g).second - adjacent_vertices(start, g).first;
        vector<int> go(size);
        for (int i = 0; i < go.size(); ++i) {
            go[i] = i;
        }
        random_shuffle(go.begin(), go.end());
        for (int i = 0; i < go.size(); ++i) {
            auto u = *(adjacent_vertices(start, g).first + go[i]);
            if ((color[g[u].id] == -1 || color[g[u].id] == c) && my_dfs(u, end, c, g)) return true;
        }
        color[g[start].id] = -1;
        return false;
    }

public:

    disjoint_path_finder(int n, vector<pair<int, int>> edges, vector<pair<int, int>> pairs) : pairs(pairs) {
        srand(time(0));
        color.assign(n, -1);
        g = Graph(edges.begin(), edges.end(), n);
        int i = 0;
        for (auto j = vertices(g).first; j != vertices(g).second; ++j) {
            g[*j].id = i;
            ++i;
        }
    }

    pair<bool, vector<int>> solve() {
        for (int tries = 0; tries < 10; ++tries) {
            color.assign(color.size(), -1);
            for (int i = 0; i < pairs.size(); ++i) {
                color[pairs[i].first] = i;
                color[pairs[i].second] = i;
            }
            bool ok = 1;
            for (int i = 0; i < pairs.size() && ok; ++i) {
                if (!my_dfs(get_vertex_by_number(pairs[i].first, g),
                    get_vertex_by_number(pairs[i].second, g),
                    i, g)) ok = 0;
            }
            if (ok && find(color.begin(), color.end(), -1) == color.end()) {
                return make_pair(true, color);
            }
            return make_pair(false, vector<int>(color.size(), -1));
        } 
    }
};