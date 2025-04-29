#pragma once

#include <memory>
#include <unordered_map>

#include "types.hpp"
#include "KDTreeClass.hpp"


class Map {
    std::unordered_map<std::string, nodePtr> nodeRegistry;
    KDTree tree;


    TransportationMode toTransportationMode(const std::string &transportationMode) const;

    void loadStreet(std::ifstream &file, const std::string &streetName, const std::string &streetId,
                    const std::string &transportationMode, const bool isOneway);

    void loadNodesFromFile();
    void removeOrphanNodes();
    void buildKDTreeFromRegistry();

    void DijkstraShortestPath(const nodePtr &startingPoint, const nodePtr &destinationPoint,
                              const TransportationMode transportationMode);

public:

    Map() = default;
    ~Map() = default;


    void loadMap();
    void findShortestPathToDestination(const Degrees latitude, const Degrees longitude, const std::string &transportationMode);

    void plotMap() const;
    void printKDTree() const;
    void printNodes() const;
};