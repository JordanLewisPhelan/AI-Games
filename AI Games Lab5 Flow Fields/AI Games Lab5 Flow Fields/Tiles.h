#pragma once

#include <iostream>
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
        const sf::Color t_colour = sf::Color::Blue,
        const int t_idValue = 0, const bool t_traversable = true);


    // -- Flow-Field data(Will implement use cases later) --
    void setCost(float t_cost);
    void setDirection(const sf::Vector2f& t_direction);

    // -- Accessors --
    const sf::Vector2f& getPosition() const;
    const sf::Vector2f& getSize() const;
    float getCost() const;
    const sf::Vector2f& getDirection() const;
    const int getID() const;
    bool isTraversable() const { return m_traversable; }

    // -- Renderers --
    void render(sf::RenderWindow& t_window, bool t_drawVectors = false, bool t_drawCost = false) const;

    // -- Adjustors --
    void setColour(const sf::Color& t_colour);
    void setCostText(const std::string& t_costText);
    void toggleTraversable();

private:
    // -- sfml variables -- //
    sf::RectangleShape m_tileShape;
    sf::Color m_tileColour;

    // -- scalar variables -- //
    int m_id = 0;
    bool m_traversable = true;

    // -- flow-field class variables -- //
    float m_cost = 1.f;
    sf::Vector2f m_direction = { 0,0 };
    
    // -- text variables -- //
    sf::Font m_jerseyFont;// font used by text
    sf::Text m_costText{ m_jerseyFont }; // text used for text on screen
};