#pragma once

#include "types.hpp"
#include "NodeClass.hpp"

#include <cmath>

namespace GeoUtils {

    inline Kilometers HaversineDistance(const Degrees latitude1, const Degrees longitude1,
                                        const Degrees latitude2, const Degrees longitude2) {
        constexpr Radians DegreesToRadians = M_PI / 180.0;
        constexpr Kilometers earthRadius = 6371.0;

        const Radians lat1 = latitude1 * DegreesToRadians;
        const Radians lon1 = longitude1 * DegreesToRadians;

        const Radians lat2 = latitude2 * DegreesToRadians;
        const Radians lon2 = longitude2 * DegreesToRadians;

        const Radians deltaLat = lat2 - lat1;
        const Radians deltaLon = lon2 - lon1;

        const Radians haversine = std::pow(std::sin(deltaLat / 2), 2) + std::cos(lat1) * std::cos(lat2) * std::pow(std::sin(deltaLon / 2), 2);
        const Radians angularDistance = 2 * std::asin(std::sqrt(haversine));

        return angularDistance * earthRadius;
    }

    inline Kilometers HaversineDistance(const nodePtr &node1, const nodePtr &node2) {
        return HaversineDistance(node1->latitude, node1->longitude, node2->latitude, node2->longitude);
    }

    inline  Kilometers HaversineDistance(const Degrees lat, const Degrees lon, const nodePtr &node) {
        return HaversineDistance(lat, lon, node->latitude, node->longitude);
    }
}
