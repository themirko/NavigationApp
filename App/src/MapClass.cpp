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
#include <queue>
#include <unordered_set>

struct pathData {
     Kilometers distance = std::numeric_limits<Kilometers>::max();
     nodePtr parent = nullptr;

     void setData(const Kilometers distance, const nodePtr &parent) {
          this->distance = distance;
          this->parent = parent;
     }

     bool operator>(const pathData &other) const {
          return this->distance > other.distance;
     }
};

TransportationMode Map::toTransportationMode(const std::string &transportationMode) const {

     std::string tM = transportationMode;
     std::ranges::transform(tM, tM.begin(), toupper);

     if (tM == "DRIVING") return TransportationMode::DRIVING;
     if (tM == "WALKING") return TransportationMode::WALKING;

     std::cerr << "Unknown transportation mode: " << transportationMode << std::endl;
     return TransportationMode::UNKNOWN;
}

void Map::loadNodesFromFile() {
     std::ifstream file("../../Parser/OSM/Nodes.txt");

     if (!file) {
          std::cerr << "File does not exist! (loadNodesFromFile)" << std::endl;
          return;
     }

     std::string nodeId;
     Degrees latitude, longitude;

     while (file >> nodeId >> latitude >> longitude) {
          if (this->nodeRegistry.contains(nodeId)) continue;

          this->nodeRegistry[nodeId] = std::make_shared<Node>(nodeId, latitude, longitude);
          // nodes.emplace_back(this->nodeRegistry[nodeId]);
     }

     if (!file.eof()) {
          std::cerr << "Error reading file! (loadNodesFromFile)" << std::endl;
     }
     else std::cout << "Nodes loaded successfully!" << std::endl;

}

void Map::loadStreet(std::ifstream &file, const std::string &streetName, const std::string &streetId,
                     const std::string &transportationMode, const bool isOneway) {

     std::string currentNodeId, nextNodeId;
     const TransportationMode mode = toTransportationMode(transportationMode);

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
          const Kilometers distance = GeoUtils::HaversineDistance(currentNode, nextNode);

          currentNode->addEdge(Edge(streetName, streetId, distance, mode, nextNode));
          if (!isOneway) nextNode->addEdge(Edge(streetName, streetId, distance, mode, currentNode));

          currentNode = nextNode;
     }
}

void Map::removeOrphanNodes() {
     if (this->nodeRegistry.empty()) return;

     auto it = std::begin(this->nodeRegistry);
     while (it != std::end(this->nodeRegistry)) {
          !it->second->getEdgesSize() ? it = this->nodeRegistry.erase(it) : ++it;
     }
}

void Map::buildKDTreeFromRegistry() {
     std::vector<nodePtr> nodes;
     nodes.reserve(this->nodeRegistry.size());

     for (const auto& [key, node] : this->nodeRegistry) {
          nodes.emplace_back(node);
     }
     this->tree.buildTree(nodes);
}


void Map::loadMap() {
     this->loadNodesFromFile();

     std::ifstream file("../../Parser/OSM/Ways.txt");

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

          bool isOneway;
          trafficDirection == "oneway" ? isOneway = true : isOneway = false;

          loadStreet(file, streetName, streetId, transportationMode, isOneway);
     }

     if (!file.eof()) {
          std::cerr << "Error reading file! (loadMap)" << std::endl;
     }
     else std::cout << "Map loaded successfully!" << std::endl;

     this->removeOrphanNodes();
     this->buildKDTreeFromRegistry();
}

void Map::printMap() const {
     std::cout << std::size(this->nodeRegistry) << std::endl;

}

void Map::DijkstraShortestPath(const nodePtr &startingPoint, const nodePtr &destinationPoint, const TransportationMode transportationMode) {

     std::priority_queue<pathData, std::vector<pathData>, std::greater<>> availableNodes;
     std::unordered_set<std::string> visitedNodes;

     std::unordered_map<std::string, pathData> traversalData;
     traversalData.reserve(this->nodeRegistry.size());

     for (const auto& [key, _] : this->nodeRegistry) {
          traversalData[key] = pathData();
     }

     traversalData[startingPoint->getId()].distance = 0.0f;
     availableNodes.push(pathData(0.0f, startingPoint));

     while (!availableNodes.empty()) {
          const nodePtr currentNode = availableNodes.top().parent;
          availableNodes.pop();

          if (currentNode->getId() == destinationPoint->getId()) {
               std::cout << "target found" << std::endl;
               break;
          }

          if (visitedNodes.contains(currentNode->getId())) continue;
          visitedNodes.insert(currentNode->getId());

          for (const Edge &edge : currentNode->edges) {
               // if (transportationMode != edge.transportationMode) continue;

               if (traversalData[edge.getNodeId()].distance == std::numeric_limits<float>::max()) {
                    traversalData[edge.getNodeId()].setData(edge.getDistance(), currentNode);
                    availableNodes.push(pathData(edge.getDistance(), edge.getNeighborNode()));

                    continue;
               }

               const Kilometers tmpDistance = traversalData[currentNode->getId()].distance + edge.getDistance();
               if (tmpDistance < traversalData[edge.getNodeId()].distance) {
                    traversalData[edge.getNodeId()].setData(tmpDistance, currentNode);
                    availableNodes.push(pathData(tmpDistance, edge.getNeighborNode()));
               }

          }
     }

}


void Map::findShortestPathToDestination(const Degrees latitude, const Degrees longitude, const std::string &transportationMode) {

     const Degrees startingPointLatitude = 44.8777, startingPointLongitude = 20.6668;

     nodePtr startingPoint = tree.findNearestNode(startingPointLatitude, startingPointLongitude);
     nodePtr destinationPoint = tree.findNearestNode(latitude, longitude);

     if (!startingPoint || !destinationPoint) {
          std::cerr << "Could not find nearest nodes! (findShortestPathToDestination)" << std::endl;
          return;
     }

     DijkstraShortestPath(startingPoint, destinationPoint, toTransportationMode(transportationMode));
}


void Map::printNodes() const {
     for (const auto &node : this->nodeRegistry) {
          node.second->printNode();
     }
}

void Map::printKDTree() const {
     this->tree.printTree();
}


