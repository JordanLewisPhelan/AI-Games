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
        float maxSpeed) const = 0;

protected:
    // Returns length/magnitude of a given vector line
    static float vectorLength(const sf::Vector2f& v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    // Normalizes the value of the vector lines magnitude between -1 and 1, good for keeping the code structure consistent
    static sf::Vector2f normalize(const sf::Vector2f& v) {
        float len = vectorLength(v);
        if (len == 0.f) return { 0.f, 0.f };
        return { v.x / len, v.y / len };
    }

    // < Refine Understanding of function purpose >
    static sf::Vector2f truncate(const sf::Vector2f& v, float max) {
        float len = vectorLength(v);
        if (len > max && len > 0.f) {
            return (v / len) * max;
        }
        return v;
    }
};



// SEEK behavior: Move directly toward a target position.
class SeekBehavior : public SteeringBehavior {
public:
    // Constructor - Uses PlayerContext to track player
    SeekBehavior(const PlayerContext& context) : m_context(context) {}

    // Is used to update the target position to reach as the npc
     void setTarget(const sf::Vector2f& target) { m_target = target; }

     // Steering input for a given NPC
     virtual sf::Vector2f calculateSteering(const sf::Vector2f& npcPos,
         const sf::Vector2f& npcVel,
         float maxSpeed) const override;

private:
    sf::Vector2f m_target;
    const PlayerContext& m_context;
};

