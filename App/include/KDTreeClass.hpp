#pragma once

#include <iostream>
#include <vector>

#include "types.hpp"

class KDTree {
public:
    KDPtr root;

    KDTree() = default;
    ~KDTree() = default;

    nodePtr findNearestNode(const double latitude, const double longitude);

    void buildTree(std::vector<nodePtr> &nodes);
    void printTree() const;

private:
    std::pair<nodePtr, double> findNearestNodeRecursive(const double latitude, const double longitude, const KDPtr &currentRoot,
                                                        const long depth, std::pair<nodePtr, double> currentBestNode);
    KDPtr createNode(const nodePtr &mapNode);

    void buildTreeRecursive(KDPtr &currentRoot, std::vector<nodePtr> &nodes, const long depth);

    static bool compareByLongitude(const nodePtr &n1, const nodePtr &n2);
    static bool compareByLatitude(const nodePtr &n1, const nodePtr &n2);
};