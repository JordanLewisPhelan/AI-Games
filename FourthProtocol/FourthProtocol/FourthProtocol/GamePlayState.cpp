#include "GamePlayState.h"
#include "MainMenuState.h"
#include <iostream>

GamePlayState::GamePlayState(sf::Font* t_font, GameMode t_mode)
    : GameState(t_font)
    , m_mode(t_mode)
    , m_gameInfoText(*m_sharedFont)
    , m_pausedText(*m_sharedFont)
    , m_resumeButton(*m_sharedFont)
    , m_mainMenuButton(*m_sharedFont)
    , m_currentPlayer(1)
    , m_isPaused(false)
    , m_transitionRequested(false)
    , m_pauseHover(PauseButtonHover::None)
{
}

void GamePlayState::onEnter()
{
    std::cout << "Entering Gameplay State - Mode: "
        << (m_mode == GameMode::VsAI ? "AI" : "PvP") << "\n";

    // Initialize fresh game
    m_currentPlayer = 1;
    m_winner = "";
    m_isPaused = false;

    // Setup game info text
    m_gameInfoText.setCharacterSize(28);
    m_gameInfoText.setFillColor(sf::Color::White);
    m_gameInfoText.setPosition(sf::Vector2f(50.f, 50.f));

    if (m_mode == GameMode::VsAI) {
        m_gameInfoText.setString("Mode: vs AI | Player 1's Turn | Press ESC to pause");
    }
    else {
        m_gameInfoText.setString("Mode: PvP | Player 1's Turn | Press ESC to pause");
    }

    // Setup pause menu texts
    m_pausedText.setString("PAUSED");
    m_pausedText.setCharacterSize(72);
    m_pausedText.setFillColor(sf::Color::White);
    m_pausedText.setPosition(sf::Vector2f(350.f, 200.f));

    m_resumeButton.setString("Resume");
    m_resumeButton.setCharacterSize(36);
    m_resumeButton.setFillColor(sf::Color::White);
    m_resumeButton.setPosition(sf::Vector2f(400.f, 350.f));

    m_mainMenuButton.setString("Main Menu");
    m_mainMenuButton.setCharacterSize(36);
    m_mainMenuButton.setFillColor(sf::Color::White);
    m_mainMenuButton.setPosition(sf::Vector2f(400.f, 450.f));
}

void GamePlayState::onExit()
{
    std::cout << "Exiting Gameplay State\n";
}

void GamePlayState::update(sf::Time t_deltaTime)
{
    if (m_isPaused) {
        return;
    }

    if (!m_winner.empty()) {
        return;
    }

    // TODO: Game logic here
}

void GamePlayState::render(sf::RenderWindow& t_window)
{
    // Draw game info
    t_window.draw(m_gameInfoText);

    // Draw placeholder board
    sf::RectangleShape boardPlaceholder(sf::Vector2f(600.f, 600.f));
    boardPlaceholder.setPosition(sf::Vector2f(212.f, 150.f));
    boardPlaceholder.setFillColor(sf::Color(100, 100, 100));
    boardPlaceholder.setOutlineColor(sf::Color::White);
    boardPlaceholder.setOutlineThickness(3.f);
    t_window.draw(boardPlaceholder);

    // Temp text for placeholder
    sf::Text placeholderText(*m_sharedFont);
    placeholderText.setString("Game Board Goes Here");
    placeholderText.setCharacterSize(32);
    placeholderText.setFillColor(sf::Color::White);
    placeholderText.setPosition(sf::Vector2f(350.f, 420.f));
    t_window.draw(placeholderText);

    // Draw pause menu if paused
    if (m_isPaused) {
        renderPauseMenu(t_window);
    }

    // Draw winner text if game is over
    if (!m_winner.empty()) {
        sf::Text winText(*m_sharedFont);
        winText.setString(m_winner + " Wins!");
        winText.setCharacterSize(64);
        winText.setFillColor(sf::Color::Green);
        winText.setPosition(sf::Vector2f(300.f, 300.f));
        t_window.draw(winText);

        sf::Text clickText(*m_sharedFont);
        clickText.setString("Click to return to menu");
        clickText.setCharacterSize(24);
        clickText.setFillColor(sf::Color::White);
        clickText.setPosition(sf::Vector2f(350.f, 400.f));
        t_window.draw(clickText);
    }
}

void GamePlayState::renderPauseMenu(sf::RenderWindow& t_window)
{
    // Semi-transparent overlay
    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(t_window.getSize().x),
        static_cast<float>(t_window.getSize().y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    t_window.draw(overlay);

    // Pause menu
    t_window.draw(m_pausedText);

    // Buttons with hover effect
    if (m_pauseHover == PauseButtonHover::Resume) {
        m_resumeButton.setFillColor(sf::Color::Yellow);
    }
    else {
        m_resumeButton.setFillColor(sf::Color::White);
    }
    t_window.draw(m_resumeButton);

    if (m_pauseHover == PauseButtonHover::MainMenu) {
        m_mainMenuButton.setFillColor(sf::Color::Yellow);
    }
    else {
        m_mainMenuButton.setFillColor(sf::Color::White);
    }
    t_window.draw(m_mainMenuButton);
}

void GamePlayState::handleEvent(const sf::Event& t_event)
{
    // ESC toggles pause
    if (const auto* keyPress = t_event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPress->code == sf::Keyboard::Key::Escape && m_winner.empty())
        {
            m_isPaused = !m_isPaused;
            std::cout << (m_isPaused ? "Game paused\n" : "Game resumed\n");
            return;
        }
    }

    if (m_isPaused) {
        handlePauseInput(t_event);
    }
    else {
        handleGameInput(t_event);
    }
}

void GamePlayState::handleGameInput(const sf::Event& t_event)
{
    // If game is over, any click returns to menu
    if (!m_winner.empty())
    {
        if (t_event.getIf<sf::Event::MouseButtonPressed>())
        {
            std::cout << "Returning to main menu\n";
            m_nextState = std::make_unique<MainMenuState>(m_sharedFont);
            m_transitionRequested = true;
        }
        return;
    }

    // Test: click to show winner
    if (t_event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (m_winner.empty()) {
            m_winner = "Player 1";
            std::cout << "Test: Setting winner to Player 1\n";
        }
    }
}

void GamePlayState::handlePauseInput(const sf::Event& t_event)
{
    // Handle mouse movement for hover
    if (const auto* mouseMove = t_event.getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2f mousePos(static_cast<float>(mouseMove->position.x),
            static_cast<float>(mouseMove->position.y));

        if (isMouseOver(m_resumeButton, mousePos)) {
            m_pauseHover = PauseButtonHover::Resume;
        }
        else if (isMouseOver(m_mainMenuButton, mousePos)) {
            m_pauseHover = PauseButtonHover::MainMenu;
        }
        else {
            m_pauseHover = PauseButtonHover::None;
        }
    }

    // Handle clicks
    if (const auto* mousePress = t_event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePress->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(static_cast<float>(mousePress->position.x),
                static_cast<float>(mousePress->position.y));

            if (isMouseOver(m_resumeButton, mousePos))
            {
                m_isPaused = false;
                std::cout << "Resuming game\n";
            }
            else if (isMouseOver(m_mainMenuButton, mousePos))
            {
                std::cout << "Returning to main menu from pause\n";
                m_nextState = std::make_unique<MainMenuState>(m_sharedFont);
                m_transitionRequested = true;
            }
        }
    }
}

bool GamePlayState::shouldTransition() const
{
    return m_transitionRequested;
}

std::unique_ptr<GameState> GamePlayState::getNextState()
{
    m_transitionRequested = false;
    return std::move(m_nextState);
}

bool GamePlayState::isMouseOver(const sf::Text& t_text, sf::Vector2f t_mousePos) const
{
    return t_text.getGlobalBounds().contains(t_mousePos);
}