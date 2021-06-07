#include <iostream>
#include <vector>   
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <algorithm>
#include "2.h"

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

PathDecomposition::CorectnessException::CorectnessException(int error_type): _error_type(error_type) {}
PathDecomposition::CorectnessException::CorectnessException
(int i, int j, int k, int u): 
_error_type(INCONTINUITY), _bag_i(i), _bag_j(j), _bag_k(k), _violating_node(u) {}

const char *PathDecomposition::CorectnessException::what() const throw() {
	switch (_error_type) {
	case EDGES:
		return "Not all edges are in path-width decomposition";
	case NEGATIVE_VERTICES:
		return "Vertices in bags are not in [0; |V(_g)|)";
	case INCONTINUITY:
		return ("_bags["
			+ std::to_string(_bag_i)
			+ "] and _bags["
			+ std::to_string(_bag_j)
			+ "] contains "
			+ std::to_string(_bag_k)
			+ " but _bags["
			+ std::to_string(_violating_node)
			+ "] does not").c_str();
	case VERTICES:
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
				throw PathDecomposition::CorectnessException(NEGATIVE_VERTICES);
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
		if (sum[i] == 0) throw PathDecomposition::CorectnessException(VERTICES);
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
				boost::adjacent_vertices(v, path_g).second) throw PathDecomposition::CorectnessException(EDGES);
		}
	}
}

PathDecomposition::PathDecomposition(std::vector<std::vector<int>> bags, Graph g): _bags(bags), _g(g) {
	Check();
}
