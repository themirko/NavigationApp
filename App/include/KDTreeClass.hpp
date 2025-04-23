#pragma once

#include <iostream>
#include <vector>

#include "types.hpp"

class KDTree {
public:
    KDPtr root;

    KDTree() = default;
    ~KDTree() = default;

    nodePtr findNearestNode(const Degrees latitude, const Degrees longitude);

    void buildTree(std::vector<nodePtr> &nodes);
    void printTree() const;

private:
    std::pair<nodePtr, Kilometers> findNearestNodeRecursive(const Degrees latitude, const Degrees longitude, const KDPtr &currentRoot,
                                                            const long depth, std::pair<nodePtr, Kilometers> currentBestNode);
    KDPtr createNode(const nodePtr &mapNode);

    void buildTreeRecursive(KDPtr &currentRoot, std::vector<nodePtr> &nodes, const long depth);

    static bool compareByLongitude(const nodePtr &n1, const nodePtr &n2);
    static bool compareByLatitude(const nodePtr &n1, const nodePtr &n2);
};