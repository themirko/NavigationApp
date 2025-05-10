#pragma once

#include "types.hpp"


namespace GeoUtils {

    Kilometers HaversineDistance(const Degrees latitude1, const Degrees longitude1,
                                 const Degrees latitude2, const Degrees longitude2);

    Kilometers HaversineDistance(const nodePtr &node1, const nodePtr &node2);
    Kilometers HaversineDistance(const Degrees lat, const Degrees lon, const nodePtr &node);
}
