#pragma once

#include "../include/EdgeClass.hpp"

#include <iostream>
#include <memory>


Edge::Edge(const std::string &streetName, const std::string &streetId, double kmDistance,
           TransportationMode transportationMode, nodePtr neighbor) :
streetName(streetName), streetId(streetId), kmDistance(kmDistance), transportationMode(transportationMode), nodeNeighbor(neighbor) {}


void Edge::printEdge() const {
  std::cout << "Edge: " << streetName << " " << kmDistance << std::endl;
}


