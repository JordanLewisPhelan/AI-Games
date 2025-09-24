#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "BoundaryManager.h"
#include "Player.h"

struct PlayerContext 
{
   const Player& player;

    // Constructor takes references to the actual Player data
    PlayerContext(const Player& p) : player(p) {}

    const sf::Vector2f& getPlayerPos() const { return player.getPosition(); }
    const sf::Vector2f& getPlayerVel() const { return player.getVelocity(); }

};


// Vision cone utility class for NPCs - AI Generated
struct VisionCone {
    float angle = 60.0f;    // degrees - half angle on each side
    float range = 100.0f;   // max vision distance

    // Check if a point is within the vision cone
    bool IsPointInCone(const sf::Vector2f& from, const sf::Vector2f& target, const sf::Vector2f& forward) const {
        sf::Vector2f toTarget = target - from;
        float distance = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

        // Check distance first (cheaper than angle calculation)
        if (distance > range || distance < 0.1f) {
            return false;
        }

        // Normalize vectors for angle calculation
        toTarget.x /= distance;
        toTarget.y /= distance;

        float forwardLength = std::sqrt(forward.x * forward.x + forward.y * forward.y);
        if (forwardLength < 0.1f) return true; // If no forward direction, everything is visible

        sf::Vector2f normalizedForward(forward.x / forwardLength, forward.y / forwardLength);

        // Calculate dot product to get cosine of angle
        float dot = normalizedForward.x * toTarget.x + normalizedForward.y * toTarget.y;
        float halfAngleRad = angle * 3.14159f / 360.0f; // Convert half-angle to radians

        return dot >= std::cos(halfAngleRad);
    }

    // Get a random point within the vision cone (for wander behavior)
    sf::Vector2f GetRandomPointInCone(const sf::Vector2f& from, const sf::Vector2f& forward,
        float distance = -1.0f) const {
        if (distance < 0) distance = range * 0.7f; // Default to 70% of range

        // Get forward direction angle
        float forwardAngle = std::atan2(forward.y, forward.x);

        // Random angle within cone
        float halfAngleRad = angle * 3.14159f / 360.0f;
        float randomAngle = forwardAngle + ((static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f) * halfAngleRad;

        // Create point at specified distance in random cone direction
        return sf::Vector2f(
            from.x + std::cos(randomAngle) * distance,
            from.y + std::sin(randomAngle) * distance
        );
    }

    // Visual debugging - get cone edge points for drawing
    std::pair<sf::Vector2f, sf::Vector2f> GetConeEdges(const sf::Vector2f& from, const sf::Vector2f& forward) const {
        float forwardAngle = std::atan2(forward.y, forward.x);
        float halfAngleRad = angle * 3.14159f / 360.0f;

        float leftAngle = forwardAngle - halfAngleRad;
        float rightAngle = forwardAngle + halfAngleRad;

        sf::Vector2f leftEdge(
            from.x + std::cos(leftAngle) * range,
            from.y + std::sin(leftAngle) * range
        );

        sf::Vector2f rightEdge(
            from.x + std::cos(rightAngle) * range,
            from.y + std::sin(rightAngle) * range
        );

        return std::make_pair(leftEdge, rightEdge);
    }
};
