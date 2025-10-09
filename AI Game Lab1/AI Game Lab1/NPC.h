#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>


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
	void setPosition(const sf::Vector2f& pos) { m_pos = pos; m_npc.setPosition(m_pos); }

    sf::Vector2f getVelocity() const { return m_velocity; }
    float getMaxSpeed() const { return m_speed; }
    float setSpeed(float t_speed) { return m_speed = t_speed; }

    float getRotationDegrees() const { return m_npc.getRotation().asDegrees(); }
    float getVisionLength() const { return m_visionLength; }
    float getVisionHalfAngleRadians() const { return (m_visionAngle * 0.5f) * 3.14159265f / 180.f; }

    // Behavior management
    void setBehavior(std::unique_ptr<SteeringBehavior>, const std::string& behaviorName);
    void toggleActive() { m_active = !m_active; }
    // Cone Management
    void toggleCone() { m_showCone = !m_showCone; }
    void updateVisionCone();

    // Update player detection status
    void setPlayerDetected(bool detected);
    bool isPlayerDetected() const { return m_playerDetected; }

private:
    void GenerateSprite();
    void FaceMovementDirection();

    sf::CircleShape m_npc;
    sf::Vector2f m_pos;
    sf::Vector2f m_velocity;
    float m_speed;
    std::unique_ptr<SteeringBehavior>  m_behaviour = nullptr;
    std::string m_behaviorName = "None";

    sf::Font m_jerseyFont;// font used by message
    sf::Text m_name{ m_jerseyFont };
    sf::Vector2f m_textOffset{ sf::Vector2f(20.f, 15.f) };

    // Control for NPC presence
    bool m_active = true;

    // Vision Cone context
    bool m_showCone = false;
    sf::ConvexShape m_visionCone;
    float m_visionAngle = 45.f;   // half-angle in degrees
    float m_visionLength = 150.f; // how far they can see
    bool m_playerDetected = false;

};



