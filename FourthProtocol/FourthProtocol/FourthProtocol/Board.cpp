#include "Board.h"
#include <iostream>

Board::Board()
    : m_placementPhase(true)
    , m_currentPlayer(Player::Player1)
    , m_selectedPieceType(PieceType::None)
    , m_selectedRow(-1)
    , m_selectedCol(-1)
    , m_pieceSelected(false)
{
    // Initialize empty grid
    for (int row = 0; row < Globals::GRID_SIZE; ++row) 
    {
        for (int col = 0; col < Globals::GRID_SIZE; ++col)
        {
            m_grid[row][col] = Piece();
        }
    }
}

bool Board::isValidPosition(int row, int col) const
{
    return row >= 0 && row < Globals::GRID_SIZE && col >= 0 && col < Globals::GRID_SIZE;
}

sf::Color Board::getPlayerColor(Player player) const
{
    switch (player) 
    {
    case Player::Player1: return sf::Color::Blue;
    case Player::Player2: return sf::Color::Red;
    default: return sf::Color::White;
    }
}

sf::Color Board::getPieceColor(PieceType type, Player player) const
{
    sf::Color baseColor = getPlayerColor(player);

    // Slightly different shades for different pieces
    switch (type) {
    case PieceType::Donkey: return baseColor;
    case PieceType::Snake:
        // Darker shade
        return sf::Color(baseColor.r * 0.7f, baseColor.g * 0.7f, baseColor.b * 0.7f);
    case PieceType::Frog:
        // Lighter shade
        return sf::Color(
            std::min(255, (int)(baseColor.r * 1.3f)),
            std::min(255, (int)(baseColor.g * 1.3f)),
            std::min(255, (int)(baseColor.b * 1.3f))
        );
    default: return sf::Color::White;
    }
}

// ==================== PLACEMENT PHASE ====================

bool Board::canPlacePiece(int row, int col, PieceType type, Player player) const
{
    if (!m_placementPhase) return false;
    if (player != m_currentPlayer) return false;
    if (!isValidPosition(row, col)) return false;
    if (!m_grid[row][col].isEmpty()) return false;
    if (type == PieceType::None) return false;

    // Check if player has this piece available
    const PlayerPieces& pieces = (player == Player::Player1) ? m_player1Pieces : m_player2Pieces;
    return pieces.getPieceCount(type) > 0;
}

bool Board::placePiece(int row, int col, PieceType type, Player player)
{
    if (!canPlacePiece(row, col, type, player)) {
        return false;
    }

    // Place the piece
    m_grid[row][col] = Piece(type, player);

    // Decrement piece count
    PlayerPieces& pieces = (player == Player::Player1) ? m_player1Pieces : m_player2Pieces;

    switch (type)
    {
    case PieceType::Donkey: pieces.donkeys--; break;
    case PieceType::Snake: pieces.snakes--; break;
    case PieceType::Frog: pieces.frogs--; break;
    default: break;
    }

    std::cout << "Placed " << (int)type << " for " << (int)player << " at (" << row << ", " << col << ")\n";

    // Check if placement phase is complete
    if (m_player1Pieces.allPlaced() && m_player2Pieces.allPlaced()) 
    {
        m_placementPhase = false;
        m_currentPlayer = Player::Player1; // P1 starts movement phase
        std::cout << "Placement complete! Movement phase begins.\n";
    }
    else 
    {
        // Switch player
        switchPlayer();
    }

    return true;
}

void Board::selectPieceType(PieceType type)
{
    m_selectedPieceType = type;
    std::cout << "Selected piece type: " << (int)type << "\n";
}

bool Board::isPlacementComplete() const
{
    return !m_placementPhase;
}

// ==================== MOVEMENT PHASE ====================

std::vector<std::pair<int, int>> Board::getDonkeyMoves(int row, int col) const
{
    std::vector<std::pair<int, int>> moves;

    // 8 directions: up, down, left, right, and 4 diagonals
    int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},      // orthogonal
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}     // diagonal
    };

    for (auto& dir : directions) 
    {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if (isValidPosition(newRow, newCol) && m_grid[newRow][newCol].isEmpty()) 
        {
            moves.push_back({ newRow, newCol });
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> Board::getSnakeMoves(int row, int col) const
{
    std::vector<std::pair<int, int>> moves;

    // 4 diagonal directions only
    int directions[4][2] = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (auto& dir : directions) 
    {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if (isValidPosition(newRow, newCol) && m_grid[newRow][newCol].isEmpty()) 
        {
            moves.push_back({ newRow, newCol });
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> Board::getFrogMoves(int row, int col) const
{
    std::vector<std::pair<int, int>> moves;

    // 8 directions (can jump in any direction)
    int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},      // orthogonal
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}     // diagonal
    };

    for (auto& dir : directions) 
    {
        int adjacentRow = row + dir[0];
        int adjacentCol = col + dir[1];

        if (isValidPosition(adjacentRow, adjacentCol) && m_grid[adjacentRow][adjacentCol].isEmpty())
        {
            moves.push_back({ adjacentRow, adjacentCol });
        }

        // Check if there's a piece adjacent (to jump over)
        if (isValidPosition(adjacentRow, adjacentCol) && !m_grid[adjacentRow][adjacentCol].isEmpty()) 
        {

            // Check landing spot (2 spaces away)
            int landRow = row + dir[0] * 2;
            int landCol = col + dir[1] * 2;

            if (isValidPosition(landRow, landCol) && m_grid[landRow][landCol].isEmpty()) 
            {
                moves.push_back({ landRow, landCol });
            }
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> Board::getValidMoves(int row, int col) const
{
    if (m_placementPhase) return {};
    if (!isValidPosition(row, col)) return {};

    const Piece& piece = m_grid[row][col];
    if (piece.isEmpty()) return {};
    if (piece.owner != m_currentPlayer) return {};

    switch (piece.type) 
    {
    case PieceType::Donkey: return getDonkeyMoves(row, col);
    case PieceType::Snake: return getSnakeMoves(row, col);
    case PieceType::Frog: return getFrogMoves(row, col);
    default: return {};
    }
}

bool Board::selectPieceForMove(int row, int col)
{
    if (m_placementPhase) return false;
    if (!isValidPosition(row, col)) return false;

    const Piece& piece = m_grid[row][col];
    if (piece.isEmpty()) return false;
    if (piece.owner != m_currentPlayer) return false;

    // Check if piece has valid moves
    auto moves = getValidMoves(row, col);
    if (moves.empty())
    {
        std::cout << "Piece at (" << row << ", " << col << ") has no valid moves\n";
        return false;
    }

    m_selectedRow = row;
    m_selectedCol = col;
    m_pieceSelected = true;

    std::cout << "Selected piece at (" << row << ", " << col << ") with " << moves.size() << " valid moves\n";
    return true;
}

bool Board::movePiece(int toRow, int toCol)
{
    if (!m_pieceSelected) return false;
    if (!isValidPosition(toRow, toCol)) return false;

    // Check if target is a valid move
    auto validMoves = getValidMoves(m_selectedRow, m_selectedCol);
    bool isValidMove = false;
    for (const auto& move : validMoves) 
    {
        if (move.first == toRow && move.second == toCol)
        {
            isValidMove = true;
            break;
        }
    }

    if (!isValidMove) 
    {
        std::cout << "Invalid move to (" << toRow << ", " << toCol << ")\n";
        return false;
    }

    // Move the piece
    m_grid[toRow][toCol] = m_grid[m_selectedRow][m_selectedCol];
    m_grid[m_selectedRow][m_selectedCol] = Piece();

    std::cout << "Moved piece from (" << m_selectedRow << ", " << m_selectedCol << ") to (" << toRow << ", " << toCol << ")\n";

    // Deselect and switch player
    deselectPiece();
    switchPlayer();

    return true;
}

void Board::deselectPiece()
{
    m_pieceSelected = false;
    m_selectedRow = -1;
    m_selectedCol = -1;
}

// ==================== WIN CHECKING ====================

bool Board::checkDirection(int startRow, int startCol, int deltaRow, int deltaCol, Player player) const
{
    int count = 0;
    int row = startRow;
    int col = startCol;

    // Check up to 4 in a row
    while (isValidPosition(row, col) && count < 4) 
    {
        const Piece& piece = m_grid[row][col];

        // Early abort if not player's piece (alpha-beta pruning concept)
        if (piece.isEmpty() || piece.owner != player) 
        {
            return false;
        }

        count++;
        row += deltaRow;
        col += deltaCol;
    }

    return count == 4;
}

bool Board::checkWin(Player player) const
{
    // Check all possible starting positions for 4-in-a-row

    // Horizontal (check each row)
    for (int row = 0; row < Globals::GRID_SIZE; ++row)
    {
        for (int col = 0; col <= Globals::GRID_SIZE - 4; ++col)
        {
            if (checkDirection(row, col, 0, 1, player)) 
            {
                return true;
            }
        }
    }

    // Vertical (check each column)
    for (int col = 0; col < Globals::GRID_SIZE; ++col)
    {
        for (int row = 0; row <= Globals::GRID_SIZE - 4; ++row)
        {
            if (checkDirection(row, col, 1, 0, player)) 
            {
                return true;
            }
        }
    }

    // Diagonal (top-left to bottom-right)
    for (int row = 0; row <= Globals::GRID_SIZE - 4; ++row)
    {
        for (int col = 0; col <= Globals::GRID_SIZE - 4; ++col)
        {
            if (checkDirection(row, col, 1, 1, player))
            {
                return true;
            }
        }
    }

    // Diagonal (top-right to bottom-left)
    for (int row = 0; row <= Globals::GRID_SIZE - 4; ++row)
    {
        for (int col = 3; col < Globals::GRID_SIZE; ++col)
        {
            if (checkDirection(row, col, 1, -1, player))
            {
                return true;
            }
        }
    }

    return false;
}

// ==================== TURN MANAGEMENT ====================

void Board::switchPlayer()
{
    m_currentPlayer = (m_currentPlayer == Player::Player1) ? Player::Player2 : Player::Player1;
    std::cout << "Switched to " << (m_currentPlayer == Player::Player1 ? "Player 1" : "Player 2") << "\n";
}

// ==================== RENDERING ====================

void Board::drawGrid(sf::RenderWindow& window, sf::Vector2f position) const
{
    for (int row = 0; row < Globals::GRID_SIZE; ++row)
    {
        for (int col = 0; col < Globals::GRID_SIZE; ++col)
        {
            sf::RectangleShape cell(sf::Vector2f(Globals::CELL_SIZE, Globals::CELL_SIZE));
            cell.setPosition(sf::Vector2f(position.x + col * Globals::CELL_SIZE, position.y + row * Globals::CELL_SIZE));
            cell.setFillColor(sf::Color(60, 60, 60));
            cell.setOutlineColor(sf::Color::White);
            cell.setOutlineThickness(2.f);
            window.draw(cell);
        }
    }
}

void Board::drawPieces(sf::RenderWindow& window, sf::Vector2f position) const
{
    for (int row = 0; row < Globals::GRID_SIZE; ++row)
    {
        for (int col = 0; col < Globals::GRID_SIZE; ++col)
        {
            const Piece& piece = m_grid[row][col];
            if (!piece.isEmpty()) 
            {
                float x = position.x + col * Globals::CELL_SIZE + Globals::CELL_SIZE / 2.f;
                float y = position.y + row * Globals::CELL_SIZE + Globals::CELL_SIZE / 2.f;

                // Draw different shapes for different piece types
                sf::Color color = getPieceColor(piece.type, piece.owner);

                switch (piece.type) 
                {
                case PieceType::Donkey: 
                {
                    // Circle for Donkey
                    sf::CircleShape shape(Globals::CELL_SIZE * 0.35f);
                    shape.setOrigin(sf::Vector2f(Globals::CELL_SIZE * 0.35f, Globals::CELL_SIZE * 0.35f));
                    shape.setPosition(sf::Vector2f(x, y));
                    shape.setFillColor(color);
                    window.draw(shape);
                    break;
                }
                case PieceType::Snake: 
                {
                    // Triangle for Snake
                    sf::CircleShape shape(Globals::CELL_SIZE * 0.35f, 3);
                    shape.setOrigin(sf::Vector2f(Globals::CELL_SIZE * 0.35f, Globals::CELL_SIZE * 0.35f));
                    shape.setPosition(sf::Vector2f(x, y));
                    shape.setFillColor(color);
                    window.draw(shape);
                    break;
                }
                case PieceType::Frog: 
                {
                    // Square for Frog
                    sf::RectangleShape shape(sf::Vector2f(Globals::CELL_SIZE * 0.6f, Globals::CELL_SIZE * 0.6f));
                    shape.setOrigin(sf::Vector2f(Globals::CELL_SIZE * 0.3f, Globals::CELL_SIZE * 0.3f));
                    shape.setPosition(sf::Vector2f(x, y));
                    shape.setFillColor(color);
                    window.draw(shape);
                    break;
                }
                default: break;
                }

                // Highlight selected piece
                if (m_pieceSelected && row == m_selectedRow && col == m_selectedCol) 
                {
                    sf::CircleShape highlight(Globals::CELL_SIZE * 0.45f);
                    highlight.setOrigin(sf::Vector2f(Globals::CELL_SIZE * 0.45f, Globals::CELL_SIZE * 0.45f));
                    highlight.setPosition(sf::Vector2f(x, y));
                    highlight.setFillColor(sf::Color::Transparent);
                    highlight.setOutlineColor(sf::Color::Yellow);
                    highlight.setOutlineThickness(3.f);
                    window.draw(highlight);
                }
            }
        }
    }
}

void Board::drawUnplacedPieces(sf::RenderWindow& window, Player player, sf::Font& font) const
{
    if (!m_placementPhase) return;
    if (player != m_currentPlayer) return;

    const PlayerPieces& pieces = (player == Player::Player1) ? m_player1Pieces : m_player2Pieces;
    sf::Color playerColor = getPlayerColor(player);

    float yOffset = Globals::UNPLACED_PANEL_Y;
    float spacing = 100.f;

    // Draw Donkeys
    if (pieces.donkeys > 0) 
    {
        sf::CircleShape donkeyShape(30.f);
        donkeyShape.setPosition(sf::Vector2f(Globals::UNPLACED_PANEL_X, yOffset));
        donkeyShape.setFillColor(getPieceColor(PieceType::Donkey, player));
        donkeyShape.setOutlineColor(m_selectedPieceType == PieceType::Donkey ? sf::Color::Yellow : sf::Color::White);
        donkeyShape.setOutlineThickness(m_selectedPieceType == PieceType::Donkey ? 3.f : 1.f);
        window.draw(donkeyShape);

        sf::Text countText(font);
        countText.setString("x" + std::to_string(pieces.donkeys));
        countText.setCharacterSize(20);
        countText.setFillColor(sf::Color::White);
        countText.setPosition(sf::Vector2f(Globals::UNPLACED_PANEL_X + 70.f, yOffset + 15.f));
        window.draw(countText);

        yOffset += spacing;
    }

    // Draw Snakes
    if (pieces.snakes > 0) 
    {
        sf::CircleShape snakeShape(30.f, 3);
        snakeShape.setPosition(sf::Vector2f(Globals::UNPLACED_PANEL_X, yOffset));
        snakeShape.setFillColor(getPieceColor(PieceType::Snake, player));
        snakeShape.setOutlineColor(m_selectedPieceType == PieceType::Snake ? sf::Color::Yellow : sf::Color::White);
        snakeShape.setOutlineThickness(m_selectedPieceType == PieceType::Snake ? 3.f : 1.f);
        window.draw(snakeShape);

        sf::Text countText(font);
        countText.setString("x" + std::to_string(pieces.snakes));
        countText.setCharacterSize(20);
        countText.setFillColor(sf::Color::White);
        countText.setPosition(sf::Vector2f(Globals::UNPLACED_PANEL_X + 70.f, yOffset + 15.f));
        window.draw(countText);

        yOffset += spacing;
    }

    // Draw Frogs
    if (pieces.frogs > 0) 
    {
        sf::RectangleShape frogShape(sf::Vector2f(60.f, 60.f));
        frogShape.setPosition(sf::Vector2f(Globals::UNPLACED_PANEL_X, yOffset));
        frogShape.setFillColor(getPieceColor(PieceType::Frog, player));
        frogShape.setOutlineColor(m_selectedPieceType == PieceType::Frog ? sf::Color::Yellow : sf::Color::White);
        frogShape.setOutlineThickness(m_selectedPieceType == PieceType::Frog ? 3.f : 1.f);
        window.draw(frogShape);

        sf::Text countText(font);
        countText.setString("x" + std::to_string(pieces.frogs));
        countText.setCharacterSize(20);
        countText.setFillColor(sf::Color::White);
        countText.setPosition(sf::Vector2f(Globals::UNPLACED_PANEL_X + 70.f, yOffset + 15.f));
        window.draw(countText);
    }
}

void Board::drawValidMoves(sf::RenderWindow& window, sf::Vector2f position, const std::vector<std::pair<int, int>>& moves) const
{
    for (const auto& move : moves) 
    {
        float x = position.x + move.second * Globals::CELL_SIZE + Globals::CELL_SIZE / 2.f;
        float y = position.y + move.first * Globals::CELL_SIZE + Globals::CELL_SIZE / 2.f;

        sf::CircleShape highlight(Globals::CELL_SIZE * 0.2f);
        highlight.setOrigin(sf::Vector2f(Globals::CELL_SIZE * 0.2f, Globals::CELL_SIZE * 0.2f));
        highlight.setPosition(sf::Vector2f(x, y));
        highlight.setFillColor(sf::Color(0, 255, 0, 100));
        window.draw(highlight);
    }
}

void Board::render(sf::RenderWindow& window, sf::Font& font) const
{
    sf::Vector2f boardPosition(Globals::BOARD_OFFSET_X, Globals::BOARD_OFFSET_Y);

    // Draw grid
    drawGrid(window, boardPosition);

    // Draw pieces
    drawPieces(window, boardPosition);

    // Draw valid moves if a piece is selected
    if (m_pieceSelected) 
    {
        auto moves = getValidMoves(m_selectedRow, m_selectedCol);
        drawValidMoves(window, boardPosition, moves);
    }

    // Draw unplaced pieces panel
    drawUnplacedPieces(window, m_currentPlayer, font);
}

// ==================== INPUT HELPERS ====================

std::pair<int, int> Board::screenToGrid(sf::Vector2f screenPos) const
{
    int col = static_cast<int>((screenPos.x - Globals::BOARD_OFFSET_X) / Globals::CELL_SIZE);
    int row = static_cast<int>((screenPos.y - Globals::BOARD_OFFSET_Y) / Globals::CELL_SIZE);
    return { row, col };
}

bool Board::isPositionOnBoard(sf::Vector2f screenPos) const
{
    auto [row, col] = screenToGrid(screenPos);
    return isValidPosition(row, col);
}

PieceType Board::getClickedUnplacedPiece(sf::Vector2f screenPos, Player player) const
{
    if (!m_placementPhase) return PieceType::None;
    if (player != m_currentPlayer) return PieceType::None;

    const PlayerPieces& pieces = (player == Player::Player1) ? m_player1Pieces : m_player2Pieces;

    float yOffset = Globals::UNPLACED_PANEL_Y;
    float spacing = 100.f;
    float clickRadius = 60.f;

    // Check Donkey
    if (pieces.donkeys > 0) 
    {
        sf::Vector2f donkeyPos(Globals::UNPLACED_PANEL_X + 30.f, yOffset + 30.f);
        float dist = std::sqrt(std::pow(screenPos.x - donkeyPos.x, 2) + std::pow(screenPos.y - donkeyPos.y, 2));
        if (dist < clickRadius) return PieceType::Donkey;
        yOffset += spacing;
    }

    // Check Snake
    if (pieces.snakes > 0)
    {
        sf::Vector2f snakePos(Globals::UNPLACED_PANEL_X + 30.f, yOffset + 30.f);
        float dist = std::sqrt(std::pow(screenPos.x - snakePos.x, 2) + std::pow(screenPos.y - snakePos.y, 2));
        if (dist < clickRadius) return PieceType::Snake;
        yOffset += spacing;
    }

    // Check Frog
    if (pieces.frogs > 0)
    {
        sf::Vector2f frogPos(Globals::UNPLACED_PANEL_X + 30.f, yOffset + 30.f);
        float dist = std::sqrt(std::pow(screenPos.x - frogPos.x, 2) + std::pow(screenPos.y - frogPos.y, 2));
        if (dist < clickRadius) return PieceType::Frog;
    }

    return PieceType::None;
}

const Piece& Board::getPiece(int row, int col) const
{
    static Piece emptyPiece;
    if (!isValidPosition(row, col)) return emptyPiece;
    return m_grid[row][col];
}