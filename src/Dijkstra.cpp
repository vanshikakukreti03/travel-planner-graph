
#include "Dijkstra.h"

#include <iostream>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <functional>

using namespace std;

DijkstraResult findShortestPath(Graph& graph, int source, int destination)
{
    DijkstraResult result;

    result.pathExists = false;
    result.totalDistance = 0;

    // Step 1: Check whether both locations exist
    if (!graph.hasVertex(source) || !graph.hasVertex(destination))
    {
        cout << "Invalid source or destination location." << endl;
        return result;
    }

    // Step 2: Store shortest distances
    unordered_map<int, double> distance;

    // Step 3: Store previous location for path reconstruction
    unordered_map<int, int> parent;

    const double INF = numeric_limits<double>::infinity();

    // Step 4: Initialize all distances to infinity
    vector<int> vertices = graph.getVertices();

    for (int vertex : vertices)
    {
        distance[vertex] = INF;
    }

    // Distance from source to itself is zero
    distance[source] = 0;

    // Step 5: Create a min-priority queue
    priority_queue<
        pair<double, int>,
        vector<pair<double, int>>,
        greater<pair<double, int>>
    > pq;

    pq.push({0, source});

    // Step 6: Apply Dijkstra's algorithm
    while (!pq.empty())
    {
        double currentDistance = pq.top().first;
        int currentVertex = pq.top().second;

        pq.pop();

        // Ignore outdated entries
        if (currentDistance > distance[currentVertex])
        {
            continue;
        }

        // Stop when destination is reached optimally
        if (currentVertex == destination)
        {
            break;
        }

        // Get all neighbouring locations
        vector<pair<int, double>> neighbors =
            graph.getNeighbors(currentVertex);

        for (const auto& edge : neighbors)
        {
            int neighbor = edge.first;
            double weight = edge.second;

            double newDistance = currentDistance + weight;

            // Update if a shorter route is found
            if (newDistance < distance[neighbor])
            {
                distance[neighbor] = newDistance;
                parent[neighbor] = currentVertex;

                pq.push({newDistance, neighbor});
            }
        }
    }

    // Step 7: Check whether destination is reachable
    if (distance[destination] == INF)
    {
        cout << "No route exists between these locations." << endl;
        return result;
    }

    // Step 8: Reconstruct the shortest path
    vector<int> path;

    int current = destination;

    while (current != source)
    {
        path.push_back(current);
        current = parent[current];
    }

    path.push_back(source);

    reverse(path.begin(), path.end());

    // Step 9: Store final result
    result.pathExists = true;
    result.path = path;
    result.totalDistance = distance[destination];

    return result;
}
