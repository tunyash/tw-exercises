#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <vector>
#include "1.h"
#include "2.h"
#include "doctest.h"

using namespace std;
using namespace boost;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

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

TEST_CASE("disconnected graph test") {
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

TEST_CASE("Correct path-width decomposition") {
    Graph g(/*n=*/4);
    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(3, 1, g);
    add_edge(3, 2, g);

    bool pass = 1;
    try {
        PathDecomposition({ {0, 1, 2, 3} }, g);
    }
    catch (...) {
        pass = 0;
    }
    CHECK(pass);
}

TEST_CASE("Not all vertices") {
    Graph g(/*n=*/4);
    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(3, 1, g);
    add_edge(3, 2, g);

    bool pass = 0;
    string msg;
    try {
        PathDecomposition({ {0, 1, 2} }, g);
    }
    catch (PathDecomposition::CorectnessException ex) {
        pass = 1;
    }
    CHECK(pass);
}

TEST_CASE("Not all edges") {
    Graph g(/*n=*/4);
    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(3, 1, g);
    add_edge(3, 2, g);

    bool pass = 0;
    string msg;
    try {
        PathDecomposition({ {0}, {1}, {2}, {3} }, g);
    }
    catch (PathDecomposition::CorectnessException ex) {
        pass = 1;
    }
    CHECK(pass);
}

TEST_CASE("Third rule exception") {
    Graph g(/*n=*/4);
    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(3, 1, g);
    add_edge(3, 2, g);

    bool pass = 0;
    string msg;
    try {
        PathDecomposition({ {0, 1}, {1, 2}, {2, 3}, {3, 0} }, g);
    }
    catch (PathDecomposition::CorectnessException ex) {
        pass = 1;
    }
    CHECK(pass);
}

TEST_CASE("Negative vertices") {
    Graph g(/*n=*/4);
    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(3, 1, g);
    add_edge(3, 2, g);

    bool pass = 0;
    string msg;
    try {
        PathDecomposition({ {-1} }, g);
    }
    catch (PathDecomposition::CorectnessException ex) {
        pass = 1;
    }
    CHECK(pass);
}
