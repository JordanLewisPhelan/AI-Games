#include "NPC.h"

// Generate our NPC and visualize them visually
void NPC::GenerateSprite()
{
	// Used to give our circle only 3 vertexes, in practisce this should yield a triangle(good for direction showcase)
	m_npc.setPointCount(3);

	// Default value setup
	m_npc.setRadius(2);
	m_npc.setScale(sf::Vector2f(8.0f, 8.0f));
	m_npc.setPosition(m_pos);
	m_npc.setOutlineThickness(0.5f);
	m_npc.setOutlineColor(sf::Color::Black);
	m_npc.setFillColor(sf::Color::Magenta);
	m_npc.setRotation(sf::degrees(180.0f));
}

void NPC::Update(sf::RenderWindow& t_window)
{
	// Draws a declared NPC
	t_window.draw(m_npc);
}

void NPC::Move()
{
	if (!directionDecided)
	{
		// Temporary to decide the SOLE direction of an NPC movment 
		int random;
		random = rand() % 4;

		switch (random)
		{
		case 0:	// Left
			m_moveDir = sf::Vector2f(-1, 0);
			directionDecided = true;

		case 1:	// Right
			m_moveDir = sf::Vector2f(1, 0);
			directionDecided = true;

		case 2:	// Down
			m_moveDir = sf::Vector2f(0, -1);
			directionDecided = true;

		case 3:	// Up
			m_moveDir = sf::Vector2f(0, 1);
			directionDecided = true;

		default:
			break;
		}
	}

	m_pos += m_moveDir;

	m_npc.setPosition(m_pos);
}
