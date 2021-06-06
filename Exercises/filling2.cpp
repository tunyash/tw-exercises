#include <iostream>
#include <vector>   
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <algorithm>
#include "2.h"

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

bool PathDecomposition::Check() {
	int n = boost::num_vertices(_g);

	for (std::vector<int> bag : _bags) {
		std::sort(bag.begin(), bag.end());
		bag.resize(std::unique(bag.begin(), bag.end()) - bag.begin());
		//Erasing non-unique vertices from bags
		for (int v : bag) {
			if (v < 0 || v >= n)
				throw PathDecomposition::CorectnessException("Vertices in bags are not in [0; |V(_g)|)");
		}
	}

	std::vector<int> sum(n);
	std::vector<int> left(n, n + 1);
	std::vector<int> right(n, -1);
	
	for (int i = 0; i < _bags.size(); ++i) {
		for (int v : _bags[i]) {
			sum[v]++;
			left[v] = std::min(left[v], i);
			right[v] = std::max(right[v], i);
		}
	}

	for (int i = 0; i < n; ++i) {
		if (sum[i] == 0) throw PathDecomposition::CorectnessException("Not all vertices are in path-width decomposition");
		if (right[i] - left[i] + 1 != sum[i]) {
			int j = left[i];
			for (; 
				std::find(_bags[j].begin(), _bags[j].end(), i) != _bags[j].end(); 
				++j);
			std::string msg = "In Decomposition _bags[i] and _bags[k] contains u but _bags[j] does not";
			auto ex = PathDecomposition::CorectnessException(msg);
			ex.type = 1;
			ex.i = left[i];
			ex.j = j;
			ex.k = right[i];
			ex.u = i;
		}
	}

	Graph path_g(n);
	
	for (std::vector<int> bag : _bags) {
		for (int v : bag) {
			for (int u : bag) {
				if (u == v) continue;
				boost::add_edge(u, v, path_g);
			}
		}
	}

	for (int v = 0; v < n; ++v) {
		for (auto it = boost::adjacent_vertices(v, _g).first; it != boost::adjacent_vertices(v, _g).second; ++it) {
			if (std::find(boost::adjacent_vertices(v, path_g).first, boost::adjacent_vertices(v, path_g).second, *it) ==
				boost::adjacent_vertices(v, path_g).second) throw PathDecomposition::CorectnessException("Not all edges are in path-width decomposition");
		}
	}
}

PathDecomposition::PathDecomposition(std::vector<std::vector<int>> bags, Graph g): _bags(bags), _g(g) {
	Check();
}
