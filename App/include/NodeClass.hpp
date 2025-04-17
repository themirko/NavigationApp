#pragma once

#include <string>
#include <vector>

#include "types.hpp"

class Node {
public:
    const std::string id;

    float latitude = 0.0;
    float longitude = 0.0;

    std::vector<Edge> edges;

    Node() = delete;
    Node(std::string id);
    Node(std::string id, float latitude, float longitude);

    ~Node() = default;

    void addEdge(Edge edge);
    void printNode() const;

};

