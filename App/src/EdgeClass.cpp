#pragma once

#include "../include/EdgeClass.hpp"

#include <iostream>
#include <utility>
#include <memory>


Edge::Edge(std::string streetName, const double &distance, nodePtr neighbor) :
 streetName(std::move(streetName)), distance(distance), neighbor(std::move(neighbor)) {}


void Edge::printEdge() const {
  std::cout << "Edge: " << streetName << " " << distance << std::endl;
}


