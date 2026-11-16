#include "Graph.h"
// Add a new location
void Graph::addVertex(int vertex) {
    if (!hasVertex(vertex)) {
        adj[vertex] = {};
    }
}

// Add or update a route
void Graph::addEdge(int source, int destination, double distance) {

    if (distance < 0) {
        cout << "Distance cannot be negative." << endl;
        return;
    }

    addVertex(source);
    addVertex(destination);

    // Add or update an edge without creating duplicates
    auto addOrUpdate = [](vector<pair<int, double>>& neighbors,
                          int neighbor, double weight) {

        bool found = false;

        for (auto it = neighbors.begin(); it != neighbors.end();) {

            if (it->first == neighbor) {

                if (!found) {
                    it->second = weight;
                    found = true;
                    ++it;
                } else {
                    it = neighbors.erase(it);
                }

            } else {
                ++it;
            }
        }

        if (!found) {
            neighbors.push_back({neighbor, weight});
        }
    };

    addOrUpdate(adj[source], destination, distance);

    // A self-loop should appear only once
    if (source != destination) {
        addOrUpdate(adj[destination], source, distance);
    }
}

// Remove a location
void Graph::removeVertex(int vertex) {

    if (!hasVertex(vertex)) {
        return;
    }

    for (auto& entry : adj) {
        vector<pair<int, double>>& neighbors = entry.second;

        for (auto it = neighbors.begin(); it != neighbors.end();) {
            if (it->first == vertex) {
                it = neighbors.erase(it);
            } else {
                ++it;
            }
        }
    }

    adj.erase(vertex);
}

// Remove a route
void Graph::removeEdge(int source, int destination) {

    if (!hasVertex(source) || !hasVertex(destination)) {
        return;
    }

    auto removeConnection = [](vector<pair<int, double>>& neighbors,
                               int neighbor) {

        for (auto it = neighbors.begin(); it != neighbors.end();) {
            if (it->first == neighbor) {
                it = neighbors.erase(it);
            } else {
                ++it;
            }
        }
    };

    removeConnection(adj[source], destination);

    if (source != destination) {
        removeConnection(adj[destination], source);
    }
}

// Get all neighbouring locations
vector<pair<int, double>> Graph::getNeighbors(int vertex) const {

    auto it = adj.find(vertex);

    if (it == adj.end()) {
        return {};
    }

    return it->second;
}

// Check whether a location exists
bool Graph::hasVertex(int vertex) const {
    return adj.find(vertex) != adj.end();
}

// Check whether a route exists
bool Graph::hasEdge(int source, int destination) const {

    auto it = adj.find(source);

    if (it == adj.end()) {
        return false;
    }

    for (const auto& edge : it->second) {
        if (edge.first == destination) {
            return true;
        }
    }

    return false;
}

// Get all location IDs
vector<int> Graph::getVertices() const {

    vector<int> vertices;

    for (const auto& entry : adj) {
        vertices.push_back(entry.first);
    }

    return vertices;
}

// Get distance between two connected locations
double Graph::getEdgeWeight(int source, int destination) const {

    auto it = adj.find(source);

    if (it == adj.end()) {
        return -1;
    }

    for (const auto& edge : it->second) {
        if (edge.first == destination) {
            return edge.second;
        }
    }

    return -1;
}
