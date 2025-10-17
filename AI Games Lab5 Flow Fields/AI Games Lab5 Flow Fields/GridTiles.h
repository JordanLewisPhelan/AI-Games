#pragma once
#include <SFML/Graphics.hpp>



/// <summary>
/// Tiles will focus on encapsulating what a tile should concern itself with, self serving and 
/// these will be slot into a grid of plots of tiles.
/// Side-Note: If this gets too large move data into a TileInfo structure
/// </summary>
class Tiles 
{

public:


private:
	// -- sfml variables -- //
	sf::RectangleShape tileShape;
	sf::Color tileColour;

	// -- scalar variables -- //
	float tileSize;

	// -- field class variables -- //
	/*
		Not yet present, this would include details such as Vector & Cost fields
		Will allow us to know what way we should push the Entity & the rough 
		outline of how expensive the cost is, we will use this to adjust the 
		colour/hue of the tiles - visual representation is ideal
	*/
};


/// <summary>
/// Grid is simply just going to hold and effectively manage a series of Tiles that will be in a 
/// vector/array/list for accessor purposes and separation of concern reasons
/// </summary>
class Grid 
{

public:


private:

};