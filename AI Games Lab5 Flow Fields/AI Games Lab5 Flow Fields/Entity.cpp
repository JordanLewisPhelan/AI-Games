#include "Entity.h"

void Entity::update(float t_dt)
{
    // Get current tile based on grid coordinates
    Tile& currentTile = m_grid.getTile(m_posGrid.x, m_posGrid.y);
    sf::Vector2f dir = currentTile.getDirection();

    // Move in world space
    m_posFloat += dir * m_speed * t_dt;

    // Calculate new grid position based on centered grid
    sf::Vector2i newGrid{
        static_cast<int>((m_posFloat.x - m_localGridOrigin.x) / Utilities::TILE_SIZE), // row
        static_cast<int>((m_posFloat.y - m_localGridOrigin.y) / Utilities::TILE_SIZE)  // col
    };

    // Clamp to grid bounds
    newGrid.x = std::clamp(newGrid.x, 0, m_grid.getRows() - 1);
    newGrid.y = std::clamp(newGrid.y, 0, m_grid.getCols() - 1);

    if (newGrid != m_posGrid)
    {
        m_posGrid = newGrid;
        m_grid.getTile(m_posGrid.x, m_posGrid.y).setColour(sf::Color::Yellow);
    }

    // Update entity's visual position
    m_entityBody.setPosition(sf::Vector2f(
        m_localGridOrigin.x + m_posGrid.y * Utilities::TILE_SIZE + Utilities::TILE_SIZE / 2.f,
        m_localGridOrigin.y + m_posGrid.x * Utilities::TILE_SIZE + Utilities::TILE_SIZE / 2.f)
    );
}


void Entity::render(sf::RenderWindow& t_window)
{
	m_entityBody.setPosition(m_posFloat);

	t_window.draw(m_entityBody);
}

// Takes only the start position as the idea is they will drift to the goal.
void Entity::pathToTraverse(const sf::Vector2i& t_startGrid, const sf::Vector2f& t_gridOrigin)
{
	m_posGrid = t_startGrid;	
    m_localGridOrigin = t_gridOrigin; // save origin for updates

    // Position entity at the center of the start tile
    m_posFloat = sf::Vector2f(
        m_localGridOrigin.x + m_posGrid.x * Utilities::TILE_SIZE + Utilities::TILE_SIZE / 2.f,
        m_localGridOrigin.y + m_posGrid.y * Utilities::TILE_SIZE + Utilities::TILE_SIZE / 2.f
    );

	m_entityBody.setPosition(m_posFloat);
}
