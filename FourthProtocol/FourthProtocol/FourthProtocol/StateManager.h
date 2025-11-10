#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "GameState.h"


class StateManager {
private:
    std::unique_ptr<GameState> m_currentState;

public:
    StateManager();

    void changeState(std::unique_ptr<GameState> t_newState);
    void update(sf::Time t_deltaTime);
    void render(sf::RenderWindow& t_window);
    void handleEvent(const sf::Event& t_event);

    // Check if we have no state (quit signal)
    bool hasNoState() const;
};