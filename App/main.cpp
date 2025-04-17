#include <iostream>

#include "include/GeoUtils.hpp"
#include "include/MapClass.hpp"
#include "include/NodeClass.hpp"


int main() {

    // std::cout << GeoUtils::HaversineDistance(44.888, 20.631,  44.8645, 20.6587) << std::endl;
    Map map{};
    map.setupMapData();
    // map.loadMap();
    // map.printMap();
    // map.printKDTree();
    map.tree.findNearestNode(44.8650, 20.6591)->printNode();
    return 0;
}
