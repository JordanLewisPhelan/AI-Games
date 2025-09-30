#pragma once

#include <vector>
#include "Swarmers.h"

class NeighbourMonitor {
public:
    explicit NeighbourMonitor(float t_cutoff) : m_cutoff(t_cutoff) {}

    std::vector<int> findNeighbours(const std::vector<Swarmer>& t_swarmers, int t_index) const;

private:
    float m_cutoff;
};
