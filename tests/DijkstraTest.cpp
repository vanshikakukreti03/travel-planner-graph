
#include <iostream>
#include "../include/Graph.h"
#include "../include/Dijkstra.h"

using namespace std;

int main()
{
    Graph graph;

    // Create locations
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);
    graph.addVertex(5);

    // Create routes with distances
    graph.addEdge(1, 2, 10);
    graph.addEdge(1, 4, 4);
    graph.addEdge(4, 3, 5);
    graph.addEdge(2, 3, 3);

    int source = 1;
    int destination = 3;

    // Find shortest path
    DijkstraResult result =
        findShortestPath(graph, source, destination);

    if (result.pathExists)
    {
        cout << "\nShortest Path: ";

        for (int vertex : result.path)
        {
            cout << vertex << " ";
        }

        cout << "\nTotal Distance: "
             << result.totalDistance << " km" << endl;
    }

    return 0;
}
