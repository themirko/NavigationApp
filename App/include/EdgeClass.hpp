#pragma once

#include <string>
#include <memory>

#include "types.hpp"

class Edge {
public:
    const std::string streetName;
    const std::string streetId;

    const Kilometers distance = 0;

    TransportationMode transportationMode;
    nodePtr nodeNeighbor;

    Edge() = delete;
    Edge(const std::string &streetName, const std::string &streetId, Kilometers distance,
         TransportationMode TransportationMode, nodePtr neighbor);

    ~Edge() = default;


    void printEdge() const;
};