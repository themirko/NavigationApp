#pragma once

#include "../include/EdgeClass.hpp"

#include <iostream>
#include <memory>

#include "../include/NodeClass.hpp"


Edge::Edge(const std::string &streetName, const std::string &streetId, Kilometers distance,
           TransportationMode transportationMode, nodePtr neighbor) : streetName(streetName),
                                                                      streetId(streetId),
                                                                      distance(distance),
                                                                      transportationMode(transportationMode),
                                                                      nodeNeighbor(neighbor) {}


std::string Edge::getNodeId() const {
    return nodeNeighbor->getId();
}

Kilometers Edge::getDistance() const {
    return this->distance;
}

nodePtr Edge::getNeighborNode() const {
    return this->nodeNeighbor;
}

void Edge::printEdge() const {
    std::cout << "Street id: " << this->streetId << std::endl;
    std::cout << "Street name: " << this->streetName << std::endl;

    std::cout << "Distance to node " << this->nodeNeighbor->id << " is " << distance << std::endl;
}


