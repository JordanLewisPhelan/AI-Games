#pragma once

#include <SFML/Graphics.hpp>
#include <random>


// Forward declaration
class SteeringBehavior;

class NPC
{
public:
    NPC(sf::Vector2f startPos = sf::Vector2f(400.f, 300.f));

    void Update(float t_deltaTime);
    void Render(sf::RenderWindow&);

    // Accessors
    sf::Vector2f getPosition() const { return m_pos; }
    sf::Vector2f getVelocity() const { return m_velocity; }
    float getMaxSpeed() const { return m_speed; }

    // Behavior management
    void setBehavior(SteeringBehavior* behavior) { m_behaviour = behavior; }

private:
    void GenerateSprite();
    void FaceMovementDirection();

    sf::CircleShape m_npc;
    sf::Vector2f m_pos;
    sf::Vector2f m_velocity;
    float m_speed;
    SteeringBehavior* m_behaviour = nullptr;
};



