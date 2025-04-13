#pragma once

#include <memory>

enum class TransportationMode {
    Driving,
    Walking,
    Unknown
};

class Node;
class Edge;

using nodePtr = std::shared_ptr<Node>;