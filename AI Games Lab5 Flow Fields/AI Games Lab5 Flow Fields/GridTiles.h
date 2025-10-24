#pragma once
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
    

private:
    void generateGrid();

    int m_rows;
    int m_columns;
    std::vector<Tile> m_gridTiles;
    const float m_tileSize = Utilities::TILE_SIZE;
};