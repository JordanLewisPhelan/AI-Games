#include "Behaviours.h"


sf::Vector2f SeekBehavior::calculateSteering(const sf::Vector2f& t_npcPos,
    const sf::Vector2f& t_npcVel,
    float t_speed) 
{
    // Distance
    // Goal     ==      targetPos(player) - currentPos(npc)
    sf::Vector2f desired = m_context.getTargetPos() - t_npcPos;
    desired = normalize(desired) * t_speed; 
    return truncate(desired - t_npcVel, t_speed);   
}


sf::Vector2f WanderBehavior::calculateSteering(
    const sf::Vector2f& t_npcPos,
    const sf::Vector2f& t_npcVel,
    float t_speed)
{
    // Determine forward direction (use velocity, fallback to right if no movement)
    sf::Vector2f forward = t_npcVel;
    if (vectorLength(forward) < 0.01f) forward = { 1.f, 0.f }; // default right
    forward = normalize(forward);

    // Pick a new target if we don't have one or we've reached prior
    if (!m_hasTarget || vectorLength(m_wanderTarget - t_npcPos) < m_tolerance) {
        // Random angle in cone [-angleAllowance/2, +angleAllowance/2]
        float angleAllowance = 40.f * 3.14159265f / 180.f; 
        float randomOffset = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * angleAllowance;

        float forwardAngle = std::atan2(forward.y, forward.x);
        float targetAngle = forwardAngle + randomOffset;

        m_wanderTarget = t_npcPos + sf::Vector2f(std::cos(targetAngle), std::sin(targetAngle)) * m_wanderRadius;
        m_hasTarget = true;
    }

    // Compute desired velocity toward current target
    sf::Vector2f desired = normalize(m_wanderTarget - t_npcPos) * t_speed;

    // Return steering vector
    return truncate(desired - t_npcVel, t_speed);
}



sf::Vector2f ArriveBehavior::calculateSteering(const sf::Vector2f& t_npcPos,
    const sf::Vector2f& t_npcVel,
    float t_speed) 
{
    // distance
    sf::Vector2f toTarget = m_context.getTargetPos() - t_npcPos;
    float distance = vectorLength(toTarget);
    if (distance < m_stopRadius) return -t_npcVel; // resist velocity when we are too close to target

    // Slow Zone: We slowly begin to slow down as we approach our target, closer we are, the slower we move
    float targetSpeed = (distance < m_slowRadius) ? t_speed * (distance / m_slowRadius) : t_speed;
    sf::Vector2f desired = normalize(toTarget) * targetSpeed;

    return truncate(desired - t_npcVel, t_speed);
}


sf::Vector2f PursueBehavior::calculateSteering(const sf::Vector2f& t_npcPos,
    const sf::Vector2f& t_npcVel,
    float t_speed)
{
    sf::Vector2f playerPos = m_context.getTargetPos();
    sf::Vector2f playerVel = m_context.getTargetVel();

    // distance - could refactor class to add in distance checker?
    sf::Vector2f toPlayer = playerPos - t_npcPos;
    float distance = vectorLength(toPlayer);

    // look-ahead time
    float maxPrediction = 0.5f; // in seconds
    float lookAhead = distance / t_speed;
    lookAhead = std::min(lookAhead, maxPrediction); // restrains prediction to prevent overshooting

    // Predicted position
    sf::Vector2f futurePos = playerPos + playerVel * lookAhead;

    sf::Vector2f desired = normalize(futurePos - t_npcPos) * t_speed;

    return truncate(desired - t_npcVel, t_speed);
}




