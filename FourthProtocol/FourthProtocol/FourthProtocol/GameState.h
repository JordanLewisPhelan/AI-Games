#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class GameState {
protected:
    sf::Font* m_sharedFont;

public:
    GameState(sf::Font* t_font) : m_sharedFont(t_font) {}
    virtual ~GameState() = default;

    // Lifecycle
    virtual void onEnter() = 0;
    virtual void onExit() = 0;

    // Per-frame
    virtual void update(sf::Time t_deltaTime) = 0;
    virtual void render(sf::RenderWindow& t_window) = 0;
    virtual void handleEvent(const sf::Event& t_event) = 0;

    // Transition control
    virtual bool shouldTransition() const { return false; }
    virtual std::unique_ptr<GameState> getNextState() { return nullptr; }
};