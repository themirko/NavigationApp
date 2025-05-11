#pragma once

#include "../../include/utils/types.hpp"
#include "../../include/map/PathDataStruct.hpp"

void pathData::setData(const Kilometers distance, const nodePtr &parent) {
    this->distance = distance;
    this->parent = parent;
}

nodePtr pathData::getParent() const{
    return parent;
}

Kilometers pathData::getDistance() const {
    return distance;
}

bool pathData::operator>(const pathData &other) const {
    return this->distance > other.distance;
}
