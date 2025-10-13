#include <iostream>
#include <QApplication>

#include "include/gui/MapWindow.hpp"
#include "include/utils/plotter.hpp"
#include "include/map/Map.hpp"


// 44.87769477055894, 20.666836137919265
int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    Map map{};
    map.loadMap();

    std::vector path = map.findShortestPathToDestination(44.87274293555709,
                                                         20.649742759559512,
                                                         "Driving");
    MapWindow window(&map, path);
    window.show();

    return app.exec();
}
