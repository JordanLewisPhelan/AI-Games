
This is going to be a little different - This is a fresh project. So I can apply the fixes and adjustments I intended to make from the previous Labwork but couldn't act on due to time preventing refactoring being something I could focus on - e.g. Making a full Global Reusable math function for normalization of Vectors.

**Goals for this Assignment**
-- 
	1. Map should be Generated as tilesets - Requested size is 50x50.
	   
	2. Tiles should be ordered and have types - Mentionable due to request of having some tiles be traversable & others are not, setting up a system and not just a bool might be helpful.
	   
	3. As a requirement of Non Traversables some form of Obstacle will be necessary. - Even if it is just not traversable should be visually apparent.
	   
	4. User Input to make Tiles Goals & Starts positions is necessary - This is being noted as it incorporates a few details;
	   A. Player needs Input - But should only allow Placements to occur on Traversable tiles. Can return empty handed to end a loop should we have this occurence.
	   B. Player Must have an Entity of some kind but the Player chooses where it starts and where it goes, should be generated when moving begins.
	   C. Mentioned later but part of the same point - Player will need to path to the goal point from the start, so we must visually display the path the Entity will take to reach the Goal.
	   
	5. Additions to the Grid - Should be informative - As it is requested by atleast toggle to have a cost and vector field.
	   A. Cost would highlight how likely the path is to be used or moved too in terms of cost efficiency, we will show this by scaling the hue or intensity of the colour of the tile - e.g. Darker if its a High Cost to travel to i.e. not worth it or out of the way.
	   B. Vector field is much simpler, will direct the Entity to flow towards the goal point so will attempt to flow and direct the Entity along the best laid path.
	   
	   Notice: Investigate the Heat-Map and Vector field direction meaning, these might not be an accurate description or too crudely fundamental and not cover the purpose or extent of their actual value-use case.
	   
	   