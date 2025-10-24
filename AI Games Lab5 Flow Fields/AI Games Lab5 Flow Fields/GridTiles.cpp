#include "GridTiles.h"



// Base Constructor
Grid::Grid(int t_rows, int t_cols)
	: m_rows(t_rows), m_columns(t_cols)	// Set Local grid values - then generate the grid
{
	generateGrid();
}


// Renders Grid and its tiles
void Grid::render(sf::RenderWindow& t_window) const
{
	for (const auto& tile : m_gridTiles)
	{
		tile.render(t_window);
	}
}


// Gets a specified tile in the grid
Tile& Grid::getTile(int t_row, int t_col)
{
	return m_gridTiles[t_row, t_col];
}


// Generates the main grid, should only be generated once - Maybe when recalculating movement for cost? But tile 
// inner values should be the only thing that is touched in that case.
void Grid::generateGrid()
{
	m_gridTiles.clear();
	m_gridTiles.reserve(m_rows * m_columns);
	
	// to center the grid when generating
	float gridWidth = m_rows * m_tileSize;
	float gridHeight = m_columns * m_tileSize;
	float winHeight = Utilities::WINDOW_HEIGHT;
	float winWidth = Utilities::WINDOW_WIDTH;

	// center offsets
	float offsetX = (winWidth - gridWidth) / 2.f;
	float offsetY = (winHeight - gridHeight) / 2.f;

	// Notice; Using l_ prefix to identify that it is the Function Local variable
	for (int l_row = 0; l_row < m_rows; l_row++)
	{
		for (int l_col = 0; l_col < m_columns; l_col++)
		{
			sf::Vector2f position(
				offsetX + l_row * m_tileSize,
				offsetY + l_col * m_tileSize);

			// ToDo: Add colour gradient effect here to apply below
			m_gridTiles.emplace_back(position); // Defaults to blue for now
		}

	}
}
