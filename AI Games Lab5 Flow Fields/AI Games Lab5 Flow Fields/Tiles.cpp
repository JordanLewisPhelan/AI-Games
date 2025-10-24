#include "Tiles.h"



Tile::Tile()
{	
	// Default
	m_tileShape.setSize({ m_tileSize, m_tileSize });
	m_tileShape.setFillColor(sf::Color::Blue);	
	m_tileShape.setOutlineColor(sf::Color::Black);
	m_tileShape.setOutlineThickness(2.f);
}

Tile::Tile(const sf::Vector2f& t_position, const sf::Color t_colour)
	: Tile()	// Delegating constructor: reuse default setup - Just nifty, saw and wanted to use - Not really important here
{
	m_tileShape.setPosition(t_position);
	m_tileShape.setFillColor(t_colour);	// Here only should we change it
}

void Tile::render(sf::RenderWindow& t_window) const
{
	t_window.draw(m_tileShape);
}

////
////

// Sets the traversal cost of a given tile
void Tile::setCost(float t_cost)
{
	m_cost = t_cost;
}

// Sets a determined direction that the flowfield will direct an entity
// that is travelling through this tile
void Tile::setDirection(const sf::Vector2f& t_direction)
{
	m_direction = t_direction;
}

// sets the colour - will be refactored mildly later to change hue based on cost
void Tile::setColour(const sf::Color& t_colour)
{
	m_tileShape.setFillColor(t_colour);
}

////
////

// Returns position of the tiles centre - Note: Don't forget to centre origin of tiles.
const sf::Vector2f& Tile::getPosition() const
{
	return m_tileShape.getPosition();
}

// Returns the tileSize dimensions of the tileShape
const sf::Vector2f& Tile::getSize() const
{
	return m_tileShape.getSize();
}

// Returns the cost of a given tile - will be used later for colour hue adjustments
float Tile::getCost() const
{
	return m_cost;
}

// Returns the flow-field direction of where a tile will send an entity
const sf::Vector2f& Tile::getDirection() const
{
	return m_direction;
}