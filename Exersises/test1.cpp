#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <vector>
#include "1.h"
#include "doctest.h"

using namespace std;

TEST_CASE("simple test") {
    vector<pair<int, int>> edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}
    };
    vector<pair<int, int>> pairs = {
        {0, 1}, {2, 3}
    };
    disjoint_path_finder q(/*n=*/4, edges, pairs);
    CHECK(q.solve(/*tries=*/30).first);
}

TEST_CASE("another simple test") {
    vector<pair<int, int>> edges = {
        {0, 1}, {2, 3}
    };
    vector<pair<int, int>> pairs = {
        {0, 3}, {1, 2}
    };
    disjoint_path_finder q(/*n=*/4, edges, pairs);
    CHECK(q.solve(/*tries=*/30).first == false);
}

TEST_CASE("exceptions") {
    vector<pair<int, int>> edges = {
        {-1, 0}
    };
    vector<pair<int, int>> pairs = {
        {0, 1}, {2, 3}
    };
    bool pass = 0;
    try {
        pass = 1;
        disjoint_path_finder q(/*n=*/-3, edges, pairs);
        q.solve(/*tries=*/30);
    }
    catch (...) {
        pass = 0;
    }
    CHECK(pass == false);
}