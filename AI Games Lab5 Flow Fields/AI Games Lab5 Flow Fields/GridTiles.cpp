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
		tile.render(t_window, m_showVectors, m_showCost);
	}
}


// Gets a specified tile in the grid
Tile& Grid::getTile(int t_row, int t_col)
{
	//auto& l_retrieved = m_gridTiles[t_row * m_columns + t_col];

	//std::cout << "The indexed tiles ID is: " << l_retrieved.getID() << ", that tiles position is: \n X( " 
	//	<< l_retrieved.getPosition().x << " ) \n Y( " << l_retrieved.getPosition().y << " ) \n\n";

	//m_gridTiles[t_row * m_columns + t_col].setColour(sf::Color::Magenta);

	if (t_row < 0 || t_row >= m_rows || t_col < 0 || t_col >= m_columns)
		throw std::out_of_range("Tile access out of bounds!");

	int index = t_row * m_columns + t_col; // correct 1D mapping

	assert(index >= 0 && index < m_gridTiles.size());

	return m_gridTiles[index];

/*
	 Proof i am an idiot, so this I did use AI to diagnose - The issue is so fucking subtle I am
	 just shocked it never came up before ; I was trying to get a Tile Index and it makes sense to grab 
	 it via the Tile X-Y positions - But what isnt explained and isn't an error is ',' disregards the
	 first variable and checks the second, so this was always wrong as it was only getting the column position
	 Leaving as a permanent reminder to never trust blasted vectors and subtle errors - I wouldve spent days on this.. diagnosing the cause alone
	
	 Update: Figured out why it never came up before, I was using 2D arrays, this is technically 1 Dimension, so this particular
	 Error would never have been applicable before. But I am glad I bumped into it - Learning is Learning, and trying something 
	 different is so important - least I can do is correct this and stick it out - God forbid this is technical debt and is not worth it...
*/
//	m_gridTiles[t_row, t_col].setColour(sf::Color::Magenta);
//	return m_gridTiles[t_row, t_col];
}

int Grid::getRows() const
{
	return m_rows;
}

int Grid::getCols() const
{
	return m_columns;
}


// Generates the main grid, should only be generated once - Maybe when recalculating movement for cost? But tile 
// inner values should be the only thing that is touched in that case.
void Grid::generateGrid()
{
	m_gridTiles.clear();
	m_gridTiles.reserve(m_rows * m_columns);
	
	// to center the grid when generating
	float gridWidth = m_rows * Utilities::TILE_SIZE;
	float gridHeight = m_columns * Utilities::TILE_SIZE;
	float winHeight = Utilities::WINDOW_HEIGHT;
	float winWidth = Utilities::WINDOW_WIDTH;

	// center offsets
	float offsetX = (winWidth - gridWidth) / 2.f;
	float offsetY = (winHeight - gridHeight) / 2.f;

	// Notice; Using l_ prefix to identify that it is the Function Local variable
	for (int l_row = 0; l_row < m_rows; l_row++)
	{
		int assignID = 0;

		for (int l_col = 0; l_col < m_columns; l_col++)
		{
			assignID++;

			sf::Vector2f position(
				offsetX + l_row * Utilities::TILE_SIZE,
				offsetY + l_col * Utilities::TILE_SIZE);

			// ToDo: Add colour gradient effect here to apply below
			m_gridTiles.emplace_back(position, sf::Color::Blue, assignID); // Defaults to blue for now
		}
	}
}
