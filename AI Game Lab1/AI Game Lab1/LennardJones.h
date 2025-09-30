#pragma once

#include <cmath>
#include <iostream>

struct LennardJonesParams {
    float A;
    float B;
    int n;
    int m;

    LennardJonesParams(float A, float B, int n, int m)
        : A(A), B(B), n(n), m(m) {}
};

class LennardJones {
public:
    explicit LennardJones(const LennardJonesParams& t_params)
        : m_params(t_params)
    {
        m_equilibriumDist = computeEquilibrium();
        m_cutoffDist = 5.5f * m_equilibriumDist;
    }

    float getEquilibrium() const { return m_equilibriumDist; }
    float getCutoff() const { return m_cutoffDist; }

    float movementForce(float t_radius) const;

private:
    LennardJonesParams m_params;
    float m_equilibriumDist;
    float m_cutoffDist;
    // Used to figure out the distance of where Entities should move to.
    float computeEquilibrium(); 
};
