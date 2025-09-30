#pragma once
#include <vector>
#include <random>
#include "Swarmers.h"
#include "LennardJones.h"
#include "NeighbourMonitor.h"

class SwarmSystem {
public:
    SwarmSystem(int t_count, const LennardJonesParams& t_params);

    void update();
    void render(sf::RenderWindow& t_window);

private:
    float A, B, n, m;   // n and m are floats for calculations

    std::vector<Swarmer> m_swarm;
    LennardJones m_lenJones;
    NeighbourMonitor m_monitor;
};
