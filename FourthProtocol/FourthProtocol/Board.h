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

    // Tracks a direction line of pieces e.g. Is this diagonal close to a winning sequence?
    struct LineInfo {
        int consecutiveCount;     // How many pieces are in a row
        bool hasSpace;              // Is there anymore points in this line
        bool isBlocked;             // Does the opponent block our line?

        LineInfo() : consecutiveCount(0), hasSpace(false), isBlocked(false) {}
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

    // Sprite textures
    sf::Texture m_donkeyTexture;
    sf::Texture m_snakeTexture;
    sf::Texture m_frogTexture;
    bool m_texturesLoaded;

    // Helper to load textures
    void loadTextures();

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


    /* -- Additions for AI and clarity -- */

    // struct to allow AI to maximize depth searching 
    struct MoveBackup {

        // For movement
        Piece movedPiece;
        Piece destinationPiece;
        int fromRow, fromCol;
        int toRow, toCol;

        // For placement
        PieceType placedType;
        int placedRow, placedCol;

        // State tracking
        Player previousPlayer;
        bool wasPlacementPhase;
        PlayerPieces player1State;
        PlayerPieces player2State;

        bool isPlacement;

        MoveBackup() : fromRow(-1), fromCol(-1), toRow(-1), toCol(-1),
            placedType(PieceType::None), placedRow(-1), placedCol(-1),
            previousPlayer(Player::None), wasPlacementPhase(false),
            isPlacement(false) {}
    };


    // Methods to simulate moves and placements
    MoveBackup simulatePlacement(PieceType t_type, int t_row, int t_col, Player t_player);
    MoveBackup simulateMove(int t_fromRow, int t_fromCol, int t_toRow, int t_toCol);
    void undoMove(const MoveBackup& t_backup);


    // Helper function for AI to analyze board for line sequences
    std::vector<LineInfo> analyzeLines(Player t_player) const;


    // How many other player pieces are left to place based on queried input type (for placement segment)
    int getRemainingPieceCount(Player t_player, PieceType t_type) const;

};