#pragma once

#include <SFML/Graphics.hpp>
#include <random>


class NPC
{
public:
	void GenerateSprite();

	void Update(sf::RenderWindow&);

	void Move();

private:

	sf::CircleShape m_npc;
	sf::Vector2f m_pos{ 400.0f, 300.0f };
	sf::Vector2f m_moveDir;
	bool directionDecided = false;

};