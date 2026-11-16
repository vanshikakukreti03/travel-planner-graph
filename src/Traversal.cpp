#include "Traversal.h"

#include <queue>
#include <unordered_set>

// ---------------- BFS ----------------

std::vector<int> bfs(const Graph& graph, int start) {
    std::vector<int> order;

    // Invalid starting location (this also covers an empty graph)
    if (!graph.hasVertex(start)) {
        return order;
    }

    std::unordered_set<int> visited;
    std::queue<int> q;

    // Mark the start as visited when it is added to the queue,
    // so no location is ever added to the queue twice.
    visited.insert(start);
    q.push(start);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        order.push_back(current);

        // Each neighbour is a pair: (neighbour ID, distance)
        for (const auto& edge : graph.getNeighbors(current)) {
            int neighbor = edge.first;

            if (visited.count(neighbor) == 0) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    return order;
}

// ---------------- DFS ----------------

// Helper used by dfs(): visits 'current', then each unvisited neighbour.
static void dfsVisit(const Graph& graph, int current,
                     std::unordered_set<int>& visited,
                     std::vector<int>& order) {
    visited.insert(current);
    order.push_back(current);

    for (const auto& edge : graph.getNeighbors(current)) {
        int neighbor = edge.first;

        if (visited.count(neighbor) == 0) {
            dfsVisit(graph, neighbor, visited, order);
        }
    }
}

std::vector<int> dfs(const Graph& graph, int start) {
    std::vector<int> order;

    // Invalid starting location (this also covers an empty graph)
    if (!graph.hasVertex(start)) {
        return order;
    }

    std::unordered_set<int> visited;
    dfsVisit(graph, start, visited, order);

    return order;
}
