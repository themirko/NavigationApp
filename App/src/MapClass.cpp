#pragma once

#include "../include/types.hpp"
#include "../include/MapClass.hpp"
#include "../include/NodeClass.hpp"
#include "../include/EdgeClass.hpp"
#include "../include/GeoUtils.hpp"

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


void Map::setupMapData() {

     std::vector<nodePtr> nodes;

     // std::ifstream file("../../Parser/OSM/Nodes.txt");
     std::ifstream file("../nodesINIT.txt");

     if (!file) {
          std::cerr << "File does not exist! (setupMapData)" << std::endl;
          return;
     }

     std::string nodeId;
     Degrees latitude, longitude;

     while (file >> nodeId >> latitude >> longitude) {
          if (this->nodeRegistry.contains(nodeId)) continue;

          this->nodeRegistry[nodeId] = std::make_shared<Node>(nodeId, latitude, longitude);
          nodes.emplace_back(this->nodeRegistry[nodeId]);
     }

     if (!file.eof()) {
          std::cerr << "Error reading file! (setupMapData)" << std::endl;
     }
     else std::cout << "Nodes initialized successfully!" << std::endl;
     file.close();

     this->tree.buildTree(nodes);
}


void Map::createOnewayStreet(std::ifstream &file, const std::string &streetName, const std::string &streetId, const std::string &transportationMode) {
     std::string currentNodeId, nextNodeId;

     if (!(file >> currentNodeId)) return;
     nodePtr currentNode = this->nodeRegistry[currentNodeId];

     while (file >> nextNodeId) {
          if (nextNodeId == "wayId:") return;

          if (!this->nodeRegistry.contains(currentNodeId)) {
               std::cerr << "Node " << currentNodeId << " does not exist!" << std::endl;

               currentNode = this->nodeRegistry[nextNodeId];
               continue;
          }

          if (!this->nodeRegistry.contains(nextNodeId)) {
               std::cerr << "Node " << nextNodeId << " does not exist!" << std::endl;
               continue;
          }

          nodePtr nextNode = this->nodeRegistry[nextNodeId];
          currentNode->addEdge(Edge(streetName, streetId, GeoUtils::HaversineDistance(currentNode, nextNode), toTransportationMode(transportationMode), nextNode));

          currentNode = nextNode;
     }

}

void Map::createTwoWayStreet(std::ifstream &file, const std::string &streetName, const std::string &streetId, const std::string &transportationMode) {
     std::string currentNodeId, nextNodeId;

     if (!(file >> currentNodeId)) return;
     nodePtr currentNode = this->nodeRegistry[currentNodeId];

     while (file >> nextNodeId) {
          if (nextNodeId == "wayId:") return;

          if (!this->nodeRegistry.contains(currentNodeId)) {
               std::cerr << "Node " << currentNodeId << " does not exist!" << std::endl;

               currentNode = this->nodeRegistry[nextNodeId];
               continue;
          }

          if (!this->nodeRegistry.contains(nextNodeId)) {
               std::cerr << "Node " << nextNodeId << " does not exist!" << std::endl;
               continue;
          }

          nodePtr nextNode = this->nodeRegistry[nextNodeId];

          currentNode->addEdge(Edge(streetName, streetId, GeoUtils::HaversineDistance(currentNode, nextNode), toTransportationMode(transportationMode), nextNode));
          nextNode->addEdge(Edge(streetName, streetId, GeoUtils::HaversineDistance(currentNode, nextNode), toTransportationMode(transportationMode), currentNode));

          currentNode = nextNode;
     }
}

void Map::loadMap() {

     this->setupMapData();
     std::ifstream file("../../Parser/OSM/Ways.txt");
     // std::ifstream file("../waysInit.txt");

     if (!file) {
          std::cerr << "File does not exist! (loadMap)" << std::endl;
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
          std::cerr << "Error reading file! (loadMap)" << std::endl;
     }
     else std::cout << "Map loaded successfully!" << std::endl;

     file.close();
}

void Map::printMap() const {
     std::cout << std::size(this->nodeRegistry) << std::endl;
}

void Map::printNodes() const {
     for (const auto &node : this->nodeRegistry) {
          node.second->printNode();
     }
}

void Map::printKDTree() const {
     this->tree.printTree();
}


