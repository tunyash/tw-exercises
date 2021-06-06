#include <iostream>
#include <vector>   
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <algorithm>
#include "2.h"

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

bool PathDecomposition::Check() {
	int n = boost::num_vertices(_g);

	for (std::vector<int> bug : _bugs) {
		std::sort(bug.begin(), bug.end());
		bug.resize(std::unique(bug.begin(), bug.end()) - bug.begin());
		//Erasing non-unique vertices from bugs
		for (int v : bug) {
			if (v < 0 || v >= n)
				throw PathDecomposition::CorectnessException("Vertices in bugs are not in [0; |V(_g)|)");
		}
	}

	std::vector<int> sum(n);
	std::vector<int> left(n, n + 1);
	std::vector<int> right(n, -1);
	
	for (int i = 0; i < _bugs.size(); ++i) {
		for (int v : _bugs[i]) {
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
				std::find(_bugs[j].begin(), _bugs[j].end(), i) != _bugs[j].end(); 
				++j);
			std::string msg = "In Decomposition _bugs["
				+ std::to_string(left[i])
				+ "] and _bugs["
				+ std::to_string(right[i])
				+ "] contains "
				+ std::to_string(i)
				+ " but _bugs["
				+ std::to_string(j)
				+ "] does not";
			throw PathDecomposition::CorectnessException(msg);
		}
	}

	Graph path_g(n);
	
	for (std::vector<int> bug : _bugs) {
		for (int v : bug) {
			for (int u : bug) {
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

PathDecomposition::PathDecomposition(std::vector<std::vector<int>> bugs, Graph g): _bugs(bugs), _g(g) {
	Check();
}
