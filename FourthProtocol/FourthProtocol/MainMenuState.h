#pragma once
#include "GameState.h"
#include "NetworkManager.h"
#include <SFML/Graphics.hpp>

enum class AIDifficulty;

class MainMenuState : public GameState {
private:
    sf::Text m_titleText;
    sf::Text m_vsAIButton;
    sf::Text m_vsPvPButton;
    sf::Text m_quitButton;

    // Difficulty selector variables
    sf::RectangleShape m_easyButton;
    sf::RectangleShape m_mediumButton;
    sf::RectangleShape m_hardButton;
    sf::Text m_easyText;
    sf::Text m_mediumText;
    sf::Text m_hardText;
    sf::Text m_hostGameButton;    
    sf::Text m_joinGameButton;

    AIDifficulty m_selectedDifficulty;

    std::shared_ptr<NetworkManager> m_networkManager;
    std::unique_ptr<GameState> m_nextState;
    bool m_transitionRequested;

    enum class ButtonHover { None, VsAI, VsPvP, HostGame, JoinGame, Quit, Easy, Medium, Hard };
    ButtonHover m_currentHover;

    bool isMouseOver(const sf::Text& t_text, sf::Vector2f t_mousePos) const;
    bool isMouseOverRectShape(const sf::RectangleShape& shape, sf::Vector2f mousePos) const;

    sf::Color getDifficultyButtonColor(AIDifficulty difficulty, bool isSelected, bool isHovered) const;

public:
    MainMenuState(sf::Font* t_font, std::shared_ptr<NetworkManager> t_networkManager);

    void onEnter() override;
    void onExit() override;
    void update(sf::Time t_deltaTime) override;
    void render(sf::RenderWindow& t_window) override;
    void handleEvent(const sf::Event& t_event) override;
    bool shouldTransition() const override;
    std::unique_ptr<GameState> getNextState() override;
};