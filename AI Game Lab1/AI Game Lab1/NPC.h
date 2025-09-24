#pragma once

#include <SFML/Graphics.hpp>
#include <random>


// Forward declaration
class SteeringBehavior;

class NPC
{
public:
    NPC(sf::Vector2f startPos);

    void Update(float t_deltaTime);
    void Render(sf::RenderWindow&);

    // Accessors
    sf::Vector2f getPosition() const { return m_pos; }
    sf::Vector2f getVelocity() const { return m_velocity; }
    float getMaxSpeed() const { return m_speed; }
    float setSpeed(float t_speed) { return m_speed = t_speed; }

    // Behavior management
    void setBehavior(std::unique_ptr<SteeringBehavior>, const std::string& behaviorName);
    void toggleActive() { m_active = !m_active; }
    // Cone Management
    void toggleCone() { m_showCone = !m_showCone; }
    void updateVisionCone();



private:
    void GenerateSprite();
    void FaceMovementDirection();

    sf::CircleShape m_npc;
    sf::Vector2f m_pos;
    sf::Vector2f m_velocity;
    float m_speed;
    std::unique_ptr<SteeringBehavior>  m_behaviour = nullptr;
    std::string m_behaviorName = "None";

    // Control for NPC presence
    bool m_active = true;

    // Vision Cone context
    bool m_showCone = false;
    sf::ConvexShape m_visionCone;
    float m_visionAngle = 45.f;   // half-angle in degrees
    float m_visionLength = 150.f; // how far they can see

    // Display text
    // sf::Text m_label;
};



