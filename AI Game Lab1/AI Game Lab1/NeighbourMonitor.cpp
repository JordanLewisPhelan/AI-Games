#include "NeighbourMonitor.h"

std::vector<int> NeighbourMonitor::findNeighbours(const std::vector<Swarmer>& t_swarmers, int t_index) const
{
    std::vector<int> neighbours;
    for (int j = 0; j < (int)t_swarmers.size(); ++j) 
    {
        if (t_index == j) 
            continue;

        float dx = t_swarmers[j].m_position.x - t_swarmers[t_index].m_position.x;
        float dy = t_swarmers[j].m_position.y - t_swarmers[t_index].m_position.y;
        float distSq = dx * dx + dy * dy;

        if (distSq < m_cutoff * m_cutoff) 
        {
            neighbours.push_back(j);
        }
    }
    return neighbours;
}
