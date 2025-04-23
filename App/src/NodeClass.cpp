#pragma once

#include "../include/NodeClass.hpp"
#include "../include/EdgeClass.hpp"

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

int Node::getEdgesSize() const {
    return this->edges.size();
}