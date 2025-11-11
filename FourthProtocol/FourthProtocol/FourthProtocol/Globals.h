#pragma once

struct Globals {

	static unsigned constexpr SCREEN_WIDTH = 1000.f;
	static unsigned constexpr SCREEN_HEIGHT = 800.f;

    // Board based globals - Used in globals should other areas need them(as of structuring i 
    // do believe knowing where the UI will be is fairly handy)
    static constexpr int GRID_SIZE = 5;
    static constexpr float CELL_SIZE = 80.f;
    static constexpr float BOARD_OFFSET_X = 200.f;
    static constexpr float BOARD_OFFSET_Y = 150.f;
    static constexpr float UNPLACED_PANEL_X = 50.f;
    static constexpr float UNPLACED_PANEL_Y = 200.f;

};