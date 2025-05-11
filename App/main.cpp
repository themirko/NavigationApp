#include <iostream>

#include "include/utils/plotter.hpp"

#include "include/map/Map.hpp"


// 44.87769477055894, 20.666836137919265
int main() {

    Map map{};
    map.loadMap();

    std::vector path = map.findShortestPathToDestination(44.87274293555709, 20.649742759559512, "Driving");
    plotter::mapPlot(map, path);

    return 0;
}
