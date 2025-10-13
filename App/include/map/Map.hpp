#pragma once

#include <memory>
#include <unordered_map>
#include <queue>
#include <unordered_set>

#include "../utils/types.hpp"
#include "PathDataStruct.hpp"
#include "KDTree.hpp"

class Map {
    std::unordered_map<std::string, nodePtr> nodeRegistry;
    KDTree tree;

    TransportationMode toTransportationMode(
      const std::string& transportationMode) const;

    void loadStreet(std::ifstream& file,
                    const std::string& streetName,
                    const std::string& streetId,
                    const std::string& transportationMode,
                    const bool isOneway);

    void loadNodesFromFile();
    void removeOrphanNodes();
    void buildKDTreeFromRegistry();

    void relaxEdges(
      const nodePtr& currentNode,
      std::unordered_map<std::string, pathData>& traversalData,
      std::priority_queue<pathData,
                          std::vector<pathData>,
                          std::greater<>>& availableNodes,
      std::unordered_set<std::string>& visitedNodes,
      const TransportationMode transportationMode);

    std::vector<nodePtr> reconstructPath(
      const nodePtr& destinationPoint,
      std::unordered_map<std::string, pathData>& traversalData);

    std::vector<nodePtr> DijkstraShortestPath(
      const nodePtr& startingPoint,
      const nodePtr& destinationPoint,
      const TransportationMode transportationMode);

public:
    Map() = default;
    ~Map() = default;

    void loadMap();

    std::vector<nodePtr> findShortestPathToDestination(
      const Degrees startLat,
      const Degrees startLon,
      const Degrees endLat,
      const Degrees endLon,
      const std::string& transportationMode);

    void printKDTree() const;
    void printNodes() const;

    std::unordered_map<std::string, nodePtr> getNodeRegistry() const;
};
