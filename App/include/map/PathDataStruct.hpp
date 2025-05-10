#pragma once

#include "../../include/utils/types.hpp"

struct pathData {
    Kilometers distance = INF;
    nodePtr parent = nullptr;

    void setData(const Kilometers distance, const nodePtr &parent) {
        this->distance = distance;
        this->parent = parent;
    }

    bool operator>(const pathData &other) const {
        return this->distance > other.distance;
    }
};