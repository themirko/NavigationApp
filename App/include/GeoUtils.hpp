#pragma once

#include "types.hpp"
#include "NodeClass.hpp"

#include <cmath>

namespace GeoUtils {

    inline double HaversineDistance(const double latitude1, const double longitude1,
                                    const double latitude2, const double longitude2) {
        constexpr double DegreesToRadians = M_PI / 180.0;
        constexpr double earthRadiusKilometers = 6371.0;

        const double lat1 = latitude1 * DegreesToRadians;
        const double lon1 = longitude1 * DegreesToRadians;

        const double lat2 = latitude2 * DegreesToRadians;
        const double lon2 = longitude2 * DegreesToRadians;

        const double deltaLat = lat2 - lat1;
        const double deltaLon = lon2 - lon1;

        const double haversine = std::pow(std::sin(deltaLat / 2), 2) + std::cos(lat1) * std::cos(lat2) * std::pow(std::sin(deltaLon / 2), 2);
        const double angularDistance = 2 * std::asin(std::sqrt(haversine));

        return angularDistance * earthRadiusKilometers;
    }

    inline double HaversineDistance(const nodePtr &node1, const nodePtr &node2) {
        return HaversineDistance(node1->latitude, node1->longitude, node2->latitude, node2->longitude);
    }

    inline  double HaversineDistance(const double lat, const double lon, nodePtr &node) {
        return HaversineDistance(lat, lon, node->latitude, node->longitude);
    }
}
