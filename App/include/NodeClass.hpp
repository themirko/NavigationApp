#pragma once

#include <string>
#include <vector>

#include "types.hpp"

class Node {
public:
    const std::string id;

    float longitude = 0.0;
    float latitude = 0.0;

    std::vector<Edge> edges;

    Node() = delete;
    Node(std::string id);
    Node(std::string id, float longitude, float latitude);

    ~Node() = default;

    void addEdge(Edge edge);
    void printNode() const;

};

