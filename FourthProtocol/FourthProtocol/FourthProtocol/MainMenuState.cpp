#include "MainMenuState.h"
#include "GamePlayState.h"
#include <iostream>

MainMenuState::MainMenuState(sf::Font* t_font)
    : GameState(t_font)
    , m_titleText(*m_sharedFont)
    , m_vsAIButton(*m_sharedFont)
    , m_vsPvPButton(*m_sharedFont)
    , m_quitButton(*m_sharedFont)
    , m_easyText(*m_sharedFont)      
    , m_mediumText(*m_sharedFont)    
    , m_hardText(*m_sharedFont)    
    , m_selectedDifficulty(AIDifficulty::Medium)
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

    // Setup difficulty buttons (to the right of Play vs AI)
    float difficultyX = 550.f;
    float difficultyY = 305.f;
    float buttonSize = 35.f;
    float buttonSpacing = 45.f;

    // Easy button (Green)
    m_easyButton.setSize(sf::Vector2f(buttonSize, buttonSize));
    m_easyButton.setPosition(sf::Vector2(difficultyX, difficultyY));
    m_easyButton.setOutlineColor(sf::Color::White);

    m_easyText.setString("E");
    m_easyText.setCharacterSize(20);
    m_easyText.setFillColor(sf::Color::White);
    m_easyText.setPosition(sf::Vector2f(difficultyX + 10.f, difficultyY + 5.f));

    // Medium button (Yellow)
    m_mediumButton.setSize(sf::Vector2f(buttonSize, buttonSize));
    m_mediumButton.setPosition(sf::Vector2f(difficultyX + buttonSpacing, difficultyY));
    m_mediumButton.setOutlineColor(sf::Color::White);

    m_mediumText.setString("M");
    m_mediumText.setCharacterSize(20);
    m_mediumText.setFillColor(sf::Color::White);
    m_mediumText.setPosition(sf::Vector2f(difficultyX + buttonSpacing + 8.f, difficultyY + 5.f));

    // Hard button (Red)
    m_hardButton.setSize(sf::Vector2f(buttonSize, buttonSize));
    m_hardButton.setPosition(sf::Vector2f(difficultyX + buttonSpacing * 2, difficultyY));
    m_hardButton.setOutlineColor(sf::Color::White);

    m_hardText.setString("H");
    m_hardText.setCharacterSize(20);
    m_hardText.setFillColor(sf::Color::White);
    m_hardText.setPosition(sf::Vector2f(difficultyX + buttonSpacing * 2 + 9.f, difficultyY + 5.f));



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
    
    
    // Drawing difficulty buttons
    m_easyButton.setFillColor(getDifficultyButtonColor(
        AIDifficulty::Easy,
        m_selectedDifficulty == AIDifficulty::Easy,
        m_currentHover == ButtonHover::Easy
    ));
    m_easyButton.setOutlineThickness(m_selectedDifficulty == AIDifficulty::Easy ? 3.f : 1.f);
    t_window.draw(m_easyButton);
    t_window.draw(m_easyText);

    m_mediumButton.setFillColor(getDifficultyButtonColor(
        AIDifficulty::Medium,
        m_selectedDifficulty == AIDifficulty::Medium,
        m_currentHover == ButtonHover::Medium
    ));
    m_mediumButton.setOutlineThickness(m_selectedDifficulty == AIDifficulty::Medium ? 3.f : 1.f);
    t_window.draw(m_mediumButton);
    t_window.draw(m_mediumText);

    m_hardButton.setFillColor(getDifficultyButtonColor(
        AIDifficulty::Hard,
        m_selectedDifficulty == AIDifficulty::Hard,
        m_currentHover == ButtonHover::Hard
    ));
    m_hardButton.setOutlineThickness(m_selectedDifficulty == AIDifficulty::Hard ? 3.f : 1.f);
    t_window.draw(m_hardButton);
    t_window.draw(m_hardText);
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
        // Check what difficulty is being hovered over
        else if (isMouseOverRectShape(m_easyButton, mousePos)) {
            m_currentHover = ButtonHover::Easy;
        }
        else if (isMouseOverRectShape(m_mediumButton, mousePos)) {
            m_currentHover = ButtonHover::Medium;
        }
        else if (isMouseOverRectShape(m_hardButton, mousePos)) {
            m_currentHover = ButtonHover::Hard;
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

            if (isMouseOverRectShape(m_easyButton, mousePos)) {
                m_selectedDifficulty = AIDifficulty::Easy;
                std::cout << "Selected difficulty: Easy\n";
            }
            else if (isMouseOverRectShape(m_mediumButton, mousePos)) {
                m_selectedDifficulty = AIDifficulty::Medium;
                std::cout << "Selected difficulty: Medium\n";
            }
            else if (isMouseOverRectShape(m_hardButton, mousePos)) {
                m_selectedDifficulty = AIDifficulty::Hard;
                std::cout << "Selected difficulty: Hard\n";
            }

            else if (isMouseOver(m_vsAIButton, mousePos))
            {
                std::cout << "Starting game vs AI\n";
                m_nextState = std::make_unique<GamePlayState>(m_sharedFont, GameMode::VsAI, m_selectedDifficulty);
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

bool MainMenuState::isMouseOverRectShape(const sf::RectangleShape& t_shape, sf::Vector2f t_mousePos) const
{
    return t_shape.getGlobalBounds().contains(t_mousePos);
}


sf::Color MainMenuState::getDifficultyButtonColor(AIDifficulty t_difficulty, bool t_isSelected, bool t_isHovered) const
{
    // Base colors for each difficulty
    sf::Color baseColor;
    switch (t_difficulty) 
    {
    case AIDifficulty::Easy:   baseColor = sf::Color(100.f, 255.f, 150.f); break;
    case AIDifficulty::Medium: baseColor = sf::Color(200.f, 200.f, 50.f); break;
    case AIDifficulty::Hard:   baseColor = sf::Color(255.f, 100.f, 100.f); break;
    }

    // If selected, make brighter
    if (t_isSelected)
    {
        return baseColor;
    }

    // If hovered (but not selected), make slightly brighter
    if (t_isHovered) 
    {
        return sf::Color(
            std::min(255, baseColor.r + 50),
            std::min(255, baseColor.g + 50),
            std::min(255, baseColor.b + 50)
        );
    }

    // Otherwise, make darker
    return sf::Color(baseColor.r / 2, baseColor.g / 2, baseColor.b / 2);
}
