#pragma once

#include "../include/NodeClass.hpp"
#include "../include/EdgeClass.hpp"

#include <iostream>
#include <utility>

Node::Node(std::string id) : id(std::move(id)) {}

Node::Node(std::string id, const double &longitude, const double &latitude)
    : id(std::move(id)), longitude(longitude), latitude(latitude) {}

void Node::setLatitudeAndLongitude(const double &longitude, const double &latitude) {
    this->longitude = longitude;
    this->latitude = latitude;
}

void Node::addEdge(Edge neighbor) {
  this->edges.emplace_back(std::move(neighbor));
}

void Node::printNode() const {
    std::cout << this->id << std::endl;
    this->edges[0].printEdge();
}
