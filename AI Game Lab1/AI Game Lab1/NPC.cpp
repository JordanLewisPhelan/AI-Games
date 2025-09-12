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

void NPC::update(sf::RenderWindow& t_window)
{

	t_window.draw(m_npc);
}
