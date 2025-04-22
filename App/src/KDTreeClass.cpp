#pragma once

#include "../include/KDTreeClass.hpp"
#include "../include/NodeClass.hpp"
#include "../include/GeoUtils.hpp"

#include <algorithm>
#include <queue>
#include <utility>
#include <climits>

bool KDTree::compareByLatitude(const nodePtr &n1, const nodePtr &n2) {
    return n1->latitude < n2->latitude;
}

bool KDTree::compareByLongitude(const nodePtr &n1, const nodePtr &n2) {
    return n1->longitude < n2->longitude;
}

KDPtr KDTree::createNode(const nodePtr &mapNode) {
    return std::make_shared<KDNode>(mapNode);
}

void KDTree::buildTreeRecursive( KDPtr &currentRoot, std::vector<nodePtr> &nodes, const long depth ) {

    if (nodes.empty()) return;

    if (nodes.size() == 1) {
        currentRoot = createNode(nodes[0]);
        return;
    }

    if (depth % 2 == 0)  std::sort(nodes.begin(), nodes.end(), compareByLatitude);
    else std::sort(nodes.begin(), nodes.end(), compareByLongitude);

    const size_t medianSize = nodes.size() / 2;
    currentRoot = createNode(nodes[medianSize]);

    std::vector leftNodes(nodes.begin(), nodes.begin() + medianSize);
    std::vector rightNodes(nodes.begin() + medianSize + 1, nodes.end());

    buildTreeRecursive(currentRoot->left, leftNodes, depth + 1);
    buildTreeRecursive(currentRoot->right, rightNodes, depth + 1);

}

void KDTree::buildTree(std::vector<nodePtr> &nodes) {

    this->buildTreeRecursive(this->root, nodes, 0);
    std::cout << "Tree built successfully!" << std::endl;
}

void KDTree::printTree() const {

    if (this->root == nullptr) return;

    std::queue<KDPtr> q;
    q.push(this->root);

    while (!q.empty()) {
        const KDPtr tmp = q.front();
        q.pop();

        std::cout << tmp->parent->id << std::endl;
        if (tmp->left != nullptr) q.push(tmp->left);
        if (tmp->right != nullptr) q.push(tmp->right);
    }

}

std::pair<nodePtr, Kilometers> KDTree::findNearestNodeRecursive(const Degrees latitude, const Degrees longitude, const KDPtr &currentRoot,
                                                            const long depth, std::pair<nodePtr, Kilometers> currentBestNode) {

    if (currentRoot == nullptr) return currentBestNode;

    Kilometers currentDistance = GeoUtils::HaversineDistance(latitude, longitude, currentRoot->parent);
    std::pair<nodePtr, Kilometers> bestNode = std::make_pair(currentRoot->parent, currentDistance);

    if (currentBestNode.first && currentBestNode.second < currentDistance)  {
        bestNode = currentBestNode;
    }

    if (!currentRoot->left && !currentRoot->right || currentDistance < 1e-9) return bestNode;

    Degrees parentAxis, targetAxis;
    Kilometers axisDistance;

    if (!(depth % 2)) {
        parentAxis = currentRoot->parent->latitude;
        targetAxis = latitude;
        axisDistance = GeoUtils::HaversineDistance(parentAxis, 0, targetAxis, 0);
    }
    else {
        parentAxis = currentRoot->parent->longitude;
        targetAxis = longitude;
        axisDistance = GeoUtils::HaversineDistance(0, parentAxis, 0, targetAxis);
    }

    const KDPtr exploreCloser = parentAxis < targetAxis ? currentRoot->right : currentRoot->left;
    const KDPtr exploreFurther = parentAxis < targetAxis ? currentRoot->left : currentRoot->right;

    bestNode = findNearestNodeRecursive(latitude, longitude, exploreCloser, depth + 1, bestNode);
    if (bestNode.second > axisDistance) {
        bestNode = findNearestNodeRecursive(latitude, longitude, exploreFurther, depth + 1, bestNode);
    }

    return bestNode;
}

nodePtr KDTree::findNearestNode(const Degrees latitude, const Degrees longitude) {
    return findNearestNodeRecursive(latitude, longitude, this->root, 0, {}).first;
}
