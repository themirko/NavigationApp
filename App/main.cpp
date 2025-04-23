#include <iostream>

#include "include/MapClass.hpp"


// 44.87769477055894, 20.666836137919265
int main() {

    Map map{};
    map.loadMap();
    // map.findShortestPathToDestination(44.8727, 20.6497, "Driving");
    map.printMap();

    return 0;
}
