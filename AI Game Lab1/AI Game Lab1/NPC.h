#pragma once

#include <SFML/Graphics.hpp>


class NPC
{
public:
	void GenerateSprite();

	void Update(sf::RenderWindow&);

	void Move();

private:

	sf::CircleShape m_npc;
	sf::Vector2f m_pos{ 400.0f, 300.0f };
	bool directionDecided;

};