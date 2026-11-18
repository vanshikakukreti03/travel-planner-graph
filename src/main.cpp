#ifdef _WIN32
#define NOMINMAX
#include <winsock2.h>
#include <windows.h>
#endif

#include "httplib.h"
#include "nlohmann/json.hpp"

#include "Graph.h"
#include "Traversal.h"
#include "Dijkstra.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>

using namespace std;
using json = nlohmann::json;

struct City {
    int id;
    string name;
    double latitude;
    double longitude;
};

const vector<City> cities = {
    {1, "Delhi", 28.6139, 77.2090},
    {2, "Agra", 27.1767, 78.0081},
    {3, "Chandigarh", 30.7333, 76.7794},
    {4, "Jaipur", 26.9124, 75.7873},
    {5, "Lucknow", 26.8467, 80.9462},
    {6, "Amritsar", 31.6340, 74.8723},
    {7, "Ahmedabad", 23.0225, 72.5714},
    {8, "Varanasi", 25.3176, 82.9739},
    {9, "Mumbai", 19.0760, 72.8777}
};

const City* findCity(const string& name) {
    for (const City& city : cities) {
        string a = city.name;
        string b = name;

        transform(a.begin(), a.end(), a.begin(),
                  [](unsigned char c) { return tolower(c); });

        transform(b.begin(), b.end(), b.begin(),
                  [](unsigned char c) { return tolower(c); });

        if (a == b) {
            return &city;
        }
    }

    return nullptr;
}

const City* findCityById(int id) {
    for (const City& city : cities) {
        if (city.id == id) {
            return &city;
        }
    }
    return nullptr;
}

json convertPathToNames(const vector<int>& path) {
    json result = json::array();

    for (int id : path) {
        const City* city = findCityById(id);

        if (city != nullptr) {
            result.push_back(city->name);
        }
    }

    return result;
}

Graph createGraph() {
    Graph graph;

    graph.addEdge(1, 2, 230);
    graph.addEdge(1, 3, 245);

    graph.addEdge(2, 4, 240);
    graph.addEdge(2, 5, 335);

    graph.addEdge(3, 6, 230);

    graph.addEdge(4, 7, 660);
    graph.addEdge(4, 9, 1150);

    graph.addEdge(5, 8, 315);

    graph.addEdge(7, 9, 530);

    return graph;
}

int main() {
    Graph graph = createGraph();

    httplib::Server server;

    // Return all available cities and their coordinates.
    server.Get("/api/cities", [](const httplib::Request&,
                                 httplib::Response& res) {
        json response = json::array();

        for (const City& city : cities) {
            response.push_back({
                {"id", city.id},
                {"name", city.name},
                {"latitude", city.latitude},
                {"longitude", city.longitude}
            });
        }

        res.set_content(response.dump(), "application/json");
    });

    // Calculate routes and graph traversals.
    server.Post("/api/route", [&graph](const httplib::Request& req,
                                       httplib::Response& res) {
        try {
            json request = json::parse(req.body);

            if (!request.contains("source") ||
                !request.contains("destination") ||
                !request.contains("algorithm") ||
                !request["source"].is_string() ||
                !request["destination"].is_string() ||
                !request["algorithm"].is_string()) {

                res.status = 400;
                res.set_content(
                    R"({"error":"Please provide source, destination and algorithm."})",
                    "application/json"
                );
                return;
            }

            string sourceName = request["source"];
            string destinationName = request["destination"];
            string algorithm = request["algorithm"];

            const City* source = findCity(sourceName);
            const City* destination = findCity(destinationName);

            if (source == nullptr || destination == nullptr) {
                res.status = 400;
                res.set_content(
                    R"({"error":"One or both cities are not available."})",
                    "application/json"
                );
                return;
            }

            json response;
            response["algorithm"] = algorithm;
            response["source"] = source->name;
            response["destination"] = destination->name;

            // DIJKSTRA: shortest-distance route.
            if (algorithm == "dijkstra") {

                vector<int> finalPath;
                double totalDistance = 0.0;

                // Optional stopover.
                string stopoverName = "";

                if (request.contains("stopover") &&
                    request["stopover"].is_string()) {
                    stopoverName = request["stopover"];
                }

                if (!stopoverName.empty()) {
                    const City* stopover = findCity(stopoverName);

                    if (stopover == nullptr) {
                        res.status = 400;
                        res.set_content(
                            R"({"error":"The selected stopover city is not available."})",
                            "application/json"
                        );
                        return;
                    }

                    DijkstraResult firstLeg =
                        findShortestPath(graph, source->id, stopover->id);

                    DijkstraResult secondLeg =
                        findShortestPath(graph, stopover->id, destination->id);

                    if (!firstLeg.pathExists || !secondLeg.pathExists) {
                        response["pathExists"] = false;
                        response["message"] =
                            "No complete route exists through the selected stopover.";

                        res.set_content(response.dump(), "application/json");
                        return;
                    }

                    finalPath = firstLeg.path;

                    // Avoid adding the stopover twice.
                    for (size_t i = 1; i < secondLeg.path.size(); i++) {
                        finalPath.push_back(secondLeg.path[i]);
                    }

                    totalDistance =
                        firstLeg.totalDistance + secondLeg.totalDistance;

                    response["stopover"] = stopover->name;
                }
                else {
                    DijkstraResult result =
                        findShortestPath(graph, source->id, destination->id);

                    if (!result.pathExists) {
                        response["pathExists"] = false;
                        response["message"] = "No route exists.";

                        res.set_content(response.dump(), "application/json");
                        return;
                    }

                    finalPath = result.path;
                    totalDistance = result.totalDistance;
                }

                response["pathExists"] = true;
                response["path"] = convertPathToNames(finalPath);
                response["totalDistance"] = totalDistance;

                // Transport mode affects estimated time and cost,
                // not the shortest-distance route.
                string mode = "car";

                if (request.contains("transportMode") &&
                    request["transportMode"].is_string()) {
                    mode = request["transportMode"];
                }

                double speed = 60.0;
                double costPerKm = 8.0;

                if (mode == "bus") {
                    speed = 45.0;
                    costPerKm = 3.0;
                }
                else if (mode == "bicycle") {
                    speed = 15.0;
                    costPerKm = 0.0;
                }
                else if (mode == "walking") {
                    speed = 5.0;
                    costPerKm = 0.0;
                }
                else if (mode != "car") {
                    res.status = 400;
                    res.set_content(
                        R"({"error":"Invalid transport mode selected."})",
                        "application/json"
                    );
                    return;
                }

                double estimatedTime = totalDistance / speed;
                double estimatedCost = totalDistance * costPerKm;

                response["transportMode"] = mode;
                response["estimatedTimeHours"] = estimatedTime;
                response["estimatedCost"] = estimatedCost;

                // Optional budget check.
                if (request.contains("budget") &&
                    !request["budget"].is_null()) {

                    if (!request["budget"].is_number()) {
                        res.status = 400;
                        res.set_content(
                            R"({"error":"Budget must be a valid number."})",
                            "application/json"
                        );
                        return;
                    }

                    double budget = request["budget"].get<double>();

                    if (!isfinite(budget) || budget < 0) {
                        res.status = 400;
                        res.set_content(
                            R"({"error":"Budget cannot be negative."})",
                            "application/json"
                        );
                        return;
                    }

                    response["budget"] = budget;
                    response["fitsBudget"] = estimatedCost <= budget;
                }
            }

            // BFS: reachable-city exploration order.
            else if (algorithm == "bfs") {
                vector<int> order = bfs(graph, source->id);

                response["traversal"] = convertPathToNames(order);

                response["destinationReached"] =
                    find(order.begin(), order.end(), destination->id)
                    != order.end();
            }

            // DFS: reachable-city exploration order.
            else if (algorithm == "dfs") {
                vector<int> order = dfs(graph, source->id);

                response["traversal"] = convertPathToNames(order);

                response["destinationReached"] =
                    find(order.begin(), order.end(), destination->id)
                    != order.end();
            }

            else {
                res.status = 400;
                res.set_content(
                    R"({"error":"Unknown algorithm selected."})",
                    "application/json"
                );
                return;
            }

            res.set_content(response.dump(), "application/json");
        }
        catch (const exception&) {
            res.status = 400;

            json error;
            error["error"] = "Invalid request.";
            res.set_content(error.dump(), "application/json");
        }
    });

    // Serve the frontend from the same server.
    if (!server.set_mount_point("/", "./frontend")) {
        cerr << "Could not find the frontend folder." << endl;
        return 1;
    }

    cout << "Travel Planner server is running!" << endl;
    cout << "Open http://localhost:8080 in your browser." << endl;

    const char* portEnv = std::getenv("PORT");
    int port = portEnv ? std::stoi(portEnv) : 8080;

    if (!server.listen("0.0.0.0", port)) {
        cerr << "Could not start the server on port 8080." << endl;
        return 1;
    }

    return 0;
}