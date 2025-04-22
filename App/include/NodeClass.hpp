#pragma once

#include <string>
#include <vector>

#include "types.hpp"

class Node {
public:
    const std::string id;

    Degrees latitude = 0.0;
    Degrees longitude = 0.0;

    std::vector<Edge> edges;

    Node() = delete;
    Node(std::string id);
    Node(std::string id, Degrees latitude, Degrees longitude);

    ~Node() = default;

    void addEdge(Edge edge);
    void printNode() const;

};

