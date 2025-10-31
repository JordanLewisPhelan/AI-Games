#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Utilities.h"
#include "Tiles.h"


/// <summary>
/// Grid is simply just going to hold and effectively manage a series of Tiles that will be in a 
/// vector/array/list for accessor purposes and separation of concern reasons
/// </summary>
class Grid 
{

public:
    Grid(int t_rows, int t_cols);

    void render(sf::RenderWindow& t_window) const;

    Tile& getTile(int t_row, int t_col);
    
    int getRows() const;
    int getCols() const;

    bool Grid::isValid(int row, int col) const
    {
        return row >= 0 && row < m_rows &&
            col >= 0 && col < m_columns;
    }

    
    // -- FF Data visual togglers -- 
    void toggleDebugVector() { m_showVectors = !m_showVectors; }
    void toggleDebugCost() { m_showCost = !m_showCost; }


    // Solely just for visualizing and checking during flowfield that they are not overwritten
    void setStartTile(const sf::Vector2i& pos) { m_startTile = pos; }
    void setEndTile(const sf::Vector2i& pos) { m_endTile = pos; }

    const sf::Vector2i& getStartTile() const { return m_startTile; }
    const sf::Vector2i& getEndTile() const { return m_endTile; }

private:
    void generateGrid();

    int m_rows;
    int m_columns;
    std::vector<Tile> m_gridTiles;
    bool m_showVectors = false;
    bool m_showCost = false;

    // Stasis values to ensure we can check and visualize start and end easily
    sf::Vector2i m_startTile{ 0, 0};
    sf::Vector2i m_endTile{ 49, 49 };
   // const float m_tileSize = Utilities::TILE_SIZE;  // done to keep scripts more readable | Maybe stupid in retrospect
};