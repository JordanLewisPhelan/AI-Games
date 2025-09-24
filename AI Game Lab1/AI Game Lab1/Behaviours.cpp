#include "Behaviours.h"


sf::Vector2f SeekBehavior::calculateSteering(const sf::Vector2f& npcPos,
    const sf::Vector2f& npcVel,
    float maxSpeed) 
{
    // steering calculation
    sf::Vector2f desired = m_context.getPlayerPos() - npcPos;
    desired = normalize(desired) * maxSpeed;
    return truncate(desired - npcVel, maxSpeed);
}


sf::Vector2f WanderBehavior::calculateSteering(
    const sf::Vector2f& npcPos,
    const sf::Vector2f& npcVel,
    float maxSpeed)
{
    // Determine forward direction (use velocity, fallback to right if zero)
    sf::Vector2f forward = npcVel;
    if (vectorLength(forward) < 0.01f) forward = { 1.f, 0.f }; // default right
    forward = normalize(forward);

    // Pick a new target if we don't have one or we've reached it
    if (!m_hasTarget || vectorLength(m_wanderTarget - npcPos) < m_tolerance) {
        // Random angle in [-angleAllowance/2, +angleAllowance/2]
        float angleAllowance = 60.f * 3.14159265f / 180.f; // 60 degrees cone
        float randomOffset = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * angleAllowance;

        float forwardAngle = std::atan2(forward.y, forward.x);
        float targetAngle = forwardAngle + randomOffset;

        m_wanderTarget = npcPos + sf::Vector2f(std::cos(targetAngle), std::sin(targetAngle)) * m_wanderRadius;
        m_hasTarget = true;
    }

    // Compute desired velocity toward current target
    sf::Vector2f desired = normalize(m_wanderTarget - npcPos) * maxSpeed;

    // Return steering vector
    return truncate(desired - npcVel, maxSpeed);
}



sf::Vector2f ArriveBehavior::calculateSteering(const sf::Vector2f& npcPos,
    const sf::Vector2f& npcVel,
    float maxSpeed) 
{
    sf::Vector2f toTarget = m_context.getPlayerPos() - npcPos;
    float distance = vectorLength(toTarget);
    if (distance < m_stopRadius) return -npcVel; // stop

    float targetSpeed = (distance < m_slowRadius) ? maxSpeed * (distance / m_slowRadius) : maxSpeed;
    sf::Vector2f desired = normalize(toTarget) * targetSpeed;

    return truncate(desired - npcVel, maxSpeed);
}


sf::Vector2f PursueBehavior::calculateSteering(const sf::Vector2f& npcPos,
    const sf::Vector2f& npcVel,
    float maxSpeed)
{
    sf::Vector2f playerPos = m_context.getPlayerPos();
    sf::Vector2f playerVel = m_context.getPlayerVel();

    // Vector to the player
    sf::Vector2f toPlayer = playerPos - npcPos;
    float distance = vectorLength(toPlayer);

    // Estimate look-ahead time
    float maxPrediction = 0.5f; // seconds, tweak this to control how far ahead
    float lookAhead = distance / maxSpeed;
    lookAhead = std::min(lookAhead, maxPrediction);

    // Predicted position
    sf::Vector2f futurePos = playerPos + playerVel * lookAhead;

    // Desired velocity toward predicted position
    sf::Vector2f desired = normalize(futurePos - npcPos) * maxSpeed;

    // Steering
    return truncate(desired - npcVel, maxSpeed);
}




