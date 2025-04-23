#pragma once

#include <memory>

enum class TransportationMode {
    DRIVING,
    WALKING,
    UNKNOWN
};

struct KDNode;

class Node;
class Edge;
class KDTree;

using nodePtr = std::shared_ptr<Node>;
using KDPtr = std::shared_ptr<KDNode>;

using Kilometers = float;
using Degrees = float;
using Radians = float;

struct KDNode {
    nodePtr parent = nullptr;

    KDPtr left = nullptr;
    KDPtr right = nullptr;

    KDNode(const nodePtr &node) : parent(node) {}
};


