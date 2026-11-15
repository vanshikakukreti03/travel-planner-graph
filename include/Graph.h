
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>

using namespace std;

class Graph {
private:
    unordered_map<int, vector<pair<int, double>>> adj;

public:
    // Add a new location
    void addVertex(int vertex);

    // Add a route between two locations
    void addEdge(int source, int destination, double distance);

    // Remove a location
    void removeVertex(int vertex);

    // Remove a route
    void removeEdge(int source, int destination);

    // Get all neighbouring locations
    vector<pair<int, double>> getNeighbors(int vertex);

    // Check whether a location exists
    bool hasVertex(int vertex);

    // Check whether a route exists
    bool hasEdge(int source, int destination);

    // Get all location IDs
    vector<int> getVertices();

    // Get the distance between two connected locations
    double getEdgeWeight(int source, int destination);
};

#endif
