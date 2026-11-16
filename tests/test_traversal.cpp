// Tests for BFS and DFS.
//
// Compile and run from the project root:
//   g++ -std=c++17 -Wall -Wextra -Iinclude tests/test_traversal.cpp src/Graph.cpp src/Traversal.cpp -o test_traversal
//   ./test_traversal

#include <iostream>
#include <string>
#include <vector>

#include "Graph.h"
#include "Traversal.h"

static int passed = 0;
static int failed = 0;

static std::string toString(const std::vector<int>& v) {
    std::string s = "[";
    for (size_t i = 0; i < v.size(); i++) {
        if (i > 0) s += ", ";
        s += std::to_string(v[i]);
    }
    return s + "]";
}

static void check(const std::string& name, const std::vector<int>& actual,
                  const std::vector<int>& expected) {
    if (actual == expected) {
        passed++;
        std::cout << "PASS: " << name << "\n";
    } else {
        failed++;
        std::cout << "FAIL: " << name << "\n"
                  << "      expected " << toString(expected)
                  << ", got " << toString(actual) << "\n";
    }
}

/* Connected graph (a small tree):
          1
         / \
        2   3
       / \   \
      4   5   6
*/
static void testConnectedGraph() {
    Graph g;
    g.addEdge(1, 2, 10);
    g.addEdge(1, 3, 20);
    g.addEdge(2, 4, 5);
    g.addEdge(2, 5, 7);
    g.addEdge(3, 6, 3);

    check("connected graph - BFS from 1", bfs(g, 1), {1, 2, 3, 4, 5, 6});
    check("connected graph - DFS from 1", dfs(g, 1), {1, 2, 4, 5, 3, 6});
    check("connected graph - BFS from 4", bfs(g, 4), {4, 2, 1, 5, 3, 6});
    check("connected graph - DFS from 4", dfs(g, 4), {4, 2, 1, 3, 6, 5});
}

// Cycle: 1 - 2 - 3 - 4 - 1, plus a self-loop on 3.
// Each location must appear exactly once.
static void testCycle() {
    Graph g;
    g.addEdge(1, 2, 1);
    g.addEdge(2, 3, 1);
    g.addEdge(3, 4, 1);
    g.addEdge(4, 1, 1);
    g.addEdge(3, 3, 1);

    check("cycle - BFS from 1", bfs(g, 1), {1, 2, 4, 3});
    check("cycle - DFS from 1", dfs(g, 1), {1, 2, 3, 4});
}

// Disconnected graph: {1, 2, 3} and {10, 11}, plus lone vertex 20.
// Only locations reachable from the start should be visited.
static void testDisconnectedGraph() {
    Graph g;
    g.addEdge(1, 2, 4);
    g.addEdge(2, 3, 4);
    g.addEdge(10, 11, 8);
    g.addVertex(20);

    check("disconnected - BFS from 1", bfs(g, 1), {1, 2, 3});
    check("disconnected - DFS from 1", dfs(g, 1), {1, 2, 3});
    check("disconnected - BFS from 11", bfs(g, 11), {11, 10});
    check("disconnected - DFS from 11", dfs(g, 11), {11, 10});
    check("disconnected - BFS from 20", bfs(g, 20), {20});
    check("disconnected - DFS from 20", dfs(g, 20), {20});
}

static void testEmptyGraph() {
    Graph g;
    check("empty graph - BFS", bfs(g, 1), {});
    check("empty graph - DFS", dfs(g, 1), {});
}

static void testSingleVertex() {
    Graph g;
    g.addVertex(7);
    check("single vertex - BFS", bfs(g, 7), {7});
    check("single vertex - DFS", dfs(g, 7), {7});
}

static void testInvalidStart() {
    Graph g;
    g.addEdge(1, 2, 5);
    check("invalid start - BFS", bfs(g, 99), {});
    check("invalid start - DFS", dfs(g, 99), {});
    check("negative start - BFS", bfs(g, -1), {});
    check("negative start - DFS", dfs(g, -1), {});
}

int main() {
    testConnectedGraph();
    testCycle();
    testDisconnectedGraph();
    testEmptyGraph();
    testSingleVertex();
    testInvalidStart();

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
