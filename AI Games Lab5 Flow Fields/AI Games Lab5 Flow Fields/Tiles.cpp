#include "Tiles.h"



Tile::Tile()
{	
	// Default
	m_tileShape.setSize({ Utilities::TILE_SIZE, Utilities::TILE_SIZE });	
	m_tileShape.setOutlineColor(sf::Color::Black);
	m_tileShape.setOutlineThickness(2.f);
	if (!m_jerseyFont.openFromFile("ASSETS\\FONTS\\Jersey20-Regular.ttf"))	// declaring it here because the text was not loading correctly
		std::cout << "No font was applied. \n\n";
}

Tile::Tile(const sf::Vector2f& t_position, const sf::Color t_colour, const int t_idVal, const bool t_travserable)
	: Tile()	// Delegating constructor: reuses default setup - Just nifty, saw and wanted to use - Not really important here
{
	m_tileShape.setPosition(t_position);
	m_id = t_idVal;
	m_traversable = t_travserable;
}

void Tile::render(sf::RenderWindow& t_window, bool t_showVectors, bool t_showCost) const
{
	t_window.draw(m_tileShape);

	if (t_showCost)
		t_window.draw(m_costText);

	if (t_showVectors)
	{
		sf::Vertex line[2];
		sf::Vector2f center = m_tileShape.getPosition() + m_tileShape.getSize() / 2.f;

		line[0].position = center;
		line[0].color = sf::Color::Magenta;

		line[1].position = center + m_direction * 10.f; // scale for visibility
		line[1].color = sf::Color::Magenta;

		// Just trying to use lines for simplicity - not ideal
		t_window.draw(line, 2, sf::PrimitiveType::Lines);
	}
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


void Tile::setCostText(const std::string& t_costText)
{
	m_costText.setFont(m_jerseyFont);
	m_costText.setString(t_costText);
	m_costText.setCharacterSize(static_cast<unsigned>(m_tileShape.getSize().y * 0.75f));	// trying to increase visual based on tileSize
	m_costText.setFillColor(sf::Color::Black);


	sf::FloatRect bounds = m_costText.getLocalBounds();
	m_costText.setOrigin(sf::Vector2f(
		bounds.position.x + bounds.size.x / 2.f,
		bounds.position.y + bounds.size.y / 2.f)
	);

	// centers it roughly inside the tile
	sf::Vector2f pos = m_tileShape.getPosition();
	sf::Vector2f size = m_tileShape.getSize();
	sf::Vector2f centered = { pos.x + size.x / 2.f, pos.y + size.y / 2.f };

	m_costText.setPosition(centered);
}


void Tile::toggleTraversable()
{
	m_traversable = !m_traversable;

	if (!m_traversable)
		m_tileShape.setFillColor(sf::Color::Black);
	else
		m_tileShape.setFillColor(sf::Color::Blue);
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

const int Tile::getID() const
{
	return m_id;
}
