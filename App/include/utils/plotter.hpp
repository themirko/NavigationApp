#pragma once

#include "../../include/utils/types.hpp"

#include <vector>

namespace plotter {
    void mapPlot(const Map &map, std::vector<nodePtr> &path);
}