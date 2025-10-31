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


/* -----
NOTE: I am aware of the MASSIVE flaw here, this is technically redundant - Initial draft had me redoing this anyway
So it gave me the idea to resolve an issue I had.	The cost should be running from the start point so it is accurate to the
start and not dependent on the end goal. But the end point is better for determining how to get back to the end point without
having to do cosntant steering calculations, if it goes outward, it only needs to know where it came from, not where it is going
- So this is what has led to this amalgamation, I can start at the start always as it makes the computation of finding the goal/end
point harder, but I cant start at the end as it makes the cost value wrong, as we have no info on where we will start and expanding outwards.

I will be looking into amending this - seeing if there is anyway to put these into 1 search with damaging the efficacy of the other.
-----*/

/// Merging both of the Below together into 1 - atleast more readable this way till comment here is done
void Flowfield::generateHybridFlowfield(const sf::Vector2i& t_startPos, const sf::Vector2i& t_goalPos)
{
    Tile& startTile = m_grid.getTile(t_startPos.x, t_startPos.y);
    startTile.setColour(sf::Color::Green);

    Tile& goalTile = m_grid.getTile(t_goalPos.x, t_goalPos.y);
    goalTile.setColour(sf::Color::Red);


    // --- 1) Cost from the start ---
    std::queue<Node> costQueue;
    costQueue.push({ t_startPos.x, t_startPos.y, 0.f });

    std::vector<std::vector<float>> costVisited(
        m_rows, std::vector<float>(m_cols, -1.f)
    );
    costVisited[t_startPos.x][t_startPos.y] = 0.f;

    costVisited[t_startPos.x][t_startPos.y] = 0.f;
    costVisited[t_goalPos.x][t_goalPos.y] = 0.f; // Prevent BFS from visiting goal or start


    const int directions[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    while (!costQueue.empty())
    {
        Node current = costQueue.front();
        costQueue.pop();


        for (auto& dir : directions)
        {
            int nRow = current.n_rows + dir[0];
            int nCol = current.n_cols + dir[1];

            if (!m_grid.isValid(nRow, nCol) || costVisited[nRow][nCol] != -1.f)
                continue;

            Tile& tile = m_grid.getTile(nRow, nCol);

             // Skip obstacles
            if (!tile.isTraversable())
                continue;

            if ((nRow == m_grid.getStartTile().x && nCol == m_grid.getStartTile().y) ||
                (nRow == m_grid.getEndTile().x && nCol == m_grid.getEndTile().y))
                continue;

            float newCost = current.n_cost + 1.f;
            costVisited[nRow][nCol] = newCost;

            // Update tile cost and color
            tile.setCost(newCost);
            tile.setCostText(std::to_string(static_cast<int>(newCost)));
            int hue = static_cast<int>(255 - std::min(newCost * 5, 200.f));
            tile.setColour(sf::Color(hue, hue, 225));

            costQueue.push({ nRow, nCol, newCost });
        }
    }

    // --- 2) Vector directions toward goal ---
    std::queue<Node> vectorQueue;
    vectorQueue.push({ t_goalPos.x, t_goalPos.y, 0.f });

    std::vector<std::vector<bool>> visited(
        m_rows, std::vector<bool>(m_cols, false)
    );
    visited[t_goalPos.x][t_goalPos.y] = true;

    while (!vectorQueue.empty())
    {
        Node current = vectorQueue.front();
        vectorQueue.pop();

        for (auto& dir : directions)
        {
            int nRow = current.n_rows + dir[0];
            int nCol = current.n_cols + dir[1];

            if (!m_grid.isValid(nRow, nCol) || visited[nRow][nCol])
                continue;

            Tile& neighbor = m_grid.getTile(nRow, nCol);

            // Skip obstacles
            if (!neighbor.isTraversable())
                continue;

            // Point toward current tile (to follow path to goal)
            sf::Vector2f dirVec(
                static_cast<float>(current.n_rows - nRow),
                static_cast<float>(current.n_cols - nCol)
            );

            float len = std::sqrt(dirVec.x * dirVec.x + dirVec.y * dirVec.y);
            if (len > 0.f)
                dirVec /= len;

            neighbor.setDirection(dirVec);
            visited[nRow][nCol] = true;
            vectorQueue.push({ nRow, nCol, 0.f });
        }
    }
}

/* -- Start from start point so we generate a cost grid based on the start and expanding outward.  -- */
//void Flowfield::generateCostField(const sf::Vector2i& t_startPos)
//{
//	// BFS Traversal queue
//	std::queue<Node> explorer;
//
//	/*					row		-  columns	-	Cost					*/
//	explorer.push({ t_startPos.x, t_startPos.y, 0.f });
//
//	// array of tiles we have visited - based on the rows and then the columns, which we will say have a value of -1 when visited to avoid repeat visits
//	std::vector<std::vector<float>> visited(
//		m_rows, std::vector<float>(m_cols, -1.f)
//	);
//
//	// Start at our chosen point
//	visited[t_startPos.x][t_startPos.y] = 0.f;
//
//	// List of explorable directions when at a node - currently: 4 options, consisting of 2 points - maps a direction
//	const int directions[4][2] = {
//		{1,0}, {-1,0}, { 0,1 }, {0, -1}
//	};
//
//
//
//	/// BFS Loop to go through each node
//	while (!explorer.empty())
//	{
//		// Grab the first node and remove it from the queue
//		Node l_current = explorer.front();
//		explorer.pop();
//
//		// Check our directions
//		for (auto& dir : directions)
//		{
//			int nodeRow = l_current.n_rows + dir[0];
//			int nodeCol = l_current.n_cols + dir[1];
//
//			if (!isValid(nodeRow, nodeCol))
//				continue;
//
//			// See if it has been visited yet - if its -1 it is default, i.e. unvisited
//			if (visited[nodeRow][nodeCol] == -1.f)
//			{
//				float l_newCost = l_current.n_cost + 1;
//
//				visited[nodeRow][nodeCol] = l_newCost;
//				explorer.push({ nodeRow, nodeCol, l_newCost });
//
//				// Skip obstacles
//				if (!m_grid.getTile(nodeRow, nodeCol).isTraversable())
//					continue;
//
//				// Update relevant Tile in the grid
//				Tile& tile = m_grid.getTile(nodeRow, nodeCol);
//				tile.setCost(l_newCost);
//				tile.setCostText(std::to_string(static_cast<int>(l_newCost)));
//
//				int l_colourHue = static_cast<int>(255 - std::min(l_newCost * 5, 200.f));
//				tile.setColour(sf::Color(l_colourHue, l_colourHue, 225.f));
//			}
//		}
//	}
//}

/* -- Starts at end point and pushes everything into the rest of the flowfield so it is directed to the goal; reverse search style -- */
//void Flowfield::generateVectorField(const sf::Vector2i& t_goalPos)
//{
//	// BFS queue for exploration
//	std::queue<Node> explorer;
//	explorer.push({ t_goalPos.x, t_goalPos.y, 0.f });
//
//	// Local visited array 
//	std::vector<std::vector<bool>> visited(
//		m_rows, std::vector<bool>(m_cols, false)
//	);
//	visited[t_goalPos.x][t_goalPos.y] = true;
//
//	// Directions (4-way for now, can expand later)
//	const int directions[4][2] = {
//		{1, 0}, {-1, 0}, {0, 1}, {0, -1}
//	};
//
//	while (!explorer.empty())
//	{
//		Node current = explorer.front();
//		explorer.pop();
//
//		for (auto& dir : directions)
//		{
//			int nodeRow = current.n_rows + dir[0];
//			int nodeCol = current.n_cols + dir[1];
//
//			if (!isValid(nodeRow, nodeCol) || visited[nodeRow][nodeCol])
//				continue;
//
//
//			// Skip obstacles
//			if (!m_grid.getTile(nodeRow, nodeCol).isTraversable())
//				continue;
//
//			Tile& neighbor = m_grid.getTile(nodeRow, nodeCol);
//
//			// Direction points toward current tile, so following it leads to goal
//			sf::Vector2f dirVector(
//				static_cast<float>(current.n_rows - nodeRow),
//				static_cast<float>(current.n_cols - nodeCol)
//			);
//
//			float length = std::sqrt(dirVector.x * dirVector.x + dirVector.y * dirVector.y);
//			if (length > 0.f)
//				dirVector /= length;
//
//			neighbor.setDirection(dirVector);
//
//			visited[nodeRow][nodeCol] = true;
//			explorer.push({ nodeRow, nodeCol, 0.f }); // Cost not used here
//		}
//	}
//}