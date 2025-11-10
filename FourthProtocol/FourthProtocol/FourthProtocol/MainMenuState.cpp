#include "MainMenuState.h"
#include "GamePlayState.h"
#include <iostream>

MainMenuState::MainMenuState(sf::Font* t_font)
    : GameState(t_font)
    , m_titleText(*m_sharedFont)
    , m_vsAIButton(*m_sharedFont)
    , m_vsPvPButton(*m_sharedFont)
    , m_quitButton(*m_sharedFont)
    , m_transitionRequested(false)
    , m_currentHover(ButtonHover::None)
{
}

void MainMenuState::onEnter()
{
    std::cout << "Entering Main Menu State\n";

    // Setup title
    m_titleText.setString("Fourth Protocol");
    m_titleText.setCharacterSize(64);
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(sf::Vector2f(300.f, 100.f));

    // Setup "Play vs AI" button
    m_vsAIButton.setString("Play vs AI");
    m_vsAIButton.setCharacterSize(36);
    m_vsAIButton.setFillColor(sf::Color::White);
    m_vsAIButton.setPosition(sf::Vector2f(400.f, 300.f));

    // Setup "Play vs Player" button
    m_vsPvPButton.setString("Play vs Player");
    m_vsPvPButton.setCharacterSize(36);
    m_vsPvPButton.setFillColor(sf::Color::White);
    m_vsPvPButton.setPosition(sf::Vector2f(400.f, 400.f));

    // Setup "Quit" button
    m_quitButton.setString("Quit");
    m_quitButton.setCharacterSize(36);
    m_quitButton.setFillColor(sf::Color::White);
    m_quitButton.setPosition(sf::Vector2f(400.f, 500.f));
}

void MainMenuState::onExit()
{
    std::cout << "Exiting Main Menu State\n";
}

void MainMenuState::update(sf::Time t_deltaTime)
{
    // Main menu doesn't need much updating
}

void MainMenuState::render(sf::RenderWindow& t_window)
{
    // Draw title
    t_window.draw(m_titleText);

    // Draw buttons with hover effect
    if (m_currentHover == ButtonHover::VsAI) {
        m_vsAIButton.setFillColor(sf::Color::Yellow);
    }
    else {
        m_vsAIButton.setFillColor(sf::Color::White);
    }
    t_window.draw(m_vsAIButton);

    if (m_currentHover == ButtonHover::VsPvP) {
        m_vsPvPButton.setFillColor(sf::Color::Yellow);
    }
    else {
        m_vsPvPButton.setFillColor(sf::Color::White);
    }
    t_window.draw(m_vsPvPButton);

    if (m_currentHover == ButtonHover::Quit) {
        m_quitButton.setFillColor(sf::Color::Red);
    }
    else {
        m_quitButton.setFillColor(sf::Color::White);
    }
    t_window.draw(m_quitButton);
}

void MainMenuState::handleEvent(const sf::Event& t_event)
{
    // Handle mouse movement for hover effects
    if (const auto* mouseMove = t_event.getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2f mousePos(static_cast<float>(mouseMove->position.x),
            static_cast<float>(mouseMove->position.y));

        // Check which button is being hovered
        if (isMouseOver(m_vsAIButton, mousePos)) {
            m_currentHover = ButtonHover::VsAI;
        }
        else if (isMouseOver(m_vsPvPButton, mousePos)) {
            m_currentHover = ButtonHover::VsPvP;
        }
        else if (isMouseOver(m_quitButton, mousePos)) {
            m_currentHover = ButtonHover::Quit;
        }
        else {
            m_currentHover = ButtonHover::None;
        }
    }

    // Handle mouse clicks
    if (const auto* mousePress = t_event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePress->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(static_cast<float>(mousePress->position.x),
                static_cast<float>(mousePress->position.y));

            if (isMouseOver(m_vsAIButton, mousePos))
            {
                std::cout << "Starting game vs AI\n";
                m_nextState = std::make_unique<GamePlayState>(m_sharedFont, GameMode::VsAI);
                m_transitionRequested = true;
            }
            else if (isMouseOver(m_vsPvPButton, mousePos))
            {
                std::cout << "Starting game vs Player\n";
                m_nextState = std::make_unique<GamePlayState>(m_sharedFont, GameMode::VsPvP);
                m_transitionRequested = true;
            }
            else if (isMouseOver(m_quitButton, mousePos))
            {
                std::cout << "Quit button clicked\n";
                m_nextState = nullptr;
                m_transitionRequested = true;
            }
        }
    }
}

bool MainMenuState::shouldTransition() const
{
    return m_transitionRequested;
}

std::unique_ptr<GameState> MainMenuState::getNextState()
{
    m_transitionRequested = false;
    return std::move(m_nextState);
}

bool MainMenuState::isMouseOver(const sf::Text& t_text, sf::Vector2f t_mousePos) const
{
    return t_text.getGlobalBounds().contains(t_mousePos);
}