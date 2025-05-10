#pragma once

#include "../../include/utils/types.hpp"
#include "../../include/utils/plotter.hpp"
#include "../../include/map/Node.hpp"
#include "../../include/map/Map.hpp"

#include "matplotlibcpp.h"

namespace plotter {
    namespace plt = matplotlibcpp;

    void plotMap(const Map &map) {

        std::vector<double> lats;
        std::vector<double> lons;

        std::vector single_lat = {44.87769477055894};
        std::vector single_lon = {20.666836137919265};

        for (const auto& [key, coord] : map.getNodeRegistry()) {
            lats.push_back(coord->latitude);
            lons.push_back(coord->longitude);
        }
        plt::figure_size(800, 600);
        plt::scatter(lons, lats, 5.0);
        plt::scatter(single_lon, single_lat, 5.0, {{"color", "r"}});

        plt::xlabel("Longitude");
        plt::ylabel("Latitude");
        plt::title("Geographic Plot");
        plt::grid(true);
        plt::show();
    }
}