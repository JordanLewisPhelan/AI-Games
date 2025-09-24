#include "Behaviours.h"


sf::Vector2f SeekBehavior::calculateSteering(const sf::Vector2f& npcPos,
    const sf::Vector2f& npcVel,
    float maxSpeed) const
{
    // steering calculation
    sf::Vector2f desired = m_context.getPlayerPos() - npcPos;
    desired = normalize(desired) * maxSpeed;
    return truncate(desired - npcVel, maxSpeed);
}



