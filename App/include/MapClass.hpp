#pragma once

#include <memory>
#include <unordered_map>

#include "types.hpp"

class Map {
    std::unordered_map<std::string, nodePtr> nodes;

    public:
        Map() = default;
        ~Map() = default;


        TransportationMode toTransportationMode(const std::string &transportationMode) const;

        void initializeNodes();
        void createOnewayStreet(std::ifstream &file,
                                  const std::string &streetName,
                                  const std::string &streetId,
                                  const std::string &transportationMode);

        void createTwoWayStreet(std::ifstream &file,
                                  const std::string &streetName,
                                  const std::string &streetId,
                                  const std::string &transportationMode);

        void loadMap();

        double calculateDistance(const nodePtr &node1, const nodePtr &node2) const;

        void printMap() const;
        void printNodes() const;
};