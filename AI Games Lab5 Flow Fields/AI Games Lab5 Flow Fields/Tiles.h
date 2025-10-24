#pragma once

#include <SFML/Graphics.hpp>
#include "Utilities.h"

/// <summary>
/// Tiles will focus on encapsulating what a tile should concern itself with, self serving and 
/// these will be slot into a grid of plots of tiles.
/// Side-Note: If this gets too large move data into a TileInfo structure
/// </summary>
class Tile
{

public:
    /// -- Constructors --

    // Default constructor
    Tile();

    // Custom Constructor to give : Position & Optional Colour(Defaults to blue)
    Tile(const sf::Vector2f& t_position,
        const sf::Color t_colour = sf::Color::Blue);


    // -- Flow-Field data(Will implement use cases later) --
    void setCost(float t_cost);
    void setDirection(const sf::Vector2f& t_direction);

    // -- Accessors --
    const sf::Vector2f& getPosition() const;
    const sf::Vector2f& getSize() const;
    float getCost() const;
    const sf::Vector2f& getDirection() const;

    // -- Renderers --
    void render(sf::RenderWindow& t_window) const;

    // -- Adjustors --
    void setColour(const sf::Color& t_colour);

private:
    // -- sfml variables -- //
    sf::RectangleShape m_tileShape;
    sf::Color m_tileColour;

    // -- scalar variables -- //

    // -- flow-field class variables -- //
    float m_cost = 1.f;
    sf::Vector2f m_direction = { 0,0 };
    float m_tileSize = Utilities::TILE_SIZE;
};