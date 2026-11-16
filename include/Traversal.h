#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include <vector>

#include "Graph.h"

// Breadth First Search (BFS)
// Visits locations level by level, starting from 'start', using a queue.
// Returns the locations in the order they were visited.
// Returns an empty vector if 'start' is not in the graph.
std::vector<int> bfs(const Graph& graph, int start);

// Depth First Search (DFS)
// Goes as deep as possible along each route before backtracking (recursive).
// Returns the locations in the order they were visited.
// Returns an empty vector if 'start' is not in the graph.
std::vector<int> dfs(const Graph& graph, int start);

#endif
