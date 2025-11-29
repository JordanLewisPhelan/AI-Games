#include "GamePlayState.h"
#include "MainMenuState.h"
#include <iostream>

GamePlayState::GamePlayState(sf::Font* t_font, GameMode t_mode, AIDifficulty t_difficulty,
    std::shared_ptr<NetworkManager> t_networkManager)
    : GameState(t_font)
    , m_mode(t_mode)
    , m_aiDifficulty(t_difficulty)
    , m_networkManager(t_networkManager)
    , m_gameInfoText(*m_sharedFont)
    , m_pausedText(*m_sharedFont)
    , m_resumeButton(*m_sharedFont)
    , m_mainMenuButton(*m_sharedFont)
    , m_currentPlayer(1)
    , m_isPaused(false)
    , m_transitionRequested(false)
    , m_pauseHover(PauseButtonHover::None)
    , m_aiIsThinking(false)
{
    // Determine local player based on mode
    if (m_mode == GameMode::VsNetworked && m_networkManager) {
        // Host is Player1, Client is Player2
        m_localPlayer = m_networkManager->isHost() ? Player::Player1 : Player::Player2;
        m_currentTurn = Player::Player1;  // Player1 always goes first

        std::cout << "Network game: I am " << (m_localPlayer == Player::Player1 ? "Player 1" : "Player 2") << "\n";
    }
    else 
    {
        // For local modes, player controls Player1
        m_localPlayer = Player::Player1;
        m_currentTurn = Player::Player1;
    }

    // Create AI if in AI mode
    if (m_mode == GameMode::VsAI) 
    {
        m_ai = std::make_unique<AIOpponent>(m_aiDifficulty, Player::Player2);
        std::cout << "Created AI opponent with difficulty: " << (int)m_aiDifficulty << "\n";
    }

    // DEBUG: 
    if (m_mode == GameMode::VsNetworked && !m_networkManager) 
    {
        std::cout << "ERROR: Network mode requires NetworkManager!\n";
    }
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

    updateGameInfoText();

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
    if (m_isPaused || !m_winner.empty())
    {
        return;
    }

    // Check for winner after each move
    checkForWinner();

    // Update UI text
    updateGameInfoText();

    if (m_mode == GameMode::VsNetworked)
    {
        receiveAndApplyMoves();

        // Check for disconnection
        if (m_networkManager && !m_networkManager->isConnected())
        {
            std::cout << "Network connection lost!\n";
            m_winner = "Connection Lost";
        }
    }

    // AI logic
    //// AI is always player 2 currently, but adding a mode where the AI can be both means it may need a local 
    //// Awareness of what player is playing - so it is aware to wait - mild refactoring to 
    //// Opponent to ensure it doesnt try to just move player 2s pieces
    if (m_mode == GameMode::VsAI && m_board.getCurrentPlayer() == Player::Player2 && !m_aiIsThinking) 
    {
        // Small delay so player can see board state
        if (m_aiThinkTimer.getElapsedTime().asSeconds() > 1.7f) 
        {
            executeAIMove();
        }
    }
}


void GamePlayState::render(sf::RenderWindow& t_window)
{
    // Draw game info
    t_window.draw(m_gameInfoText);

    m_board.render(t_window, *m_sharedFont);

    // Draw pause menu if paused
    if (m_isPaused) 
    {
        renderPauseMenu(t_window);
    }

    // Draw winner text if game is over
    if (!m_winner.empty()) 
    {
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
            m_nextState = std::make_unique<MainMenuState>(m_sharedFont, m_networkManager);
            m_transitionRequested = true;
        }
        return;
    }

    if (!isMyTurn()) 
        return;


    // Handle mouse clicks for game interaction
    if (const auto* mousePress = t_event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePress->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(static_cast<float>(mousePress->position.x),
                static_cast<float>(mousePress->position.y));

            // PLACEMENT PHASE
            if (m_board.isPlacementPhase())
            {
                // First check if clicking on unplaced piece selector
                PieceType clickedPiece = m_board.getClickedUnplacedPiece(mousePos, m_board.getCurrentPlayer());
                if (clickedPiece != PieceType::None)
                {
                    m_board.selectPieceType(clickedPiece);
                    std::cout << "Selected piece type: " << (int)clickedPiece << "\n";
                }
                // Then check if clicking on board to place
                else if (m_board.isPositionOnBoard(mousePos))
                {
                    auto [row, col] = m_board.screenToGrid(mousePos);
                    PieceType selectedType = m_board.getSelectedPieceType();

                    if (selectedType != PieceType::None)
                    {
                        if (m_board.placePiece(row, col, selectedType, m_board.getCurrentPlayer()))
                        {
                            std::cout << "Placed piece at (" << row << ", " << col << ")\n";

                            if (m_mode == GameMode::VsNetworked)
                            {
                                // Send move over network
                                std::string pieceTypeName;
                                switch (selectedType) {
                                case PieceType::Donkey: pieceTypeName = "Donkey"; break;
                                case PieceType::Snake: pieceTypeName = "Snake"; break;
                                case PieceType::Frog: pieceTypeName = "Frog"; break;
                                default: pieceTypeName = "Unknown"; break;
                                }

                                std::string message = "PLACE|" + pieceTypeName + "|" +
                                    std::to_string(row) + "|" + std::to_string(col);

                                sendMove(message);
                            }

                            // Switch turn to opponent
                            m_currentTurn = (m_currentTurn == Player::Player1) ? Player::Player2 : Player::Player1;


                            // Reset selection after successful placement
                            m_board.selectPieceType(PieceType::None);
                        }
                        else
                        {
                            std::cout << "Cannot place piece at (" << row << ", " << col << ")\n";
                        }
                    }
                    else
                    {
                        std::cout << "No piece type selected!\n";
                    }
                }
            }

            // MOVEMENT PHASE
            else
            {
                if (m_board.isPositionOnBoard(mousePos))
                {
                    auto [row, col] = m_board.screenToGrid(mousePos);

                    // If no piece is selected, try to select one
                    if (!m_board.isPieceSelected())
                    {
                        if (m_board.selectPieceForMove(row, col))
                        {
                            std::cout << "Selected piece at (" << row << ", " << col << ")\n";
                        }
                    }
                    // If piece is selected, try to move it
                    else
                    {
                        if (m_board.movePiece(row, col))
                        {
                            std::cout << "Moved piece to (" << row << ", " << col << ")\n";

                            if (m_mode == GameMode::VsNetworked)
                            {
                                std::string message = "MOVE|" + std::to_string(row) + "|" +
                                    std::to_string(col) + "|" +
                                    std::to_string(row) + "|" + std::to_string(col);

                                sendMove(message);
                            }

                            // Switch turn to opponent
                            m_currentTurn = (m_currentTurn == Player::Player1) ? Player::Player2 : Player::Player1;

                        }
                        else
                        {
                            // Click elsewhere - deselect or select new piece
                            m_board.deselectPiece();
                            m_board.selectPieceForMove(row, col);
                        }
                    }
                }
            }
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
                m_nextState = std::make_unique<MainMenuState>(m_sharedFont, m_networkManager);
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

void GamePlayState::updateGameInfoText()
{
    std::string modeText;
    if (m_mode == GameMode::VsAI)
    {
        modeText = "vs AI";
    }
    else if (m_mode == GameMode::VsPvP) 
    {
        modeText = "PvP";
    }
    else if (m_mode == GameMode::VsNetworked)
    {
        modeText = "Network";
    }

    std::string playerText = (m_board.getCurrentPlayer() == Player::Player1) ? "Player 1" : "Player 2";
    std::string phaseText = m_board.isPlacementPhase() ? "PLACEMENT" : "MOVEMENT";

    std::string info = "Mode: " + modeText + " | " + playerText + "'s Turn | Phase: " + phaseText;

    // Add turn indicator for network mode
    if (m_mode == GameMode::VsNetworked) 
    {
        std::string youAre = (m_localPlayer == Player::Player1) ? "You are Player 1" : "You are Player 2";
        info = youAre + " | " + info;

        if (isMyTurn()) 
        {
            info += " | YOUR TURN";
        }
        else {
            info += " | Waiting for opponent...";
        }
    }

    if (m_board.isPlacementPhase())
    {
        info += " | Click piece, then click board";
    }
    else 
    {
        info += " | Click piece, then click destination";
    }

    info += " | ESC to pause";

    m_gameInfoText.setString(info);
}

void GamePlayState::checkForWinner()
{
    if (m_winner.empty() && !m_board.isPlacementPhase()) 
    {
        if (m_board.checkWin(Player::Player1)) 
        {
            m_winner = "Player 1";
            std::cout << "Player 1 wins!\n";

            if (m_mode == GameMode::VsNetworked)
            {
                sendMove("WIN|Player 1");
            }
        }
        else if (m_board.checkWin(Player::Player2)) 
        {
            m_winner = "Player 2";
            std::cout << "Player 2 wins!\n";

            if (m_mode == GameMode::VsNetworked)
            {
                sendMove("WIN|Player 2"); 
            }
        }
    }
}

// AI move

void GamePlayState::executeAIMove()
{
    if (!m_ai || m_aiIsThinking) return;

    m_aiIsThinking = true;
    std::cout << "=== AI TURN ===" << "\n";

    AIMove move = m_ai->calculateBestMove(m_board);

    if (move.isInPlacement) 
    {
        if (m_board.placePiece(move.row, move.col, move.pieceType, Player::Player2)) {
            std::cout << "AI placed " << (int)move.pieceType << " at (" << move.row << ", " << move.col << ")\n";
        }
        else 
        {
            std::cout << "ERROR: AI tried invalid placement!\n";
        }
    }
    else {
        if (m_board.selectPieceForMove(move.fromRow, move.fromCol)) 
        {
            if (m_board.movePiece(move.toRow, move.toCol)) 
            {
                std::cout << "AI moved from (" << move.fromRow << ", " << move.fromCol << ") to (" << move.toRow << ", " << move.toCol << ")\n";
            }
            else 
            {
                std::cout << "ERROR: AI tried invalid move!\n";
            }
        }
        else 
        {
            std::cout << "ERROR: AI couldn't select piece!\n";
        }
    }

    m_aiIsThinking = false;
    m_aiThinkTimer.restart();
}




bool GamePlayState::isMyTurn() const
{
    if (m_mode == GameMode::VsNetworked)
        return m_localPlayer == m_currentTurn;

    return true;
}


void GamePlayState::sendMove(const std::string& t_message)
{
    if (m_mode == GameMode::VsNetworked && m_networkManager)
    {
        if (!m_networkManager->sendMessage(t_message))
        {
            std::cout << "ERROR: Failed to send move. \n";
        }

    }
}


void GamePlayState::receiveAndApplyMoves()
{
    if (m_mode != GameMode::VsNetworked || !m_networkManager) 
    {
        return;
    }

    // Check for incoming messages
    auto messageOpt = m_networkManager->receiveMessage();

    if (messageOpt.has_value()) 
    {
        std::string message = messageOpt.value();
        applyOpponentMove(message);
    }
}


void GamePlayState::applyOpponentMove(const std::string& t_message)
{
    std::cout << "Applying opponent move: " << t_message << "\n";

    // Parse message format: "PLACE|PieceType|Row|Col" or "MOVE|FromRow|FromCol|ToRow|ToCol"
    size_t firstDelim = t_message.find('|');

    if (firstDelim == std::string::npos)
    {
        std::cout << "ERROR: Invalid message format\n";
        return;
    }

    std::string messageType = t_message.substr(0, firstDelim);

    if (messageType == "PLACE") 
    {
        // Parse: PLACE|Type|Row|Col
        size_t secondDelim = t_message.find('|', firstDelim + 1);
        size_t thirdDelim = t_message.find('|', secondDelim + 1);

        if (secondDelim == std::string::npos || thirdDelim == std::string::npos) 
        {
            std::cout << "ERROR: Invalid PLACE message\n";
            return;
        }

        std::string typeStr = t_message.substr(firstDelim + 1, secondDelim - firstDelim - 1);
        std::string rowStr = t_message.substr(secondDelim + 1, thirdDelim - secondDelim - 1);
        std::string colStr = t_message.substr(thirdDelim + 1);

        // Convert type string to PieceType
        PieceType type = PieceType::None;

        if (typeStr == "Donkey") type = PieceType::Donkey;
        else if (typeStr == "Snake") type = PieceType::Snake;
        else if (typeStr == "Frog") type = PieceType::Frog;

        int row = std::stoi(rowStr);
        int col = std::stoi(colStr);

        // Apply opponent's placement
        Player opponentPlayer = (m_localPlayer == Player::Player1) ? Player::Player2 : Player::Player1;

        if (m_board.placePiece(row, col, type, opponentPlayer)) 
        {
            std::cout << "Opponent placed " << typeStr << " at (" << row << ", " << col << ")\n";

            // Switch turn back to local player
            m_currentTurn = m_localPlayer;
        }
        else 
        {
            std::cout << "ERROR: Opponent's move was invalid!\n";
        }
    }
    else if (messageType == "MOVE")
    {
        // Parse: MOVE|FromRow|FromCol|ToRow|ToCol
        size_t secondDelim = t_message.find('|', firstDelim + 1);
        size_t thirdDelim = t_message.find('|', secondDelim + 1);
        size_t fourthDelim = t_message.find('|', thirdDelim + 1);

        if (secondDelim == std::string::npos || thirdDelim == std::string::npos || fourthDelim == std::string::npos)
        {
            std::cout << "ERROR: Invalid MOVE message\n";
            return;
        }

        int fromRow = std::stoi(t_message.substr(firstDelim + 1, secondDelim - firstDelim - 1));
        int fromCol = std::stoi(t_message.substr(secondDelim + 1, thirdDelim - secondDelim - 1));
        int toRow = std::stoi(t_message.substr(thirdDelim + 1, fourthDelim - thirdDelim - 1));
        int toCol = std::stoi(t_message.substr(fourthDelim + 1));

        // Apply opponent's move
        if (m_board.selectPieceForMove(fromRow, fromCol)) 
        {
            if (m_board.movePiece(toRow, toCol)) 
            {
                std::cout << "Opponent moved from (" << fromRow << ", " << fromCol << ") to (" << toRow << ", " << toCol << ")\n";

                // Switch turn back to local player
                m_currentTurn = m_localPlayer;
            }
            else 
            {
                std::cout << "ERROR: Opponent's move destination was invalid!\n";
            }
        }
        else 
        {
            std::cout << "ERROR: Opponent couldn't select piece!\n";
        }
    }
    else if (messageType == "WIN") 
    {
        // Opponent won
        std::string winnerName = t_message.substr(firstDelim + 1);
        m_winner = winnerName;
        std::cout << "Opponent won!\n";
    }
}
