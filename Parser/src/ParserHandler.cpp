#include "../include/ParserHandler.hpp"
#include "../include/Utilities.hpp"

#include <iostream>
#include <filesystem>
#include <string_view>
#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>


namespace fs = std::filesystem;

namespace OSMParser {

    Handler::Handler() {

        nodeFile.open("C:/Users/Dusan/CLionProjects/NavigationApp/Parser/OSM/Nodes.txt", std::ios::out);
        if (!nodeFile.is_open()) {
            std::cerr << "Error opening Node output file " << std::endl;
            exit(1);
        }

        wayFile.open("C:/Users/Dusan/CLionProjects/NavigationApp/Parser/OSM/Ways.txt", std::ios::out);
        if (!wayFile.is_open()) {
            std::cerr << "Error opening Way output file " << std::endl;
            exit(1);
        }

    }

    Handler::~Handler() {
        if (nodeFile.is_open()) {
            nodeFile.close();
        }

        if (wayFile.is_open()) {
            wayFile.close();
        }
    }

    void Handler::extractHighwayElements(const osmium::Way &way, const std::basic_string_view<char> &type) {
        const std::basic_string_view streetName(way.tags().get_value_by_key("int_name", "unnamed"));

        wayFile << streetName << std::endl;
        wayFile << Utilities::isOnewayOrTwowayStreet(way) << " " << Utilities::determineTransportationMode(type) << std::endl;
        wayFile << "wayId: " << way.id() << std::endl;

        for (const auto &node : way.nodes()) {
            wayFile << node.ref() << std::endl;
        }
    }

    void Handler::node(const osmium::Node& node) {
        nodeFile << node.id() << " ";
        nodeFile << node.location().lat() << " ";
        nodeFile << node.location().lon() << std::endl;
    }

    void Handler::way(const osmium::Way& way) {

        const std::basic_string_view type(way.tags().get_value_by_key("highway", "other"));
        if (Utilities::isExcludedHighwayType(type)) return;

        extractHighwayElements(way, type);
    }
} // OSMParser