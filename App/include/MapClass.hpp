#pragma once

#include <memory>
#include <unordered_map>

#include "types.hpp"
#include "KDTreeClass.hpp"


class Map {
    std::unordered_map<std::string, nodePtr> nodeRegistry;


    TransportationMode toTransportationMode(const std::string &transportationMode) const;

    void createOnewayStreet(std::ifstream &file,
                            const std::string &streetName,
                            const std::string &streetId,
                            const std::string &transportationMode);

    void createTwoWayStreet(std::ifstream &file,
                            const std::string &streetName,
                            const std::string &streetId,
                            const std::string &transportationMode);

public:
    KDTree tree;

    Map() = default;
    ~Map() = default;


    void setupMapData();
    void loadMap();

    void printMap() const;
    void printKDTree() const;
    void printNodes() const;
};