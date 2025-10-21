#include <iostream>
#include <chrono>

#include "matplotlibcpp.h"
#include "include/map/Map.hpp"
#include "include/utils/geoUtils.hpp"

using namespace std::chrono;
namespace plt = matplotlibcpp;

int main() {
    Map map;
    map.loadMap();

    double startLat = 52.51619296900066;
    double startLon = 13.375329033130757;

    double endLat = 52.515153652600084;
    double endLon = 13.406953707030942;

    auto kdStart = high_resolution_clock::now();
    nodePtr startNode = map.findNearestNode(startLat, startLon);
    nodePtr destNode = map.findNearestNode(endLat, endLon);
    auto kdStop = high_resolution_clock::now();
    double kdDuration =
      duration_cast<microseconds>(kdStop - kdStart).count();
    auto dijStart = high_resolution_clock::now();
    std::vector<nodePtr> path =
      map.DijkstraShortestPath(startNode, destNode,
                               TransportationMode::WALKING);
    auto dijStop = high_resolution_clock::now();
    double dijDuration =
      duration_cast<milliseconds>(dijStop - dijStart).count();

    double totalDistance = 0.0;
    for (size_t i = 1; i < path.size(); ++i)
        totalDistance += GeoUtils::HaversineDistance(path[i - 1], path[i]);

    std::cout << std::fixed << std::setprecision(3)
              << "Destination: " << endLat << ", " << endLon
              << " | Path length: " << totalDistance << " km"
              << " | KD time: " << kdDuration << " μs"
              << " | Dijkstra time: " << dijDuration << " ms"
              << std::endl;

    map.printNumOfNodes();
    map.printNumOfEdges();

    return 0;
}
