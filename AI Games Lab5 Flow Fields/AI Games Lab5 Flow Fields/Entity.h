#pragma once

#include <SFML/Graphics.hpp>
#include "GridTiles.h"

class Entity
{
public:
	Entity(const sf::Vector2i& t_startGrid, Grid& t_gridRef)
		: m_posGrid(t_startGrid), m_grid(t_gridRef)
	{
		m_posFloat = { t_startGrid.x * Utilities::TILE_SIZE + Utilities::TILE_SIZE / 2.f,
					   t_startGrid.y * Utilities::TILE_SIZE + Utilities::TILE_SIZE / 2.f };
		m_entityBody.setRadius(Utilities::TILE_SIZE / 4.f);
		m_entityBody.setFillColor(sf::Color::Cyan);
		m_entityBody.setOrigin(sf::Vector2f(m_entityBody.getRadius(), m_entityBody.getRadius()));


		sf::Vector2f gridOrigin(	// Just hard coded based on screen dimensions.
			(1200 - (m_grid.getCols() * Utilities::TILE_SIZE)) / 2.f,
			(1000 - (m_grid.getRows() * Utilities::TILE_SIZE)) / 2.f
		);

		m_localGridOrigin = gridOrigin;
		pathToTraverse(t_startGrid, m_localGridOrigin);
	}

	void update(float t_dt);

	void render(sf::RenderWindow& t_window);

	void pathToTraverse(const sf::Vector2i& t_gridStart, const sf::Vector2f& t_gridOrigin);

private:
	sf::Vector2i m_posGrid;
	sf::Vector2f m_posFloat;
	float m_speed = 50.f;
	Grid& m_grid;
	sf::CircleShape m_entityBody;
	sf::Vector2f m_localGridOrigin;
};