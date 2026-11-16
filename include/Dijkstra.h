
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Graph.h"
#include <vector>

struct DijkstraResult
{
    bool pathExists;
    std::vector<int> path;
    double totalDistance;
};

DijkstraResult findShortestPath(const Graph& graph, int source, int destination);

#endif
