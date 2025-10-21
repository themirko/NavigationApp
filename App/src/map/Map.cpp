#pragma once

#include "../../include/utils/types.hpp"
#include "../../include/map/Map.hpp"
#include "../../include/map/Node.hpp"
#include "../../include/map/Edge.hpp"
#include "../../include/utils/GeoUtils.hpp"
#include "../../include/map/PathDataStruct.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <limits>
#include <chrono>


TransportationMode Map::toTransportationMode(
  const std::string& transportationMode) const {

  std::string tM = transportationMode;
  std::ranges::transform(tM.begin(), tM.end(), tM.begin(), ::toupper);

  if (tM == "DRIVING") return TransportationMode::DRIVING;
  if (tM == "WALKING") return TransportationMode::WALKING;

  std::cerr << "Unknown transportation mode: " << transportationMode
            << std::endl;
  return TransportationMode::UNKNOWN;
}

void Map::loadNodesFromFile() {
  std::ifstream file("../../Parser/OSM/NodesB.txt");

  if (!file) {
    std::cerr << "File does not exist! (loadNodesFromFile)" << std::endl;
    return;
  }

  std::string nodeId;
  Degrees latitude, longitude;

  while (file >> nodeId >> latitude >> longitude) {
    if (this->nodeRegistry.contains(nodeId)) continue;

    this->nodeRegistry[nodeId] =
      std::make_shared<Node>(nodeId, latitude, longitude);
  }

  if (!file.eof()) {
    std::cerr << "Error reading file! (loadNodesFromFile)" << std::endl;
  } else {
    std::cout << "Nodes loaded successfully!" << std::endl;
  }
}

void Map::loadStreet(std::ifstream& file,
                     const std::string& streetName,
                     const std::string& streetId,
                     const std::string& transportationMode,
                     const bool isOneway) {
  std::string currentNodeId, nextNodeId;
  const TransportationMode mode = toTransportationMode(transportationMode);

  if (!(file >> currentNodeId)) return;
  nodePtr currentNode = this->nodeRegistry[currentNodeId];

  while (file >> nextNodeId) {
    if (nextNodeId == "wayId:") return;

    if (!this->nodeRegistry.contains(currentNodeId)) {
      std::cerr << "Node " << currentNodeId << " does not exist!"
                << std::endl;
      currentNode = this->nodeRegistry[nextNodeId];
      continue;
    }

    if (!this->nodeRegistry.contains(nextNodeId)) {
      std::cerr << "Node " << nextNodeId << " does not exist!"
                << std::endl;
      continue;
    }

    nodePtr nextNode = this->nodeRegistry[nextNodeId];
    const Kilometers distance =
      GeoUtils::HaversineDistance(currentNode, nextNode);

    if (!distance || currentNode == nextNode) continue;

    currentNode->addEdge(
      Edge(streetName, streetId, distance, mode, nextNode));

    if (!isOneway)
      nextNode->addEdge(
        Edge(streetName, streetId, distance, mode, currentNode));

    currentNode = nextNode;
    currentNodeId = currentNode->getId();
  }
}

void Map::removeOrphanNodes() {
  if (this->nodeRegistry.empty()) return;

  auto it = std::begin(this->nodeRegistry);
  while (it != std::end(this->nodeRegistry)) {
    !it->second->getEdgesSize()
      ? it = this->nodeRegistry.erase(it)
      : ++it;
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

  std::ifstream file("../../Parser/OSM/WaysB.txt");
  if (!file) {
    std::cerr << "File does not exist! (loadMap)" << std::endl;
    return;
  }

  std::string streetId, streetName, trafficDirection, transportationMode;

  file.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
  while (file >> streetId) {
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(file, streetName);

    if (!(file >> trafficDirection >> transportationMode)) break;
    std::ranges::transform(
      trafficDirection, trafficDirection.begin(), toupper);

    bool isOneway;
    trafficDirection == "ONEWAY" ? isOneway = true : isOneway = false;

    loadStreet(file, streetName, streetId, transportationMode, isOneway);
  }

  if (!file.eof()) {
    std::cerr << "Error reading file! (loadMap)" << std::endl;
  } else {
    std::cout << "Map loaded successfully!" << std::endl;
  }

  this->removeOrphanNodes();
  this->simplifyGraph();

  int idx = 0;
  for (auto& [id, node] : this->nodeRegistry) {
    node->index = idx++;
  }
  this->nodeCount = idx;
  this->buildKDTreeFromRegistry();
}

std::vector<nodePtr> Map::reconstructPath(
  const nodePtr& destinationPoint,
   const std::vector<pathData>& traversalData) {

  std::vector<nodePtr> path;
  nodePtr currentNode = destinationPoint;

  while (currentNode) {
    path.insert(path.begin(), currentNode);
    int parentIndex = traversalData[currentNode->index].getParent()
                      ? traversalData[currentNode->index].getParent()->index
                      : -1;
    if (parentIndex == -1)
      break;
    currentNode = traversalData[currentNode->index].getParent();
  }
  return path;
}

void Map::relaxEdges(
  const nodePtr& currentNode,
  std::vector<pathData>& traversalData,
  std::priority_queue<pathData,
                      std::vector<pathData>,
                      std::greater<>>& availableNodes,
  std::vector<bool>& visitedNodes,
  const TransportationMode transportationMode) {

  int currentIndex = currentNode->index;
  for (const Edge& edge : currentNode->edges) {
    //if (transportationMode != edge.transportationMode) continue;

    const nodePtr neighborNode = edge.getNeighborNode();
    if (!neighborNode) continue;

    int neighborIndex = neighborNode->index;
    if (visitedNodes[neighborIndex]) continue;

    Kilometers tmpDistance =
            traversalData[currentIndex].getDistance() + edge.getDistance();


    if (traversalData[neighborIndex].getDistance() == INF) {
      traversalData[neighborIndex].setData(tmpDistance, currentNode);
      availableNodes.push(pathData(tmpDistance, neighborNode));
      continue;
    }

    if (tmpDistance < traversalData[neighborIndex].getDistance()) {
      traversalData[neighborIndex].setData(tmpDistance, currentNode);
      availableNodes.push(pathData(tmpDistance, neighborNode));
    }
  }
}

std::vector<nodePtr> Map::DijkstraShortestPath(
  const nodePtr& startingPoint,
  const nodePtr& destinationPoint,
  const TransportationMode transportationMode) {

  std::priority_queue<pathData,
                      std::vector<pathData>,
                      std::greater<>> availableNodes;

  std::vector<pathData> traversalData(this->nodeCount);
  std::vector<bool> visitedNodes(this->nodeCount, false);

  traversalData[startingPoint->index].setData(0.0f, nullptr);
  availableNodes.push(pathData(0.0f, startingPoint));

  while (!availableNodes.empty()) {
    const nodePtr currentNode = availableNodes.top().getParent();
    int currentIndex = currentNode->index;
    availableNodes.pop();

    if (visitedNodes[currentIndex]) continue;
    visitedNodes[currentIndex] = true;

    if (currentIndex == destinationPoint->index) break;

    this->relaxEdges(
      currentNode, traversalData, availableNodes,
      visitedNodes, transportationMode);
  }

  return this->reconstructPath(destinationPoint, traversalData);
}

std::vector<nodePtr> Map::findShortestPathToDestination(
  const Degrees startLat,
  const Degrees startLon,
  const Degrees endLat,
  const Degrees endLon,
  const std::string& transportationMode) {

  nodePtr start = tree.findNearestNode(startLat, startLon);
  nodePtr dest = tree.findNearestNode(endLat, endLon);

  if (!start || !dest) {
    std::cerr << "Could not find nearest nodes! "
                 "(findShortestPathToDestination)"
              << std::endl;
    return {};
  }

  return DijkstraShortestPath(
    start, dest, toTransportationMode(transportationMode));
}


void Map::printNumOfNodes() const {
  std::cout << "Number of nodes: " << this->nodeCount << std::endl;
}

void Map::printNumOfEdges() const {
  int edgeCount = 0;

  for (const auto& [id, node] : this->nodeRegistry) {
    edgeCount += node->getEdgesSize();
  }

  std::cout << "Number of edges: " << edgeCount << std::endl;
}


void Map::printKDTree() const {
  this->tree.printTree();
}

std::unordered_map<std::string, nodePtr> Map::getNodeRegistry() const {
  return this->nodeRegistry;
}

void Map::simplifyGraph() {
  std::vector<std::string> toRemove;

  for (const auto& [id, node] : nodeRegistry) {
    if (node->edges.size() != 2) continue;

    const Edge& e1 = node->edges[0];
    const Edge& e2 = node->edges[1];
    if (e1.streetId != e2.streetId) continue;

    nodePtr n1 = e1.getNeighborNode();
    nodePtr n2 = e2.getNeighborNode();

    if (!n1 || !n2) continue;
    if (e1.transportationMode != e2.transportationMode) continue;

    Kilometers dist = e1.getDistance() + e2.getDistance();

    n1->edges.erase(
      std::remove_if(n1->edges.begin(), n1->edges.end(),
                     [&](const Edge& e) { return e.getNeighborNode() == node; }),
      n1->edges.end());

    n2->edges.erase(
      std::remove_if(n2->edges.begin(), n2->edges.end(),
                     [&](const Edge& e) { return e.getNeighborNode() == node; }),
      n2->edges.end());

    n1->addEdge(Edge(e1.streetName,
                     e1.streetId,
                     dist,
                     e1.transportationMode,
                     n2));
    n2->addEdge(Edge(e2.streetName,
                     e2.streetId,
                     dist,
                     e2.transportationMode,
                     n1));

    toRemove.push_back(id);
  }

  for (const auto& id : toRemove)
    nodeRegistry.erase(id);

  for (auto& [id, node] : nodeRegistry) {
    node->edges.erase(
      std::remove_if(node->edges.begin(), node->edges.end(),
                     [&](const Edge& e) {
                       auto n = e.getNeighborNode();
                       return !n || !nodeRegistry.contains(n->getId());
                     }),
      node->edges.end());
  }
}


nodePtr Map::findNearestNode(const Degrees latitude,
                             const Degrees longitude) {
  return this->tree.findNearestNode(latitude, longitude);
}
