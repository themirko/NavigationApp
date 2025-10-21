#pragma once

#include <string>
#include <memory>
#include "../utils/types.hpp"

class Edge {
public:
    std::string streetName;
    std::string streetId;

    Kilometers distance = 0;

    TransportationMode transportationMode;
    nodePtr nodeNeighbor;

    Edge() = delete;
    Edge(const std::string& streetName,
         const std::string& streetId,
         Kilometers distance,
         TransportationMode transportationMode,
         nodePtr neighbor);

    ~Edge() = default;

    Edge(const Edge& other) = delete;
    Edge(Edge&& other) noexcept = default;

    Edge& operator=(Edge&& other) noexcept = default;
    Edge& operator=(const Edge& other) = delete;

    nodePtr getNeighborNode() const;
    Kilometers getDistance() const;
    std::string getNodeId() const;

    void printEdge() const;
};
