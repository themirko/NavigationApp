#pragma once

#include "../../include/utils/types.hpp"
#include "../../include/map/Map.hpp"
#include "../../include/map/Node.hpp"
#include "../../include/map/Edge.hpp"
#include "../../include/utils/GeoUtils.hpp"
#include "../../include/map/PathDataStruct.hpp"

#include "matplotlibcpp.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <limits>

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
     }

     if (!file.eof()) {
          std::cerr << "Error reading file! (loadNodesFromFile)" << std::endl;
     }
     else std::cout << "Nodes loaded successfully!" << std::endl;

}

void Map::loadStreet(std::ifstream &file,
                     const std::string &streetName,
                     const std::string &streetId,
                     const std::string &transportationMode,
                     const bool isOneway) {

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

          if (!distance || currentNode == nextNode) continue;

          currentNode->addEdge(Edge(streetName, streetId, distance, mode, nextNode));
          if (!isOneway) nextNode->addEdge(Edge(streetName, streetId, distance, mode, currentNode));

          currentNode = nextNode;
          currentNodeId = currentNode->getId();
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
          std::ranges::transform(trafficDirection, trafficDirection.begin(), toupper);

          bool isOneway;
          trafficDirection == "ONEWAY" ? isOneway = true : isOneway = false;

          loadStreet(file, streetName, streetId, transportationMode, isOneway);
     }

     if (!file.eof()) {
          std::cerr << "Error reading file! (loadMap)" << std::endl;
     }
     else std::cout << "Map loaded successfully!" << std::endl;

     this->removeOrphanNodes();
     this->buildKDTreeFromRegistry();
}

std::vector<nodePtr> Map::reconstructPath(const nodePtr &destinationPoint,
                                          std::unordered_map<std::string, pathData> &traversalData) {
     std::vector<nodePtr> path;

     nodePtr currentNode = destinationPoint;
     while (currentNode) {
          path.insert(path.begin(), currentNode);
          currentNode = traversalData[currentNode->getId()].getParent();
     }

     return path;

}

void Map::relaxEdges(const nodePtr &currentNode,
                     std::unordered_map<std::string, pathData> &traversalData,
                     std::priority_queue<pathData, std::vector<pathData>, std::greater<>> &availableNodes,
                     std::unordered_set<std::string> &visitedNodes,
                     const TransportationMode transportationMode) {

     for (const Edge &edge : currentNode->edges) {
          if (transportationMode != edge.transportationMode) continue;

          const nodePtr neighborNode = edge.getNeighborNode();
          if (!neighborNode) continue;

          const std::string edgeNodeId = edge.getNodeId();
          const Kilometers tmpDistance = traversalData[currentNode->getId()].getDistance() + edge.getDistance();

          if (visitedNodes.contains(edgeNodeId)) continue;

          if (traversalData[edgeNodeId].getDistance() == INF) {
               traversalData[edgeNodeId].setData(tmpDistance, currentNode);
               availableNodes.push(pathData(tmpDistance, neighborNode));

               continue;
          }


          if (tmpDistance < traversalData[edgeNodeId].getDistance()) {
               traversalData[edgeNodeId].setData(tmpDistance, currentNode);
               availableNodes.push(pathData(tmpDistance, neighborNode));
          }
     }
}


std::vector<nodePtr> Map::DijkstraShortestPath(const nodePtr &startingPoint,
                                               const nodePtr &destinationPoint,
                                               const TransportationMode transportationMode) {

     std::priority_queue<pathData, std::vector<pathData>, std::greater<>> availableNodes;
     std::unordered_set<std::string> visitedNodes;

     std::unordered_map<std::string, pathData> traversalData;
     traversalData.reserve(this->nodeRegistry.size());

     for (const auto& [key, _] : this->nodeRegistry) {
          traversalData[key] = pathData();
     }

     traversalData[startingPoint->getId()].setData(0.0f, nullptr);
     availableNodes.push(pathData(0.0f, startingPoint));

     while (!availableNodes.empty()) {
          const nodePtr currentNode = availableNodes.top().getParent();
          const std::string currentNodeId = currentNode->getId();

          availableNodes.pop();

          if (visitedNodes.contains(currentNodeId)) continue;
          visitedNodes.insert(currentNodeId);

          if (currentNodeId == destinationPoint->getId()) break;
          this->relaxEdges(currentNode, traversalData, availableNodes, visitedNodes, transportationMode);
     }

     return this->reconstructPath(destinationPoint, traversalData);

}

std::vector<nodePtr> Map::findShortestPathToDestination(const Degrees latitude,
                                                        const Degrees longitude,
                                                        const std::string &transportationMode) {

     const Degrees startingPointLatitude = 44.877338055650206, startingPointLongitude = 20.66571439220806;

     nodePtr startingPoint = tree.findNearestNode(startingPointLatitude, startingPointLongitude);
     nodePtr destinationPoint = tree.findNearestNode(latitude, longitude);

     if (!startingPoint || !destinationPoint) {
          std::cerr << "Could not find nearest nodes! (findShortestPathToDestination)" << std::endl;
          return {};
     }

     return DijkstraShortestPath(startingPoint, destinationPoint, toTransportationMode(transportationMode));
}


void Map::printNodes() const {
     for (const auto &node : this->nodeRegistry) {
          node.second->printNode();
     }
}

void Map::printKDTree() const {
     this->tree.printTree();
}

std::unordered_map<std::string, nodePtr> Map::getNodeRegistry() const {
     return this->nodeRegistry;
}


