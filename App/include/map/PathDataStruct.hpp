#pragma once

#include "../../include/utils/types.hpp"

struct pathData {
    Kilometers distance = INF;
    nodePtr parent = nullptr;

    void setData(const Kilometers distance, const nodePtr &parent);

    nodePtr getParent() const;
    Kilometers getDistance() const;

    bool operator>(const pathData &other) const;
};