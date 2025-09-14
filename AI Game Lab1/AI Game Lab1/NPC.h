#pragma once

#include <SFML/Graphics.hpp>
#include <random>


class NPC
{
public:

	NPC(sf::Vector2f startPos = sf::Vector2f(400.0f, 300.0f));

	void Update(float t_deltaTime);
	void Render(sf::RenderWindow&);

	sf::Vector2f getPosition() const { return m_pos; }

private:

	void GenerateSprite();
	void GenerateRandomDirection();

	sf::CircleShape m_npc;
	sf::Vector2f m_pos;
	sf::Vector2f m_velocity;
	float m_speed;
	bool directionDecided = false;

};