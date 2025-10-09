#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "BoundaryManager.h"
#include "Player.h"
#include "Transform.h"


struct MovementContext
{
    const Player* m_player = nullptr;
    const Transform* m_target = nullptr;

	// Constructors for different contexts
    MovementContext(const Player& player)
        : m_player(&player) {}

    MovementContext(const Transform& target)
        : m_target(&target) {}

    MovementContext(const Player& player, const Transform& target)
        : m_player(&player), m_target(&target) {}


    // Accessor functions
    sf::Vector2f getTargetPos() const
    {
        if (m_target)
        {
            getTargetVel();
            return m_target->getPosition();
        }

        if (m_player)
            return m_player->getPosition();
        return {};
    }

    sf::Vector2f getTargetVel() const  { return m_player->getVelocity(); }
};


