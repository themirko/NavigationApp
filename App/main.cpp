#include <iostream>

#include "include/types.hpp"
#include "include/MapClass.hpp"



int main() {

    Map map{};
    map.loadMap();
    map.printMap();

    return 0;
}