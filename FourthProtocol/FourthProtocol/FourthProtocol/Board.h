#pragma once
#include "Globals.h"
#include <SFML\Graphics.hpp>
#include <vector>
#include <utility>
#include <cmath>
#include "Pieces.h"

class Board {
private:

    Piece m_grid[Globals::GRID_SIZE][Globals::GRID_SIZE];

    // Track unplaced pieces for each player
    struct PlayerPieces {
        int donkeys = 3;
        int snakes = 1;
        int frogs = 1;

        bool allPlaced() const { return donkeys == 0 && snakes == 0 && frogs == 0; }
        int getPieceCount(PieceType type) const {
            switch (type) {
            case PieceType::Donkey: return donkeys;
            case PieceType::Snake: return snakes;
            case PieceType::Frog: return frogs;
            default: return 0;
            }
        }
    };

    PlayerPieces m_player1Pieces;
    PlayerPieces m_player2Pieces;

    bool m_placementPhase;
    Player m_currentPlayer;

    // Currently selected piece for placement
    PieceType m_selectedPieceType;

    // For move phase - selected piece position
    int m_selectedRow;
    int m_selectedCol;
    bool m_pieceSelected;

    // Helper methods
    bool isValidPosition(int row, int col) const;
    sf::Color getPlayerColor(Player player) const;
    sf::Color getPieceColor(PieceType type, Player player) const;
    void drawGrid(sf::RenderWindow& window, sf::Vector2f position) const;
    void drawPieces(sf::RenderWindow& window, sf::Vector2f position) const;
    void drawUnplacedPieces(sf::RenderWindow& window, Player player, sf::Font& font) const;
    void drawValidMoves(sf::RenderWindow& window, sf::Vector2f position, const std::vector<std::pair<int, int>>& moves) const;

    // Movement validation helpers
    std::vector<std::pair<int, int>> getDonkeyMoves(int row, int col) const;
    std::vector<std::pair<int, int>> getSnakeMoves(int row, int col) const;
    std::vector<std::pair<int, int>> getFrogMoves(int row, int col) const;

    // Win checking helpers
    bool checkDirection(int startRow, int startCol, int deltaRow, int deltaCol, Player player) const;

public:
    Board();

    // Placement phase
    bool canPlacePiece(int row, int col, PieceType type, Player player) const;
    bool placePiece(int row, int col, PieceType type, Player player);
    void selectPieceType(PieceType type);
    PieceType getSelectedPieceType() const { return m_selectedPieceType; }
    bool isPlacementComplete() const;

    // Movement phase
    std::vector<std::pair<int, int>> getValidMoves(int row, int col) const;
    bool selectPieceForMove(int row, int col);
    bool movePiece(int toRow, int toCol);
    void deselectPiece();
    bool isPieceSelected() const { return m_pieceSelected; }
    std::pair<int, int> getSelectedPiecePosition() const { return { m_selectedRow, m_selectedCol }; }

    // Win checking
    bool checkWin(Player player) const;

    // Turn management
    Player getCurrentPlayer() const { return m_currentPlayer; }
    void switchPlayer();

    // Rendering
    void render(sf::RenderWindow& window, sf::Font& font) const;

    // Getters
    const Piece& getPiece(int row, int col) const;
    bool isPlacementPhase() const { return m_placementPhase; }

    // Convert screen position to grid coordinates
    std::pair<int, int> screenToGrid(sf::Vector2f screenPos) const;
    bool isPositionOnBoard(sf::Vector2f screenPos) const;

    // Check if clicking on unplaced piece panel
    PieceType getClickedUnplacedPiece(sf::Vector2f screenPos, Player player) const;
};