/// <summary>
/// author Pete Lowe May 2025
/// you need to change the above line or lose marks
/// </summary>


#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the texts
/// load and setup the images
/// load and setup the sounds
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{1200U, 1000U}, 32U }, "SFML Game 3.0" },
	m_DELETEexitGame{false}, //when true game will exit
	m_grid(50, 50),
	m_flow(m_grid),
	m_npc(m_grid.getStartTile(), m_grid)
{
	setupTexts(); // load font 
	setupSprites(); // load texture
	setupAudio(); // load sounds
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if ( newEvent->is<sf::Event::Closed>()) // close window message 
		{
			m_DELETEexitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>()) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (newEvent->is<sf::Event::MouseButtonReleased>())	
		{
			const auto* mouseEvent = newEvent->getIf<sf::Event::MouseButtonReleased>();
			handleMouseClick(*mouseEvent);
		}
	}
}
/*
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
	sf::Vector2i gridPos(
		mousePos.y / Utilities::TILE_SIZE,   // row
		mousePos.x / Utilities::TILE_SIZE    // col
	);
*/

/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		m_DELETEexitGame = true; 
	}

	if (sf::Keyboard::Key::G == newKeypress->code)
	{
		m_grid.getTile(25, 9);	// Debugging
	}

	if (sf::Keyboard::Key::C == newKeypress->code)
		m_grid.toggleDebugCost();  // toggle cost overlay

	if (sf::Keyboard::Key::V == newKeypress->code)
		m_grid.toggleDebugVector();  // toggle vector overlay



}

/// <summary>
/// Check if any keys are currently pressed
/// </summary>
void Game::checkKeyboardState()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_DELETEexitGame = true; 
	}
}

void Game::handleMouseClick(const sf::Event::MouseButtonReleased& t_mouseEvent)
{
	// Mouse position in window
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);

	// Top-left of the grid on the screen
	sf::Vector2f gridOrigin(
		(m_window.getSize().x - (m_grid.getCols() * Utilities::TILE_SIZE)) / 2.f,
		(m_window.getSize().y - (m_grid.getRows() * Utilities::TILE_SIZE)) / 2.f
	);

	// Relative position inside the grid area
	float relativeX = mousePos.x - gridOrigin.x;
	float relativeY = mousePos.y - gridOrigin.y;

	// Check if click is inside the grid area first
	if (relativeX < 0 || relativeY < 0)
		return; // click outside, ignore


	// Convert pixel -> grid indices (row, col)
	int row = static_cast<int>((mousePos.x - gridOrigin.x) / Utilities::TILE_SIZE);
	int col = static_cast<int>((mousePos.y - gridOrigin.y) / Utilities::TILE_SIZE);

	// Clamp indices to valid range
	row = std::clamp(row, 0, m_grid.getRows() - 1);
	col = std::clamp(col, 0, m_grid.getCols() - 1);

	sf::Vector2i gridPos(row, col);

	Tile& tile = m_grid.getTile(gridPos.x, gridPos.y);

	// Left click -> start tile
	if (t_mouseEvent.button == sf::Mouse::Button::Left)
	{
		m_grid.setStartTile(gridPos);
		m_npc.pathToTraverse(m_grid.getStartTile(), gridOrigin);	// Only need to update position when we start, will always move towards end
		std::cout << "Start set at: " << gridPos.x << ", " << gridPos.y << "\n";
	}
	// Right click -> end tile
	else if (t_mouseEvent.button == sf::Mouse::Button::Right)
	{
		m_grid.setEndTile(gridPos);
		std::cout << "Goal set at: " << gridPos.x << ", " << gridPos.y << "\n";
	}
	else if (t_mouseEvent.button == sf::Mouse::Button::Middle)
	{
		// Toggle obstacle
		tile.toggleTraversable();

		// Set a visual cue
		if (!tile.isTraversable())
			tile.setColour(sf::Color(sf::Color::Black)); // dark for obstacle
		else
			tile.setColour(sf::Color::White); // back to normal

		// Optional: regenerate flowfield after toggling
		m_flow.generateHybridFlowfield(m_grid.getStartTile(), m_grid.getEndTile());
	}

	// Regenerate flowfield after changes
	m_flow.generateHybridFlowfield(m_grid.getStartTile(), m_grid.getEndTile());
}





/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	float dt = t_deltaTime.asSeconds();
	checkKeyboardState();

	m_npc.update(dt);

	if (m_DELETEexitGame)
	{
		m_window.close();
	}
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(ULTRAMARINE);

	m_grid.render(m_window);
	m_npc.render(m_window);
	
	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupTexts()
{

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprites()
{

}

/// <summary>
/// load sound file and assign buffers
/// </summary>
void Game::setupAudio()
{

}


