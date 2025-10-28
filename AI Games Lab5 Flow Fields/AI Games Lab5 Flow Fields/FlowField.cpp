#include "FlowField.h"



/*
	Just a reminder;
	
	using prefix l_ for internal/loop values in functions - DOES NOT APPLY TO FUNCTION-WIDE VALUES, this is an effort
	to try and categorize what is used throughout a function & what is used in a sub-loop like a while or for loop.

	using prefix n_ to differentiate local member varaibles from the struct node variables.
*/


Flowfield::Flowfield(Grid& t_grid) : m_grid(t_grid)
{
	// Attach grid values to local FlowField values while we are linking flowfield & grid
	m_rows = m_grid.getRows();
	m_cols = m_grid.getCols();
}


void Flowfield::generateCostField(const sf::Vector2i& t_startPos)
{
	// BFS Traversal queue
	std::queue<Node> explorer;

	/*					row		-  columns	-	Cost					*/
	explorer.push({ t_startPos.x, t_startPos.y, 0.f });

	// array of tiles we have visited - based on the rows and then the columns, which we will say have a value of -1 when visited to avoid repeat visits
	std::vector<std::vector<float>> visited(
		m_rows, std::vector<float>(m_cols, -1.f)
	);

	// Start at our chosen point
	visited[t_startPos.x][t_startPos.y] = 0.f;

	// List of explorable directions when at a node - currently: 4 options, consisting of 2 points - maps a direction
	const int directions[4][2] = {
		{1,0}, {-1,0}, { 0,1 }, {0, -1}
	};



	/// BFS Loop to go through each node
	while (!explorer.empty())
	{
		// Grab the first node and remove it from the queue
		Node l_current = explorer.front();
		explorer.pop();

		// Check our directions
		for (auto& dir : directions)
		{
			int nodeRow = l_current.n_rows + dir[0];
			int nodeCol = l_current.n_cols + dir[1];

			if (!isValid(nodeRow, nodeCol))
				continue;

			// See if it has been visited yet - if its -1 it is default, i.e. unvisited
			if (visited[nodeRow][nodeCol] == -1.f)
			{
				float l_newCost = l_current.n_cost + 1;
				visited[nodeRow][nodeCol] = l_newCost;
				explorer.push({ nodeRow, nodeCol, l_newCost });

				// Update relevant Tile in the grid
				Tile& tile = m_grid.getTile(nodeRow, nodeCol);
				tile.setCost(l_newCost);

				int l_colourHue = static_cast<float>(255 - std::min(l_newCost * 5, 200.f));
				tile.setColour(sf::Color(l_colourHue, l_colourHue, 225.f));
			}
		}
	}
}


// Checks by comparing input is 0 or more and that input is not more than the local values
bool Flowfield::isValid(int t_row, int t_col) const
{
	return t_row >= 0 && t_col >= 0
		&& t_row < m_rows
		&& t_col < m_cols;
}
