#pragma once

#include <array>
#include <algorithm>
#include <string_view>
#include <osmium/osm/way.hpp>

namespace OSMParser::Utilities {
    static constexpr std::array<std::basic_string_view<char>, 11> ExcludedHighwayTypes = {
        "aerialway", "bus_guideway", "busway", "construction", "cycleway",
        "escape", "other", "private", "proposed", "raceway", "service"
    }; // keep this sorted (used by binary_search)

    inline bool isExcludedHighwayType(const std::basic_string_view<char> &type) {
        return std::binary_search(std::begin(ExcludedHighwayTypes), std::end(ExcludedHighwayTypes), type);
    }

    static constexpr std::array<std::string_view, 9> PedestrianRoadTypes = {
        "bridleway", "corridor", "crossing", "footway", "path", "pedestrian",
        "sidewalk", "steps", "traffic_island"
    }; // keep this sorted (used by binary_search)


    inline std::basic_string_view<char> determineTransportationMode(const std::basic_string_view<char> &type) {
        if (std::binary_search(std::begin(PedestrianRoadTypes), std::end(PedestrianRoadTypes), type)) {
            return "pedestrian";
        }
        return "vehicles";
    }

    inline std::basic_string_view<char> isOnewayOrTwowayStreet(const osmium::Way &way) {
        const std::basic_string_view oneway(way.tags().get_value_by_key("oneway", "no"));

        if (oneway == "yes") {
            return "oneway";
        }

        return "twoway";
    }
}
