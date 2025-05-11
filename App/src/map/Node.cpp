#pragma once

#include "../../include/utils/types.hpp"
#include "../../include/map/Node.hpp"
#include "../../include/map/Edge.hpp"

#include <iostream>
#include <utility>

Node::Node(std::string id) : id(std::move(id)) {
    edges.reserve(5);
}

Node::Node(std::string id, const Degrees latitude, const Degrees longitude)
    : id(std::move(id)), latitude(latitude), longitude(longitude) {
    edges.reserve(5);
}

void Node::addEdge(Edge edge) {
    this->edges.emplace_back(std::move(edge));
}

void Node::printNode() const {
    std::cout << this->id << " ";
    std::cout << this->latitude << " " <<  this->longitude << std::endl;
}

std::string Node::getId() const {
    return this->id;
}


Degrees Node::getLatitude() const {
    return this->latitude;
}

Degrees Node::getLongitude() const {
    return this->longitude;
}

int Node::getEdgesSize() const {
    return this->edges.size();
}