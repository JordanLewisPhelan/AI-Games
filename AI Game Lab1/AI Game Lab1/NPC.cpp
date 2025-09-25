#include "NPC.h"
#include "Behaviours.h"
#include "BoundaryManager.h"

// // Personal Notes
	// In Vectors:
	// Cos is the Horizontal -- Left - 180 | 0 - Right -
	// Sin is the Vertical -- Up - 90 | 270 - Down -


NPC::NPC(sf::Vector2f startPos)
	: m_pos(startPos),
	m_speed(50.f)
{
	// Generate visual triangle
	GenerateSprite();

	// Setup vision cone as a triangle fan (3 points = FOV wedge)
	m_visionCone.setPointCount(3);
	m_visionCone.setFillColor(sf::Color(255, 255, 0, 100)); // semi-transparent yellow

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

	if (!m_jerseyFont.openFromFile("ASSETS\\FONTS\\Jersey20-Regular.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_name.setFont(m_jerseyFont);
	m_name.setString("hello world");
	m_name.setPosition(sf::Vector2f{ 205.0f, 240.0f });
	m_name.setCharacterSize(15U);
	m_name.setFillColor(sf::Color::White);
}

void NPC::setBehavior(std::unique_ptr<SteeringBehavior>t_behavior, const std::string& t_behaviorName)
{
	m_behaviour = std::move(t_behavior);
	m_behaviorName = t_behaviorName;
	m_name.setString(m_behaviorName); // update label
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

// AI Generated, it is late, this shit is awkward.. I will regrettable read over this and how to do this better later..
void NPC::updateVisionCone()
{
	if (!m_showCone) return;

	float visionLength = 100.f;
	float visionAngle = 40.f; // full cone angle

	// Get NPC rotation in degrees and correct for the triangle pointing right by default
	float rotationDeg = m_npc.getRotation().asDegrees() - 90.f;
	float halfAngle = visionAngle / 2.f;

	// Convert left/right angles to radians
	float leftRad = (rotationDeg - halfAngle) * 3.14159265f / 180.f;
	float rightRad = (rotationDeg + halfAngle) * 3.14159265f / 180.f;

	sf::Vector2f pos = m_pos;

	// Compute the end points of the vision cone
	sf::Vector2f leftPoint = pos + sf::Vector2f(std::cos(leftRad), std::sin(leftRad)) * visionLength;
	sf::Vector2f rightPoint = pos + sf::Vector2f(std::cos(rightRad), std::sin(rightRad)) * visionLength;

	// Build the triangle
	m_visionCone.setPointCount(3);
	m_visionCone.setPoint(0, pos);        // NPC position as apex
	m_visionCone.setPoint(1, leftPoint);  // left edge
	m_visionCone.setPoint(2, rightPoint); // right edge

	m_visionCone.setFillColor(sf::Color(255, 255, 0, 100)); // semi-transparent yellow
}


void NPC::Update(float t_deltaTime)
{
	if (!m_active) return;

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
	m_name.setPosition(m_pos + m_textOffset);
	
	// Replaces Generate Random Direction to work with any movement not just a fixed one
	FaceMovementDirection();

	updateVisionCone();
}

void NPC::Render(sf::RenderWindow& t_window)
{	
	// Draws a declared NPC
	if (!m_active) return;

	if (m_showCone)
		t_window.draw(m_visionCone);
	t_window.draw(m_name);
	t_window.draw(m_npc);
}

