#pragma once

#include "GridTiles.h"
#include <queue>

/*	Intenral Notes + reminders - Remove later [ Or keep for proof of logic? ]:
//// Summary Notice about concept: What is the intended idea of a flowfield?
//// Flow-fields are just nodes/positions or in this case tiles - with a direction to proceed to the next tile,
//// this allows for any number of entities to "Flow" to a central, consistent output location, they have a cost
//// as well however, this is determined by how out of the way they are from the path.
////	Maybe Start position will matter?

/// 

 How am I going to implement this?
 Well as mentioned there is a consistent output location once it is generated. So we know where we NEED TO GO,
 how we get there is by using the directions to direct our entities. So realistically, while we need a start point
 we should actually just discard it for calculations. The goal is what matters, so if we build outward from the
 end point we have an origin so know where we are coming back to. Meaning as we traverse new tiles we know where we 
 are heading and can use that tiles neighbours to send us back to the end point.

/// 

 So idea is to build outward from Destination as start could be anywhere and shouldnt matter as we cover it all. 
 So I just gotta determine what actually makes a good traversal algorithm - In research ive seen djikstras come up
 a lot and BFS(Breadth First Search), I will need to weigh which is better for this as the only rules are;
 
 1. Some tiles are non-traversable. Boolean will sort that.
 1a. In researching most of these are intended to be weighted, there is no such requirement here, so I am
 torn between doing it RIGHT and applicably for flow-field physics or keeping it simple so I can focus on
 other work: Current thought process? Sadly, Simple. Just a boolean, no weighting.
 - I realize for specifically flow-field the above is it..

 2. Debate whether start position for effective cost field is necessary.
*/


class Flowfield {
public:
	Flowfield(Grid& t_grid);

	void generateCostField(const sf::Vector2i& t_startPos);

private:
	Grid& m_grid;

	// Local Struct to represent where we have explored in the grid
	struct Node 
	{
		int n_rows;
		int n_cols;
		float n_cost;
	};

	// Checking if a tile is valid
	bool isValid(int t_row, int t_col) const;

	int m_rows, m_cols;
};