#pragma once
#include "GameState.h"
#include "NetworkManager.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "GamePlayState.h"
#include <iostream>

class LobbyState : public GameState {
private:
    std::shared_ptr<NetworkManager> m_networkManager;
    bool m_isHost;

    sf::Text m_titleText;
    sf::Text m_statusText;
    sf::Text m_instructionText;

    std::unique_ptr<GameState> m_nextState;
    bool m_transitionRequested;

    // For host: check for connection
    // For client: already connected, just wait for START signal
    sf::Clock m_waitClock;
    bool m_connectionEstablished;

public:
    LobbyState(sf::Font* font, std::shared_ptr<NetworkManager> t_networkManager, bool t_isHost);

    void onEnter() override;
    void onExit() override;
    void update(sf::Time t_deltaTime) override;
    void render(sf::RenderWindow& t_window) override;
    void handleEvent(const sf::Event& t_event) override;
    bool shouldTransition() const override;
    std::unique_ptr<GameState> getNextState() override;
};