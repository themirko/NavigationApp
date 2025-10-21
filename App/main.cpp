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

    double startLat = 46.102041742112604;
    double startLon = 19.648701918626404;

    std::vector<std::pair<double, double>> destinations = {
        {46.086716390962025, 19.669246221698025},
        {45.949504229099645, 19.631454482067983},
        {45.81245057631571,  19.630975933925974},
        {45.711584910100335, 19.66830963248084},
        {45.54749243825372,  19.78982084093405},
        {45.266446047136,    19.841319252071887},
        {44.816201200988644, 20.446253937894795},
        {43.528957134072606, 21.697464591115068},
        {42.554483482576316, 21.903497442087257}
    };

    std::vector<double> pathLengths;
    std::vector<double> kdTimes;
    std::vector<double> dijTimes;

    for (const auto& [endLat, endLon] : destinations) {
        auto kdStart = high_resolution_clock::now();
        nodePtr startNode = map.findNearestNode(startLat, startLon);
        nodePtr destNode  = map.findNearestNode(endLat, endLon);
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

        if (path.size() <= 1) {
            std::cout << "No path found for destination: "
                      << endLat << ", " << endLon << std::endl;
            continue;
        }

        double totalDistance = 0.0;
        for (size_t i = 1; i < path.size(); ++i)
            totalDistance += GeoUtils::HaversineDistance(path[i - 1], path[i]);

        std::cout << std::fixed << std::setprecision(3)
                  << "Destination: " << endLat << ", " << endLon
                  << " | Path length: " << totalDistance << " km"
                  << " | KD time: " << kdDuration << " μs"
                  << " | Dijkstra time: " << dijDuration << " ms"
                  << std::endl;


        pathLengths.push_back(totalDistance);
        kdTimes.push_back(kdDuration);
        dijTimes.push_back(dijDuration);
    }

    map.printNumOfNodes();
    map.printNumOfEdges();


    plt::figure_size(800, 600);

    plt::figure();
    plt::scatter(pathLengths, kdTimes, 80.0);
    plt::xlabel("Path length (km)");
    plt::ylabel("KD-Tree time (μs)");
    plt::title("Nearest-node (KD-Tree) execution time");
    plt::grid(true);

    plt::figure();
    plt::scatter(pathLengths, dijTimes, 80.0);
    plt::xlabel("Path length (km)");
    plt::ylabel("Dijkstra time (ms)");
    plt::title("Dijkstra execution time vs. path length");
    plt::grid(true);

    plt::show();

    return 0;
}

