#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "1.h"
#include <cstdio>

using namespace std;

int main() {
	freopen("input.txt", "r", stdin);

	int n, m, k; cin >> n >> m >> k;
	vector<pair<int, int>> edges;
	vector<pair<int, int>> pairs;
	for (int i = 0; i < m; ++i) {
		int v, u; cin >> v >> u;
		edges.push_back({ v, u });
	}
	for (int i = 0; i < k; ++i) {
		int v, u; cin >> v >> u;
		pairs.push_back({ v, u });
	}
	disjoint_path_finder q(n, edges, pairs);
	q.solve();

	return 0;
}