#include "SwarmSystem.h"

SwarmSystem::SwarmSystem(int t_count, const LennardJonesParams& t_params)
    : m_lenJones(t_params), m_monitor(m_lenJones.getCutoff())
{
    float r0 = m_lenJones.getEquilibrium();
    float visualRadius = 4.f * r0;
    A = t_params.A;     B = t_params.B;
    n = t_params.n;     m = t_params.m;

    // Spawn swarmers anywhere in a broad area
    float spawnMinX = 100.f, spawnMaxX = 700.f;
    float spawnMinY = 100.f, spawnMaxY = 500.f;

    for (int i = 0; i < t_count; ++i) 
    {
        float x = spawnMinX + static_cast<float>(rand()) / RAND_MAX * (spawnMaxX - spawnMinX);
        float y = spawnMinY + static_cast<float>(rand()) / RAND_MAX * (spawnMaxY - spawnMinY);

        m_swarm.emplace_back(sf::Vector2f(x, y), visualRadius);
    }
}

void SwarmSystem::update()
{
    float speedScalar = 1.f;
    float damping = 0.93f;
    float maxForce = 1.5f;
    float cohesionFactor = 0.02f; // Converging force weight

    // Compute swarm center
    sf::Vector2f swarmCenter(0.f, 0.f);
    for (auto& s : m_swarm)
        swarmCenter += s.m_position;

    swarmCenter /= (float)m_swarm.size();

    // Update each swarmer
    for (int i = 0; i < (int)m_swarm.size(); ++i) 
    {
        auto neighbours = m_monitor.findNeighbours(m_swarm, i);
        sf::Vector2f totalForce(0.f, 0.f);

        // Lennard-Jones local interaction using parameters
        for (int j : neighbours) 
        {
            sf::Vector2f dir = m_swarm[j].m_position - m_swarm[i].m_position;
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (dist > 0.f) {
                // Classical LJ force
                float f = -n * A / std::pow(dist, n + 1) + m * B / std::pow(dist, m + 1);

                // Cap the per-neighbor force
                if (f > maxForce) f = maxForce;
                if (f < -maxForce) f = -maxForce;

                totalForce += (f / dist) * dir;
            }
        }

        // Converging force toward swarm center
        sf::Vector2f cohesion = (swarmCenter - m_swarm[i].m_position) * cohesionFactor;
        totalForce += cohesion;

        // Apply velocity update
        m_swarm[i].m_velocity += speedScalar * totalForce;
        m_swarm[i].m_velocity *= damping;
        m_swarm[i].m_position += m_swarm[i].m_velocity * m_swarm[i].m_speed;
        m_swarm[i].m_body.setPosition(m_swarm[i].m_position);
    }
}

void SwarmSystem::render(sf::RenderWindow& t_window) 
{
    for (auto& swarmer : m_swarm) 
    {
        t_window.draw(swarmer.m_body);
    }
}
