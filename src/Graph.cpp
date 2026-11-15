#include "Graph.h"
// Add a new location
void Graph::addVertex(int vertex) {
    if (!hasVertex(vertex)) {
        adj[vertex] = {};
    }
}

// Add a route between two locations
void Graph::addEdge(int source, int destination, double distance) {

    if (distance < 0) {
        cout << "Distance cannot be negative." << endl;
        return;
    }

    // Automatically create locations if they don't exist
    addVertex(source);
    addVertex(destination);

    // Check if route already exists
    for (auto &edge : adj[source]) {
        if (edge.first == destination) {
            edge.second = distance;

            for (auto &reverseEdge : adj[destination]) {
                if (reverseEdge.first == source) {
                    reverseEdge.second = distance;
                    return;
                }
            }
        }
    }

    // Add route in both directions
    adj[source].push_back({destination, distance});
    adj[destination].push_back({source, distance});
}
// Remove a location
void Graph::removeVertex(int vertex) {

    if (!hasVertex(vertex)) {
        return;
    }

    // Remove all connections to this location
    for (auto &entry : adj) {
        vector<pair<int, double>> &neighbors = entry.second;

        for (auto it = neighbors.begin(); it != neighbors.end();) {
            if (it->first == vertex) {
                it = neighbors.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Remove the location itself
    adj.erase(vertex);
}

// Remove a route
void Graph::removeEdge(int source, int destination) {

    if (!hasVertex(source) || !hasVertex(destination)) {
        return;
    }

    // Remove source -> destination
    auto &neighbors1 = adj[source];

    for (auto it = neighbors1.begin(); it != neighbors1.end();) {
        if (it->first == destination) {
            it = neighbors1.erase(it);
        } else {
            ++it;
        }
    }

    // Remove destination -> source
    auto &neighbors2 = adj[destination];

    for (auto it = neighbors2.begin(); it != neighbors2.end();) {
        if (it->first == source) {
            it = neighbors2.erase(it);
        } else {
            ++it;
        }
    }
}
// Get all neighbouring locations
vector<pair<int, double>> Graph::getNeighbors(int vertex) {

    if (!hasVertex(vertex)) {
        return {};
    }

    return adj[vertex];
}

// Check whether a location exists
bool Graph::hasVertex(int vertex) {
    return adj.find(vertex) != adj.end();
}

// Check whether a route exists
bool Graph::hasEdge(int source, int destination) {

    if (!hasVertex(source) || !hasVertex(destination)) {
        return false;
    }

    for (const auto &edge : adj[source]) {
        if (edge.first == destination) {
            return true;
        }
    }

    return false;
}

// Get all location IDs
vector<int> Graph::getVertices() {

    vector<int> vertices;

    for (const auto &entry : adj) {
        vertices.push_back(entry.first);
    }

    return vertices;
}

// Get distance between two connected locations
double Graph::getEdgeWeight(int source, int destination) {

    if (!hasVertex(source) || !hasVertex(destination)) {
        return -1;
    }

    for (const auto &edge : adj[source]) {
        if (edge.first == destination) {
            return edge.second;
        }
    }

    return -1;
}