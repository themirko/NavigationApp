#pragma once

#include "../include/NodeClass.hpp"
#include "../include/EdgeClass.hpp"

#include <iostream>
#include <utility>

Node::Node(std::string id) : id(std::move(id)) {
    edges.reserve(5);
}

Node::Node(std::string id, const float longitude, const float latitude)
    : id(std::move(id)), longitude(longitude), latitude(latitude) {
    edges.reserve(5);
}

void Node::addEdge(Edge edgde) {
  this->edges.emplace_back(std::move(edgde));
}

void Node::printNode() const {
    std::cout << this->id << " ";
    std::cout << this->longitude << " " <<  this->latitude << std::endl;
}
