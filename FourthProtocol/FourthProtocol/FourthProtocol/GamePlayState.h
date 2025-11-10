#pragma once
#include "GameState.h"
#include <SFML/Graphics.hpp>

enum class GameMode { VsAI, VsPvP };

class GamePlayState : public GameState {
private:
    GameMode m_mode;

    // Game data
    int m_currentPlayer;
    bool m_isPaused;
    std::string m_winner;

    // UI elements
    sf::Text m_gameInfoText;
    sf::Text m_pausedText;
    sf::Text m_resumeButton;
    sf::Text m_mainMenuButton;

    std::unique_ptr<GameState> m_nextState;
    bool m_transitionRequested;

    enum class PauseButtonHover { None, Resume, MainMenu };
    PauseButtonHover m_pauseHover;

    // Helper methods
    void renderPauseMenu(sf::RenderWindow& t_window);
    void handleGameInput(const sf::Event& t_event);
    void handlePauseInput(const sf::Event& t_event);
    bool isMouseOver(const sf::Text& t_text, sf::Vector2f t_mousePos) const;

public:
    GamePlayState(sf::Font* t_font, GameMode t_mode);

    void onEnter() override;
    void onExit() override;
    void update(sf::Time t_deltaTime) override;
    void render(sf::RenderWindow& t_window) override;
    void handleEvent(const sf::Event& t_event) override;
    bool shouldTransition() const override;
    std::unique_ptr<GameState> getNextState() override;
};