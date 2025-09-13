#include "NPC.h"
#include "BoundaryManager.h"

// // Personal Notes
	// In Vectors:
	// Cos is the Horizontal -- Left - 180 | 0 - Right -
	// Sin is the Vertical -- Up - 90 | 270 - Down -


// Initializer for an NPC
NPC::NPC(sf::Vector2f startPos)
	: m_pos(startPos), m_speed(90.0f)
{
	GenerateSprite();
	GenerateRandomDirection();
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
	m_npc.setRotation(sf::degrees(180.0f));

	// Centers the shape on its position - prevents odd movements
	sf::FloatRect bounds = m_npc.getLocalBounds();
	m_npc.setOrigin(sf::Vector2f(bounds.size.x / 2.0f,
								 bounds.size.y / 2.0f));
}

// Creates a random direction for our NPC to move in, testing 360 degree angles for inevitable complexity 
// Tldr; half of this is unnecessary.. But for SCIENCE!
void NPC::GenerateRandomDirection()
{

	// VILE, Hacky idea to randomize the move direction properly
	rand(); rand(); rand();

	// Simple random angle between 0 and 359 degrees
	float angle = static_cast<float>(rand() % 360);

	// Convert to radians for math functions
	float angleRad = angle * (3.14159f / 180.0f);

	// Create velocity vector from angle
	m_velocity.x = std::cos(angleRad) * m_speed;
	m_velocity.y = std::sin(angleRad) * m_speed;
	

	// Rotate the triangle to face the movement direction
	m_npc.setRotation(sf::degrees(angle + 90.0f)); // +90 because triangle points up by default
}

void NPC::Update(float t_deltaTime)
{
	// should be more mild movements - less jitter or snappiness
	m_pos += m_velocity * t_deltaTime;

	// Using the full window size we check to see if we are even near the border
	BoundaryManager::wrapPositionGlobal(m_pos);

	m_npc.setPosition(m_pos);
}

void NPC::Render(sf::RenderWindow& t_window)
{	
	// Draws a declared NPC
	t_window.draw(m_npc);
}
