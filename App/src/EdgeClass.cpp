#pragma once

#include "../include/EdgeClass.hpp"

#include <iostream>
#include <memory>

#include "../include/NodeClass.hpp"


Edge::Edge(const std::string &streetName, const std::string &streetId, double kmDistance,
           TransportationMode transportationMode, nodePtr neighbor) :
streetName(streetName), streetId(streetId), kmDistance(kmDistance), transportationMode(transportationMode), nodeNeighbor(neighbor) {}


void Edge::printEdge() const {

    std::cout << "Street id: " << this->streetId << std::endl;
    std::cout << "Street name: " << this->streetName << std::endl;

    std::cout << "Distance in kilometers to " << this->nodeNeighbor->id << " is " << kmDistance << std::endl;
}


