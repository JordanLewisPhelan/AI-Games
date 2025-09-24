#include "NPC.h"
#include "Behaviours.h"
#include "BoundaryManager.h"

// // Personal Notes
	// In Vectors:
	// Cos is the Horizontal -- Left - 180 | 0 - Right -
	// Sin is the Vertical -- Up - 90 | 270 - Down -


// Initializer for an NPC
NPC::NPC(sf::Vector2f startPos)
	: m_pos(startPos), m_speed(50.0f)
{
	GenerateSprite();
}

// Generate our NPC and visualize them visually
void NPC::GenerateSprite()
{
	// Used to give our circle only 3 vertexes, in practice this should yield a triangle(good for direction showcase)
	m_npc.setPointCount(3);

	// Default value setup
	m_npc.setRadius(2);
	m_npc.setScale(sf::Vector2f(8.0f, 8.0f));
	m_npc.setPosition(m_pos);
	m_npc.setOutlineThickness(0.5f);
	m_npc.setOutlineColor(sf::Color::Black);
	m_npc.setFillColor(sf::Color::Magenta);

	// Centers the shape on its position - prevents odd movements
	sf::FloatRect bounds = m_npc.getLocalBounds();
	m_npc.setOrigin(sf::Vector2f(bounds.size.x / 2.0f,
								 bounds.size.y / 2.0f));
}

// Simply adjusts shape rotation to face the Player - ToDo: currently pretty snappy at intense re-directs, look into a fix later on
void NPC::FaceMovementDirection()
{
	// Only update if velocity is non-zero
	if (m_velocity.x != 0.f || m_velocity.y != 0.f)
	{
		// Calculate angle in degrees from velocity
		float angleRad = std::atan2(m_velocity.y, m_velocity.x); // atan2 returns radians
		float angleDeg = angleRad * 180.f / 3.14159265f;

		// Triangle points up by default, so +90 to align
		m_npc.setRotation(sf::degrees(angleDeg + 90.f));
	}
}


void NPC::Update(float t_deltaTime)
{
	if (m_behaviour) {
		sf::Vector2f steering = m_behaviour->calculateSteering(m_pos,
			m_velocity,
			m_speed);

		m_velocity += steering;

		// Clamp velocity
		float len = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
		if (len > m_speed) {
			m_velocity = (m_velocity / len) * m_speed;
		}
	}
	// should be more mild movements - less jitter or snappiness
	m_pos += m_velocity * t_deltaTime;

	// Using the full window size we check to see if we are even near the border
	BoundaryManager::wrapPositionGlobal(m_pos);

	m_npc.setPosition(m_pos);
	
	// Replaces Generate Random Direction to work with any movement not just a fixed one
	FaceMovementDirection();
}

void NPC::Render(sf::RenderWindow& t_window)
{	
	// Draws a declared NPC
	t_window.draw(m_npc);
}

