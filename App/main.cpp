#include <iostream>

#include "include/MapClass.hpp"
#include "include/NodeClass.hpp"


int main() {

    Map map{};
    map.setupMapData();
    // map.loadMap();
    // map.printMap();
    // map.printKDTree();
    map.tree.findNearestNode(44.8650, 20.6591)->printNode();
    return 0;
}
