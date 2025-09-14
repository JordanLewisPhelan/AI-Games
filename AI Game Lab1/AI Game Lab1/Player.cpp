#include "Player.h"
#include "BoundaryManager.h" 
#include <cmath>

Player::Player(sf::Vector2f startPos)
    : m_position(startPos), m_speed(75.0f),        
    m_acceleration(300.0f), // How fast we accelerate to target speed
    m_deceleration(200.0f)  // How fast we slow down when stopping
{
    m_velocity = sf::Vector2f(0.0f, 0.0f);
    m_targetVelocity = sf::Vector2f(0.0f, 0.0f);
    setupSprite();
}

void Player::setupSprite()
{
    m_playerSprite.setPointCount(3); 
    m_playerSprite.setRadius(2.0f);
    m_playerSprite.setScale(sf::Vector2f(8.0f, 8.0f));
    m_playerSprite.setOutlineThickness(0.5f);
    m_playerSprite.setOutlineColor(sf::Color::Black);
    m_playerSprite.setFillColor(sf::Color::Green); 

    // Center the shape on its position
    sf::FloatRect bounds = m_playerSprite.getLocalBounds();
    m_playerSprite.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));

    m_playerSprite.setPosition(m_position);
}

void Player::handleInput(float t_deltaTime)
{
    sf::Vector2f inputDirection(0.0f, 0.0f);

    // Check all arrow keys simultaneously
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) 
    {
        inputDirection.y -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) 
    {
        inputDirection.y += 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) 
    {
        inputDirection.x -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) 
    {
        inputDirection.x += 1.0f;
    }

    // Normalize diagonal movement so it's not faster
    inputDirection = normalizeVector(inputDirection);

    // Set target velocity based on input
    m_targetVelocity = inputDirection * m_speed;
}

sf::Vector2f Player::normalizeVector(const sf::Vector2f& t_vector)
{
    float length = std::sqrt(t_vector.x * t_vector.x + t_vector.y * t_vector.y);

    if (length != 0.0f)
    {
        return sf::Vector2f(t_vector.x / length, t_vector.y / length);
    }

    return sf::Vector2f(0.0f, 0.0f);
}

void Player::updateMovement(float t_deltaTime)
{
    // Smoothly interpolate current velocity toward target velocity
    sf::Vector2f velocityDiff = m_targetVelocity - m_velocity;
    float diffLength = std::sqrt(velocityDiff.x * velocityDiff.x + velocityDiff.y * velocityDiff.y);

    if (diffLength > 0.0f) 
    {
        // Choose acceleration or deceleration based on whether we're speeding up or slowing down
        float rate = (diffLength > m_speed * 0.1f) ? m_acceleration : m_deceleration;

        sf::Vector2f normalizedDiff = normalizeVector(velocityDiff);
        float maxChange = rate * t_deltaTime;

        if (diffLength <= maxChange)
        {
            // Close enough, just set to target
            m_velocity = m_targetVelocity;
        }
        else 
        {
            // Move toward target velocity
            m_velocity += normalizedDiff * maxChange;
        }
    }

    // Update position
    m_position += m_velocity * t_deltaTime;

    // Apply boundary wrapping
    BoundaryManager::wrapPosition(m_position, 1000.0f, 800.0f);

    // Update visual rotation to face movement direction (optional)
    if (m_velocity.x != 0.0f || m_velocity.y != 0.0f) 
    {
        float angle = std::atan2(m_velocity.y, m_velocity.x) * (180.0f / 3.14159f);
        m_playerSprite.setRotation(sf::degrees(angle + 90.0f)); // +90 because triangle points up by default
    }
}

void Player::Update(float t_deltaTime)
{
    handleInput(t_deltaTime);
    updateMovement(t_deltaTime);
    m_playerSprite.setPosition(m_position);
}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(m_playerSprite);
}