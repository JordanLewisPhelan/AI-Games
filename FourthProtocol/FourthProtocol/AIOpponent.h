#pragma once
#include "Board.h"
#include "Globals.h"
#include <limits>
#include <algorithm>
#include <random>
#include <iostream>


enum class AIDifficulty {
	Easy,	// Some randomness, doesnt look far ahead
	Medium, // Depth 2, looks mildly ahead
	Hard	// Depth 3, will look a few moves ahead to make a better decision
};


struct AIMove {

	bool isInPlacement;

    // For placement
    PieceType pieceType;
    int row;
    int col;

    // For movement
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;


    AIMove() : isInPlacement(false), pieceType(PieceType::None),
        row(-1), col(-1), fromRow(-1), fromCol(-1),
        toRow(-1), toCol(-1) {}

    static AIMove createPlacement(PieceType type, int r, int c) {
        AIMove move;
        move.isInPlacement = true;
        move.pieceType = type;
        move.row = r;
        move.col = c;
        return move;
    }

    static AIMove createMovement(int fr, int fc, int tr, int tc) {
        AIMove move;
        move.isInPlacement = false;
        move.fromRow = fr;
        move.fromCol = fc;
        move.toRow = tr;
        move.toCol = tc;
        return move;
    }
};



class AIOpponent
{
private:
    AIDifficulty m_difficulty;
    Player m_aiPlayer;
    int m_maxDepth;     // how many moves it will  investigate through

    // Evaluation weights
    static constexpr int WIN_SCORE = 100000;
    static constexpr int THREE_IN_ROW_OPEN = 500;   // 3 in row with space
    static constexpr int TWO_IN_ROW_OPEN = 50;      // 2 in row with space
    static constexpr int CENTER_CONTROL = 10;       // Bonus for center pieces
    static constexpr int PIECE_MOBILITY = 5;        // Bonus for mobile pieces


    /// <summary>
    /// Minimax algorithm with Alpha-Beta pruning
    /// </summary>
    /// <param name="t_board">reference to board</param>
    /// <param name="t_depth">how many turns we investigate</param>
    /// <param name="t_alpha & t_beta">the miniMax algorithms alpha and beta</param>
    /// <param name="t_MaximizingPlayer">simulates whose player turn it is will help AI determine best course of action over a series of turns</param>
    int miniMax(Board& t_board, int t_depth, int t_alpha, int t_beta, bool t_maximizingPlayer);


    // Evaluators
    int evaluateBoard(Board& t_board);
    int evaluateLines(const Board& t_board, Player t_player) const;
    int evaluatePiecePlacement(const Board& t_board, Player t_player) const;
    int evaluateMobility(const Board& t_board, Player t_player) const;


    // Move generation systems
    std::vector<AIMove> getAllPossibleMoves(const Board& board, Player player) const;
    std::vector<AIMove> getPlacementMoves(const Board& board, Player player) const;
    std::vector<AIMove> getMovementMoves(const Board& board, Player player) const;

public:
    AIOpponent(AIDifficulty t_diff, Player t_aiPlayer);

    AIMove calculateBestMove(Board& t_board);

    AIDifficulty getDifficulty() const { return m_difficulty; }
    void setDifficulty(AIDifficulty t_diff);

};