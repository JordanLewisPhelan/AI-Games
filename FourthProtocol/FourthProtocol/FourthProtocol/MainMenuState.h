#pragma once
#include "GameState.h"
#include <SFML/Graphics.hpp>

class MainMenuState : public GameState {
private:
    sf::Text m_titleText;
    sf::Text m_vsAIButton;
    sf::Text m_vsPvPButton;
    sf::Text m_quitButton;

    std::unique_ptr<GameState> m_nextState;
    bool m_transitionRequested;

    enum class ButtonHover { None, VsAI, VsPvP, Quit };
    ButtonHover m_currentHover;

    bool isMouseOver(const sf::Text& t_text, sf::Vector2f t_mousePos) const;

public:
    MainMenuState(sf::Font* t_font);

    void onEnter() override;
    void onExit() override;
    void update(sf::Time t_deltaTime) override;
    void render(sf::RenderWindow& t_window) override;
    void handleEvent(const sf::Event& t_event) override;
    bool shouldTransition() const override;
    std::unique_ptr<GameState> getNextState() override;
};