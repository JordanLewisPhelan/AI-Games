#include "AIOpponent.h"


// Constructor & setup
AIOpponent::AIOpponent(AIDifficulty t_diff, Player t_aiPlayer)
    : m_difficulty(t_diff), m_aiPlayer(t_aiPlayer)
{
    setDifficulty(m_difficulty);
}



void AIOpponent::setDifficulty(AIDifficulty t_diff)
{
    m_difficulty = t_diff;

    switch (m_difficulty)
    {
    case AIDifficulty::Easy: 
        m_maxDepth = 1;
        break;
    case AIDifficulty::Medium:
        m_maxDepth = 2;
        break;
    case AIDifficulty::Hard:
        m_maxDepth = 3;
        break;
    }
}


// Minimax algorithm
int AIOpponent::miniMax(Board& t_board, int t_depth, int t_alpha, int t_beta, bool t_maximizingPlayer)
{
    // Return conditions
    if (t_depth == 0 || t_board.checkWin(Player::Player1) || t_board.checkWin(Player::Player2))
    {
        return evaluateBoard(t_board);
    }

    Player l_currentPlayer = t_board.getCurrentPlayer();
    std::vector<AIMove> l_moves = getAllPossibleMoves(t_board, l_currentPlayer);

    if (l_moves.empty())
    {
        return evaluateBoard(t_board);
    }

    if (t_maximizingPlayer)
    {
        int maxEval = std::numeric_limits<int>::min();

        for (const AIMove& move : l_moves)
        {
            // Safeguarding current board state with a backup
            Board::MoveBackup backup;

            if (move.isInPlacement)
            {
                backup = t_board.simulatePlacement(move.pieceType, move.row, move.col, l_currentPlayer);
            }
            else
            {
                backup = t_board.simulateMove(move.fromRow, move.fromCol, move.toRow, move.toCol);
            }

            // Begin Recursive Minimax loop
            int eval = miniMax(t_board, t_depth - 1, t_alpha, t_beta, false);
            maxEval = std::max(maxEval, eval);

            // Undo move after performing it
            t_board.undoMove(backup);

            // Alpha-Beta pruning
            t_alpha = std::max(t_alpha, t_beta);
            if (t_beta <= t_alpha)
                break; // Prunes any remaining moves that stem from this stem
        }
        return maxEval;
    }
    else
    {
        int minEval = std::numeric_limits<int>::max();

        for (const AIMove& move : l_moves)
        {
            Board::MoveBackup backup;
            if (move.isInPlacement) {
                backup = t_board.simulatePlacement(move.pieceType, move.row, move.col, l_currentPlayer);
            }
            else {
                backup = t_board.simulateMove(move.fromRow, move.fromCol, move.toRow, move.toCol);
            }

            int eval = miniMax(t_board, t_depth - 1, t_alpha, t_beta, true);
            minEval = std::min(minEval, eval);

            t_board.undoMove(backup);

            t_beta = std::min(t_beta, eval);
            if (t_beta <= t_alpha) {
                break;
            }
        }

        return minEval;
    }
}


// ==================== MOVE GENERATION ====================

std::vector<AIMove> AIOpponent::getAllPossibleMoves(const Board& t_board, Player t_player) const
{
    if (t_board.isPlacementPhase())
    {
        return getPlacementMoves(t_board, t_player);
    }
    else 
    {
        return getMovementMoves(t_board, t_player);
    }
}

std::vector<AIMove> AIOpponent::getPlacementMoves(const Board& t_board, Player t_player) const
{
    std::vector<AIMove> l_moves;

    // Check what pieces this player has available
    std::vector<PieceType> l_availableTypes;
    if (t_board.getRemainingPieceCount(t_player, PieceType::Donkey) > 0)
    {
        l_availableTypes.push_back(PieceType::Donkey);
    }
    if (t_board.getRemainingPieceCount(t_player, PieceType::Snake) > 0) 
    {
        l_availableTypes.push_back(PieceType::Snake);
    }
    if (t_board.getRemainingPieceCount(t_player, PieceType::Frog) > 0) 
    {
        l_availableTypes.push_back(PieceType::Frog);
    }

    // Try placing each available piece type at each empty position
    for (int row = 0; row < Globals::GRID_SIZE; ++row)
    {
        for (int col = 0; col < Globals::GRID_SIZE; ++col)
        {
            for (PieceType type : l_availableTypes) 
            {
                if (t_board.canPlacePiece(row, col, type, t_player)) 
                {
                    l_moves.push_back(AIMove::createPlacement(type, row, col));
                }
            }
        }
    }

    return l_moves;
}

std::vector<AIMove> AIOpponent::getMovementMoves(const Board& t_board, Player t_player) const
{
    std::vector<AIMove> l_moves;

    // Find all pieces owned by this player
    for (int row = 0; row < Globals::GRID_SIZE; ++row) 
    {
        for (int col = 0; col < Globals::GRID_SIZE; ++col)
        {
            const Piece& l_piece = t_board.getPiece(row, col);

            if (!l_piece.isEmpty() && l_piece.owner == t_player)
            {
                // Get valid moves for this piece
                auto validMoves = t_board.getValidMoves(row, col);

                for (const auto& [toRow, toCol] : validMoves) 
                {
                    l_moves.push_back(AIMove::createMovement(row, col, toRow, toCol));
                }
            }
        }
    }

    return l_moves;
}


// ==================== EVALUATION ====================

int AIOpponent::evaluateBoard(Board& t_board)
{
    if (t_board.checkWin(m_aiPlayer))
    {
        return WIN_SCORE;
    }

    Player l_opponent = (m_aiPlayer == Player::Player1) ? Player::Player2 : Player::Player1;
    if (t_board.checkWin(l_opponent))
    {
        return -WIN_SCORE;
    }

    // Check score
    int l_score = 0;

    // Potential winning lines
    l_score += evaluateLines(t_board, m_aiPlayer);
    l_score -= evaluateLines(t_board, l_opponent);

    // Piece positioning
    l_score += evaluatePiecePlacement(t_board, m_aiPlayer);
    l_score -= evaluatePiecePlacement(t_board, l_opponent);

    // Only evaluate mobility in movement phase
    if (!t_board.isPlacementPhase()) {
        l_score += evaluateMobility(t_board, m_aiPlayer);
        l_score -= evaluateMobility(t_board, l_opponent);
    }

    return l_score;
}

int AIOpponent::evaluateLines(const Board& t_board, Player t_player) const
{
    int l_score = 0;
    auto lines = t_board.analyzeLines(t_player);

    for (const auto& line : lines) 
    {
        if (line.consecutiveCount == 3 && line.hasSpace)
        {
            l_score += THREE_IN_ROW_OPEN;
        }
        else if (line.consecutiveCount == 2 && line.hasSpace) 
        {
            l_score += TWO_IN_ROW_OPEN;
        }
    }

    return l_score;
}

int AIOpponent::evaluatePiecePlacement(const Board& t_board, Player t_player) const
{
    int l_score = 0;

    // Bonus for controlling center (rows 1-3, cols 1-3)
    for (int row = 1; row <= 3; ++row) 
    {
        for (int col = 1; col <= 3; ++col) 
        {
            const Piece& l_piece = t_board.getPiece(row, col);

            if (!l_piece.isEmpty() && l_piece.owner == t_player) 
            {
                l_score += CENTER_CONTROL;

                // Extra bonus for very center (2,2)
                if (row == 2 && col == 2) {
                    l_score += CENTER_CONTROL;
                }
            }
        }
    }

    return l_score;
}

int AIOpponent::evaluateMobility(const Board& t_board, Player t_player) const
{
    int l_score = 0;

    // Counts total number of available moves - Always better to have more options than less
    for (int row = 0; row < Globals::GRID_SIZE; ++row)
    {
        for (int col = 0; col < Globals::GRID_SIZE; ++col)
        {
            const Piece& l_piece = t_board.getPiece(row, col);

            if (!l_piece.isEmpty() && l_piece.owner == t_player)
            {
                int l_moveCount = t_board.getValidMoves(row, col).size();
                l_score += l_moveCount * PIECE_MOBILITY;
            }
        }
    }

	return l_score;
}


// ===================== AI DECISION MAKING =================

AIMove AIOpponent::calculateBestMove(Board& t_board)
{
    std::cout << "AI thinking (depth " << m_maxDepth << ")...\n";

    std::vector<AIMove> possibleMoves = getAllPossibleMoves(t_board, m_aiPlayer);

    if (possibleMoves.empty()) {
        std::cout << "ERROR: AI has no valid moves!\n";
        return AIMove();
    }

    std::cout << "Evaluating " << possibleMoves.size() << " possible moves...\n";

    // Easy mode: Add randomness
    if (m_difficulty == AIDifficulty::Easy) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 99);

        if (dis(gen) < 40) {  // 40% chance of random move
            std::uniform_int_distribution<> moveDis(0, possibleMoves.size() - 1);
            std::cout << "AI making random move (Easy mode)\n";
            return possibleMoves[moveDis(gen)];
        }
    }

    AIMove bestMove;
    int bestScore = std::numeric_limits<int>::min();
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    for (size_t i = 0; i < possibleMoves.size(); ++i) {
        const AIMove& move = possibleMoves[i];

        // Apply move
        Board::MoveBackup backup;
        if (move.isInPlacement) {
            backup = t_board.simulatePlacement(move.pieceType, move.row, move.col, m_aiPlayer);
        }
        else {
            backup = t_board.simulateMove(move.fromRow, move.fromCol, move.toRow, move.toCol);
        }

        // Evaluate
        int score = miniMax(t_board, m_maxDepth - 1, alpha, beta, false);

        // Undo
        t_board.undoMove(backup);

        std::cout << "  Move " << (i + 1) << "/" << possibleMoves.size() << " - Score: " << score << "\n";

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        alpha = std::max(alpha, score);
    }

    std::cout << "AI chose move with score: " << bestScore << "\n";
    return bestMove;
}

