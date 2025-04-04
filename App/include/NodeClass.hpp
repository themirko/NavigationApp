#pragma once

#include <string>
#include <vector>

#include "types.hpp"

class Node {

    const std::string id;

    double longitude = 0.0;
    double latitude = 0.0;

    std::vector<Edge> edges;

    public:
        Node() = delete;
        Node(std::string id);
        Node(std::string id, const double &longitude, const double &latitude);

        ~Node() = default;

        void setLatitudeAndLongitude(const double &longitude, const double &latitude);
        void addEdge(Edge neighbor);

        void printNode() const;
};

