#include <iostream>
#include <vector>   
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <algorithm>
#include "2.h"

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

enum error_types {
	_edges = 0,
	_negative_vertices = 1,
	_continuity = 2,
	_vertices = 3
};

PathDecomposition::CorectnessException::CorectnessException(int error_type): _error_type(error_type) {}
PathDecomposition::CorectnessException::CorectnessException
(int i, int j, int k, int u): 
_error_type(_continuity), _i(i), _j(j), _k(k), _u(u) {}

const char *PathDecomposition::CorectnessException::what() const throw() {
	switch (_error_type) {
	case _edges:
		return "Not all edges are in path-width decomposition";
	case _negative_vertices:
		return "Vertices in bags are not in [0; |V(_g)|)";
	case _continuity:
		return ("_bags["
			+ std::to_string(_i)
			+ "] and _bags["
			+ std::to_string(_k)
			+ "] contains "
			+ std::to_string(_u)
			+ " but _bags["
			+ std::to_string(_j)
			+ "] does not").c_str();
	case _vertices:
		return "Not all vertices are in path-width decomposition";
	}
}

bool PathDecomposition::Check() {
	int n = boost::num_vertices(_g);

	for (std::vector<int> bag : _bags) {
		std::sort(bag.begin(), bag.end());
		bag.resize(std::unique(bag.begin(), bag.end()) - bag.begin());
		//Erasing non-unique vertices from bags
		for (int v : bag) {
			if (v < 0 || v >= n)
				throw PathDecomposition::CorectnessException(_negative_vertices);
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
		if (sum[i] == 0) throw PathDecomposition::CorectnessException(_vertices);
		if (right[i] - left[i] + 1 != sum[i]) {
			int j = left[i];
			for (; 
				std::find(_bags[j].begin(), _bags[j].end(), i) != _bags[j].end(); 
				++j);
			throw PathDecomposition::CorectnessException(left[i], j, right[i], i);
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
				boost::adjacent_vertices(v, path_g).second) throw PathDecomposition::CorectnessException(_edges);
		}
	}
}

PathDecomposition::PathDecomposition(std::vector<std::vector<int>> bags, Graph g): _bags(bags), _g(g) {
	Check();
}
