#include <iostream>

#include "include/utils/plotter.hpp"

#include "include/map/Map.hpp"


// 44.87769477055894, 20.666836137919265
int main() {

    Map map{};
    map.loadMap();
    map.findShortestPathToDestination(44.8727, 20.6497, "Driving");
    plotter::plotMap(map);

    return 0;
}
