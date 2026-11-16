
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "../include/Graph.h"
#include "../include/Dijkstra.h"

using namespace std;

int passed = 0;
int failed = 0;

void checkResult(
    const string& testName,
    const DijkstraResult& result,
    const vector<int>& expectedPath,
    double expectedDistance)
{
    if (result.pathExists &&
        result.path == expectedPath &&
        abs(result.totalDistance - expectedDistance) < 1e-9)
    {
        cout << "PASS: " << testName << endl;
        passed++;
    }
    else
    {
        cout << "FAIL: " << testName << endl;
        failed++;
    }
}

void checkNoPath(
    const string& testName,
    const DijkstraResult& result)
{
    if (!result.pathExists)
    {
        cout << "PASS: " << testName << endl;
        passed++;
    }
    else
    {
        cout << "FAIL: " << testName << endl;
        failed++;
    }
}

int main()
{
    // Test 1: Normal shortest path
    {
        Graph graph;
        graph.addEdge(1, 2, 10);
        graph.addEdge(1, 4, 4);
        graph.addEdge(4, 3, 5);
        graph.addEdge(2, 3, 3);

        checkResult(
            "Normal shortest path",
            findShortestPath(graph, 1, 3),
            {1, 4, 3},
            9
        );
    }

    // Test 2: Direct route is shortest
    {
        Graph graph;
        graph.addEdge(1, 2, 3);
        graph.addEdge(2, 3, 4);
        graph.addEdge(1, 3, 5);

        checkResult(
            "Direct route is shortest",
            findShortestPath(graph, 1, 3),
            {1, 3},
            5
        );
    }

    // Test 3: Destination is unreachable
    {
        Graph graph;
        graph.addEdge(1, 2, 5);
        graph.addVertex(3);

        checkNoPath(
            "Unreachable destination",
            findShortestPath(graph, 1, 3)
        );
    }

    // Test 4: Invalid source
    {
        Graph graph;
        graph.addEdge(1, 2, 5);

        checkNoPath(
            "Invalid source",
            findShortestPath(graph, 10, 2)
        );
    }

    // Test 5: Invalid destination
    {
        Graph graph;
        graph.addEdge(1, 2, 5);

        checkNoPath(
            "Invalid destination",
            findShortestPath(graph, 1, 10)
        );
    }

    // Test 6: Source and destination are the same
    {
        Graph graph;
        graph.addVertex(1);

        checkResult(
            "Source equals destination",
            findShortestPath(graph, 1, 1),
            {1},
            0
        );
    }

    // Test 7: Zero-weight edge
    {
        Graph graph;
        graph.addEdge(1, 2, 0);
        graph.addEdge(2, 3, 5);
        graph.addEdge(1, 3, 10);

        checkResult(
            "Zero-weight edge",
            findShortestPath(graph, 1, 3),
            {1, 2, 3},
            5
        );
    }

    // Test 8: Empty graph
    {
        Graph graph;

        checkNoPath(
            "Empty graph",
            findShortestPath(graph, 1, 2)
        );
    }
    // Test 9: Multiple possible routes
{
    Graph graph;

    graph.addEdge(1, 2, 2);
    graph.addEdge(2, 4, 2);
    graph.addEdge(1, 3, 3);
    graph.addEdge(3, 4, 3);
    graph.addEdge(1, 4, 10);

    checkResult(
        "Multiple possible routes",
        findShortestPath(graph, 1, 4),
        {1, 2, 4},
        4
    );
}
// Test 10: Decimal distances
{
    Graph graph;

    graph.addEdge(1, 2, 2.5);
    graph.addEdge(2, 3, 1.75);
    graph.addEdge(1, 3, 6.0);

    checkResult(
        "Decimal distances",
        findShortestPath(graph, 1, 3),
        {1, 2, 3},
        4.25
    );
}

    cout << "\n-----------------------------" << endl;
    cout << "Tests passed: " << passed << endl;
    cout << "Tests failed: " << failed << endl;
    cout << "-----------------------------" << endl;

    return failed == 0 ? 0 : 1;
}
