#include "LennardJones.h"

float LennardJones::movementForce(float t_radius) const
{
    if (t_radius > m_cutoffDist)
        return 0.f;

    float repulse = m_params.n * m_params.A / std::pow(t_radius, m_params.n + 1);
    float attract = m_params.m * m_params.B / std::pow(t_radius, m_params.m + 1);

    return repulse - attract;
}

float LennardJones::computeEquilibrium()
{
    if (m_params.n == m_params.m)
        std::cout << "n and m must differ\n";

    float ratio = (m_params.n * m_params.A) / (m_params.m * m_params.B);
    return std::pow(ratio, 1.0f / float(m_params.n - m_params.m));
}
