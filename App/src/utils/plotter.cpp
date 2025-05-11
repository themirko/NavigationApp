#pragma once

#include "../../include/utils/types.hpp"
#include "../../include/utils/plotter.hpp"
#include "../../include/map/Node.hpp"
#include "../../include/map/Map.hpp"

#include "matplotlibcpp.h"

#include <vector>

namespace plotter {
    namespace plt = matplotlibcpp;

    void mapPlot(const Map &map, std::vector<nodePtr> &path) {

        std::vector<Degrees> lats;
        std::vector<Degrees> lons;

        std::vector<Degrees> pathLats;
        std::vector<Degrees> pathLons;

        for (nodePtr node : path) {
            pathLats.push_back(node->getLatitude());
            pathLons.push_back(node->getLongitude());
        }

        for (const auto& [key, coord] : map.getNodeRegistry()) {
            lats.push_back(coord->latitude);
            lons.push_back(coord->longitude);
        }
        plt::figure_size(800, 600);
        plt::scatter(lons, lats, 5.0);
        plt::plot(pathLons, pathLats, {{"color", "r"}, {"label", "path line"}});

        plt::xlabel("Longitude");
        plt::ylabel("Latitude");
        plt::title("Geographic Plot");
        plt::grid(true);
        plt::show();
    }
}