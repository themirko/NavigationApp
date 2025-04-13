#pragma once

#include "../include/types.hpp"
#include "../include/MapClass.hpp"
#include "../include/NodeClass.hpp"
#include "../include/EdgeClass.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

TransportationMode Map::toTransportationMode(const std::string &transportationMode) const {

     if (transportationMode == "Driving") return TransportationMode::Driving;
     if (transportationMode == "Walking") return TransportationMode::Walking;

     std::cerr << "Unknown transportation mode: " << transportationMode << std::endl;
     return TransportationMode::Unknown;
}

double Map::calculateDistance(const nodePtr &node1, const nodePtr &node2) const {
     constexpr double DegreesToRadians = M_PI / 180.0;
     constexpr double earthRadiusKilometers = 6371.0;

     const double lat1 = node1->latitude * DegreesToRadians;
     const double lon1 = node1->longitude * DegreesToRadians;

     const double lat2 = node2->latitude * DegreesToRadians;
     const double lon2 = node2->longitude * DegreesToRadians;

     const double deltaLat = lat2 - lat1;
     const double deltaLon = lon2 - lon1;

     const double haversine = std::pow(std::sin(deltaLat / 2), 2) + std::cos(lat1) * std::cos(lat2) * std::pow(std::sin(deltaLon / 2), 2);
     const double angularDistance = 2 * std::asin(std::sqrt(haversine));


     return angularDistance * earthRadiusKilometers;
}


void Map::initializeNodes() {

     std::ifstream file("../../Parser/OSM/Nodes.txt");

     if (!file) {
          std::cerr << "File does not exist! (nodes initialization)" << std::endl;
          return;
     }

     std::string nodeId;
     double latitude, longitude;

     while (file >> nodeId >> latitude >> longitude) {
          if (nodes.contains(nodeId)) {
               std::cerr << "Node " << nodeId << " already exists!" << std::endl;
               continue;
          }

          this->nodes[nodeId] = std::make_shared<Node>(nodeId, latitude, longitude);
     }

     if (!file.eof()) {
          std::cerr << "Error reading file! (nodes initialization)" << std::endl;
     }
     else std::cout << "Nodes initialized successfully!" << std::endl;

     file.close();
}


void Map::createOnewayStreet(std::ifstream &file, const std::string &streetName, const std::string &streetId, const std::string &transportationMode) {
     std::string currentNodeId, nextNodeId;

     if (!(file >> currentNodeId)) return;
     nodePtr currentNode = this->nodes[currentNodeId];

     while (file >> nextNodeId) {
          if (nextNodeId == "wayId:") return;

          if (!this->nodes.contains(currentNodeId)) {
               std::cerr << "Node " << currentNodeId << " does not exist!" << std::endl;

               currentNode = this->nodes[nextNodeId];
               continue;
          }

          if (!this->nodes.contains(nextNodeId)) {
               std::cerr << "Node " << nextNodeId << " does not exist!" << std::endl;
               continue;
          }

          nodePtr nextNode = this->nodes[nextNodeId];
          currentNode->addEdge(Edge(streetName, streetId, calculateDistance(currentNode, nextNode), toTransportationMode(transportationMode), nextNode));

          currentNode = nextNode;
     }

}

void Map::createTwoWayStreet(std::ifstream &file, const std::string &streetName, const std::string &streetId, const std::string &transportationMode) {
     std::string currentNodeId, nextNodeId;

     if (!(file >> currentNodeId)) return;
     nodePtr currentNode = this->nodes[currentNodeId];

     while (file >> nextNodeId) {
          if (nextNodeId == "wayId:") return;

          if (!this->nodes.contains(currentNodeId)) {
               std::cerr << "Node " << currentNodeId << " does not exist!" << std::endl;

               currentNode = this->nodes[nextNodeId];
               continue;
          }

          if (!this->nodes.contains(nextNodeId)) {
               std::cerr << "Node " << nextNodeId << " does not exist!" << std::endl;
               continue;
          }

          nodePtr nextNode = this->nodes[nextNodeId];

          currentNode->addEdge(Edge(streetName, streetId, calculateDistance(currentNode, nextNode), toTransportationMode(transportationMode), nextNode));
          nextNode->addEdge(Edge(streetName, streetId, calculateDistance(currentNode, nextNode), toTransportationMode(transportationMode), currentNode));

          currentNode = nextNode;
     }
}

void Map::loadMap() {

     this->initializeNodes();
     std::ifstream file("../../Parser/OSM/Ways.txt");
     // std::ifstream file("../waysInit.txt");

     if (!file) {
          std::cerr << "File does not exist! (map loading)" << std::endl;
          return;
     }

     std::string streetId, streetName, trafficDirection, transportationMode;

     file.ignore(std::numeric_limits<std::streamsize>::max(), ' '); // ignores "wayId: "
     while (file >> streetId) {

          file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::getline(file, streetName);

          if (!( file >> trafficDirection >> transportationMode)) break;

          if (trafficDirection == "oneway") createOnewayStreet(file, streetName, streetId, transportationMode);
          else createTwoWayStreet(file, streetName, streetId, transportationMode);

     }

     if (!file.eof()) {
          std::cerr << "Error reading file! (map loading)" << std::endl;
     }
     else std::cout << "Map loaded successfully!" << std::endl;

     file.close();
}

void Map::printMap() const {
     std::cout << std::size(this->nodes) << std::endl;
}

void Map::printNodes() const {
     for (const auto &node : this->nodes) {
          node.second->printNode();
     }
}


