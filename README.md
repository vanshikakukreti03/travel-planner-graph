# Travel Planner Using Graph

## Project Overview

Travel Planner Using Graph is a C++ based Data Structures project developed as part of the 3rd Semester B.Tech CSE curriculum.

The project represents cities as vertices and routes between cities as weighted edges in a graph. It uses graph traversal and shortest-path algorithms to help users explore routes between locations.

## Objectives

- Represent a network of cities using a graph.
- Store connections and distances using an adjacency list.
- Implement Breadth First Search (BFS).
- Implement Depth First Search (DFS).
- Find shortest routes using Dijkstra's Algorithm.
- Use a priority queue for efficient shortest-path processing.
- Provide an interactive menu for users.

## Technologies Used

- Programming Language: C++
- Compiler Standard: C++17
- Data Structure: Weighted Graph
- Graph Representation: Adjacency List
- Algorithms: BFS, DFS and Dijkstra's Algorithm
- Version Control: Git and GitHub

## Project Structure

```text
TravelPlanner/
├── include/
│   ├── Graph.h
│   ├── Traversal.h
│   └── Dijkstra.h
├── src/
│   ├── main.cpp
│   ├── Graph.cpp
│   ├── Traversal.cpp
│   └── Dijkstra.cpp
├── tests/
├── README.md
└── .gitignore
```

## Main Features

1. Display available cities.
2. Explore connected locations using BFS.
3. Explore locations using DFS.
4. Find the shortest route between two cities.
5. Display the route and total distance.
6. Handle invalid inputs and unreachable destinations.

## Compilation and Execution

Compile using a C++17 compiler:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o TravelPlanner
```

Run on Windows PowerShell:

```powershell
.\TravelPlanner.exe
```

## Team Members

- Divyanshi – Graph Creation and Adjacency List
- Medha – BFS and DFS
- Manya – Dijkstra's Algorithm and Priority Queue
- Vanshika – Menu, Input Handling, Integration and Testing



## Future Enhancements

- Travel budget constraints.
- Travel mode preferences.
- External location APIs.
- Graphical user interface.
- Real-time travel information.