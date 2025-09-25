#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Utilities.h"

class NPC;  // declared to avoid circular dependency  errors

// Base class for all steering behaviors
class SteeringBehavior {
public:
    virtual ~SteeringBehavior() = default;

    virtual sf::Vector2f calculateSteering(const sf::Vector2f& npcPos,
        const sf::Vector2f& npcVel,
        float maxSpeed) = 0;

protected:
    // Returns length/magnitude of a given vector line
    static float vectorLength(const sf::Vector2f& v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    // Normalizes the value of the vector lines magnitude between -1 and 1, good for keeping the vector maths consistent
    static sf::Vector2f normalize(const sf::Vector2f& v) {
        float len = vectorLength(v);
        if (len == 0.f) return { 0.f, 0.f };
        return { v.x / len, v.y / len };
    }

    // Caps vector magnitude to a maximum value If vector is too long;
    // - it normalizes then scales to max - Good for preventing overpowering steering forces
    static sf::Vector2f truncate(const sf::Vector2f& v, float max) {
        float len = vectorLength(v);
        if (len > max && len > 0.f) {
            return (v / len) * max;
        }
        return v;
    }
};



// SeekBehavior: Move directly toward a target position.
class SeekBehavior : public SteeringBehavior {
public:
    SeekBehavior(const PlayerContext& context) : m_context(context) {}

    // Is used to update the target position to reach as the npc
     void setTarget(const sf::Vector2f& target) { m_target = target; }

     // Steering input for a given NPC
     virtual sf::Vector2f calculateSteering(const sf::Vector2f& npcPos,
         const sf::Vector2f& npcVel,
         float maxSpeed) override;

private:
    sf::Vector2f m_target;
    const PlayerContext& m_context;
};


// WanderBehavior: Moves towards a random position inside an angle
class WanderBehavior : public SteeringBehavior
{
public:
    WanderBehavior::WanderBehavior(const PlayerContext& context, float wanderRadius = 50.f, float tolerance = 5.f)
        : m_context(context), m_wanderRadius(wanderRadius), m_tolerance(tolerance) {}

    // Non-const so we can update internal target
    sf::Vector2f calculateSteering(const sf::Vector2f& npcPos,
        const sf::Vector2f& npcVel,
        float maxSpeed) override;

private:
    const PlayerContext& m_context; // Reference to player info if needed
    sf::Vector2f m_wanderTarget;    // Current wander target
    bool m_hasTarget = false;
    float m_wanderRadius;           // How far to pick new points
    float m_tolerance;              // Distance to consider target reached
};

// Arrive Behaviour: NPC will approach the player and slow down when reaching too close to the player
class ArriveBehavior : public SteeringBehavior {
public:
    ArriveBehavior(const PlayerContext& context, float slowRadius = 100.f, float stopRadius = 10.f)
    : m_context(context), m_slowRadius(slowRadius), m_stopRadius(stopRadius){}

    virtual sf::Vector2f calculateSteering(const sf::Vector2f& npcPos,
        const sf::Vector2f& npcVel,
        float maxSpeed) override;

private:
    const PlayerContext& m_context;
    float m_slowRadius;
    float m_stopRadius;
};


// PursueBehavior: NPC will predict where the target will go 
class PursueBehavior : public SteeringBehavior {
public:
    PursueBehavior(const PlayerContext& context)
        : m_context(context) {}

    virtual sf::Vector2f calculateSteering(const sf::Vector2f& npcPos,
        const sf::Vector2f& npcVel,
        float maxSpeed) override;

private:
    const PlayerContext& m_context;
};


