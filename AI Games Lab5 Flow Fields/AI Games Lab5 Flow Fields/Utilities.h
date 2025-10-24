#pragma once


namespace Utilities
{
	/*
		This section is for general use global values
	*/

	// Using inline to prevent other similarly named - not that it should happen but for safety - and its a nifty fun fact i just learned
	inline constexpr float TILE_SIZE = 15.0f;    // Constexpr Inline so that all tiles use the same value and data
	inline constexpr float WINDOW_WIDTH = 1200.0f;	// Window Dimensions
	inline constexpr float WINDOW_HEIGHT = 1000.0f;


	/*
		This section of the Utilities will cover Vector Maths
	*/



	/*
		This section will cover the Heat-Mapping for the Cost Field
		Personal Note:	Will probably be an interesting rabbit hole want to be able to reuse this later on independently
	*/
}