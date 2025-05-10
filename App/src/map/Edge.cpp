#pragma once

#include "../../include/utils/types.hpp"
#include "../../include/map/Edge.hpp"
#include "../../include/map/Node.hpp"

#include <iostream>
#include <memory>



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


