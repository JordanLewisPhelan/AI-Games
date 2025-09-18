#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player(sf::Vector2f startPos = sf::Vector2f(500.0f, 400.0f));
    void Update(float deltaTime);
    void Render(sf::RenderWindow& window);
    sf::Vector2f getPosition() const { return m_position; }
    void setSpeed(float speed) { m_speed = speed; }

private:
    void handleInput(float deltaTime);
    void setupSprite();
    void updateMovement(float deltaTime);
    sf::Vector2f normalizeVector(const sf::Vector2f& vector);

    sf::CircleShape m_playerSprite;
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Vector2f m_targetVelocity;  // What we want to move toward

    float m_speed;
    float m_angle;
    float m_rotationSpeed = 180.0f; // degrees per second
    float m_acceleration;  // How quickly we reach target velocity
    float m_deceleration;  // How quickly we slow down when no input
};