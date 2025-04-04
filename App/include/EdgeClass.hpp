#pragma once

#include <string>
#include <memory>

#include "types.hpp"

class Edge {
    const std::string streetName;
    double distance;

    nodePtr neighbor;
    public:

        Edge() = delete;
        Edge(std::string streetName, const double &distance, nodePtr neighbor);

        ~Edge() = default;

        void printEdge() const;
};