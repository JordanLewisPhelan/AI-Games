#include "LobbyState.h"
#include "GamePlayState.h"
#include "MainMenuState.h"
#include <iostream>

LobbyState::LobbyState(sf::Font *t_font, std::shared_ptr<NetworkManager> t_networkManager, bool t_isHost)
    : GameState(t_font)
    , m_networkManager(t_networkManager)
    , m_isHost(t_isHost)
    , m_titleText(*m_sharedFont)
    , m_statusText(*m_sharedFont)
    , m_instructionText(*m_sharedFont)
    , m_transitionRequested(false)
    , m_connectionEstablished(false)
{
}

void LobbyState::onEnter()
{
    std::cout << "Entering Lobby State\n";

    // Title
    m_titleText.setString("Network Lobby");
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(sf::Vector2f(350.f, 100.f));

    // Status
    m_statusText.setCharacterSize(32);
    m_statusText.setFillColor(sf::Color::Yellow);
    m_statusText.setPosition(sf::Vector2f(250.f, 250.f));

    if (m_isHost) 
    {
        m_statusText.setString("Waiting for opponent to connect...");

        // Show host's IP in console for easy sharing
        auto localIP = sf::IpAddress::getLocalAddress();
        if (localIP.has_value()) {
            std::cout << "\n========================================\n";
            std::cout << "Your IP Address: " << localIP.value().toString() << "\n";
            std::cout << "Tell your opponent to connect to this IP\n";
            std::cout << "========================================\n\n";
        }
    }
    else 
    {
        m_statusText.setString("Connecting to host...");
    }

    // Instructions
    m_instructionText.setString("Press SPACE to start game\nPress ESC to cancel");
    m_instructionText.setCharacterSize(24);
    m_instructionText.setFillColor(sf::Color(150, 150, 150));  // Gray initially
    m_instructionText.setPosition(sf::Vector2f(300.f, 400.f));
}

void LobbyState::onExit()
{
    std::cout << "Exiting Lobby State\n";
}

void LobbyState::update(sf::Time t_dt)
{
    // Update connection status
    m_networkManager->updateConnection();

    ConnectionState connState = m_networkManager->getConnectionState();

    // Update status text based on connection state
    if (connState == ConnectionState::Connected && !m_connectionEstablished) 
    {
        m_connectionEstablished = true;
        m_statusText.setString("Connected! Press SPACE to start");
        m_statusText.setFillColor(sf::Color::Green);
        m_instructionText.setFillColor(sf::Color::White);  // Make instructions visible

        // Send READY message
        m_networkManager->sendMessage("READY");
    }
    else if (connState == ConnectionState::Failed) 
    {
        std::cout << "Connection failed! Returning to menu\n";
        m_nextState = std::make_unique<MainMenuState>(m_sharedFont, m_networkManager);
        m_transitionRequested = true;
        return;
    }
    else if (connState == ConnectionState::Listening)
    {
        m_statusText.setString("Waiting for opponent...");
    }
    else if (connState == ConnectionState::Connecting)
    {
        m_statusText.setString("Connecting...");
    }

    // Check for messages
    auto messageOpt = m_networkManager->receiveMessage();
    if (messageOpt.has_value()) 
    {
        std::string message = messageOpt.value();

        if (message == "READY" || message == "PING")
        {
            std::cout << "Opponent is ready!\n";
            if (!m_connectionEstablished)
            {
                m_connectionEstablished = true;
                m_statusText.setString("Connected! Press SPACE to start");
                m_statusText.setFillColor(sf::Color::Green);
                m_instructionText.setFillColor(sf::Color::White);
            }
        }
        else if (message == "START") 
        {
            std::cout << "Game starting!\n";
            // Transition to game - pass NetworkManager!
            auto gameState = std::make_unique<GamePlayState>(
                m_sharedFont,
                GameMode::VsNetworked,
                AIDifficulty::Medium,
                m_networkManager
            );
            // TODO: Pass NetworkManager to GamePlayState
            m_nextState = std::move(gameState);
            m_transitionRequested = true;
        }
    }
}

void LobbyState::render(sf::RenderWindow& t_window)
{
    t_window.draw(m_titleText);
    t_window.draw(m_statusText);
    t_window.draw(m_instructionText);
}

void LobbyState::handleEvent(const sf::Event& t_event)
{
    if (const auto* keyPress = t_event.getIf<sf::Event::KeyPressed>())
    {
        // ESC to cancel and return to menu
        if (keyPress->code == sf::Keyboard::Key::Escape)
        {
            std::cout << "Canceling connection...\n";
            m_networkManager->disconnect();
            m_nextState = std::make_unique<MainMenuState>(m_sharedFont, m_networkManager);
            m_transitionRequested = true;
        }

        // SPACE to start (only if connected)
        if (keyPress->code == sf::Keyboard::Key::Space && m_connectionEstablished)
        {
            std::cout << "Starting game!\n";

            // Send START message
            m_networkManager->sendMessage("START");

            // Transition to game 
            auto gameState = std::make_unique<GamePlayState>(
                m_sharedFont,
                GameMode::VsNetworked,
                AIDifficulty::Medium,
                m_networkManager
            );
            m_nextState = std::move(gameState);
            m_transitionRequested = true;
        }
    }
}

bool LobbyState::shouldTransition() const
{
    return m_transitionRequested;
}

std::unique_ptr<GameState> LobbyState::getNextState()
{
    m_transitionRequested = false;
    return std::move(m_nextState);
}