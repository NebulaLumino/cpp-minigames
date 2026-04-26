/**
 * =============================================================================
 * @file chess_engine.h
 * @brief Chess AI Engine / 国际象棋AI引擎
 * =============================================================================
 *
 * A reusable chess AI library with minimax search and alpha-beta pruning.
 * 可重用的国际象棋AI库，包含极小极大搜索和Alpha-Beta剪枝。
 *
 * Features:
 * - Legal move generation / 合法走法生成
 * - Minimax with alpha-beta pruning / 极小极大搜索与Alpha-Beta剪枝
 * - Material and positional evaluation / 物质与位置评估
 * - Configurable search depth / 可配置搜索深度
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

/**
 * @brief 棋子类型 / Piece types
 * White pieces: 1-6, Black pieces: 8-13 (offset by 7)
 */
enum ChessPiece {
    EMPTY = 0,
    KING = 1, QUEEN = 2, ROOK = 3, BISHOP = 4, KNIGHT = 5, PAWN = 6,
    // Black pieces offset by 7
    BKING = 8, BQUEEN = 9, BROOK = 10, BBISHOP = 11, BKNIGHT = 12, BPAWN = 13
};

/**
 * @brief 棋子颜色 / Piece color
 */
enum ChessColor {
    NONE = 0, WHITE = 1, BLACK = 2
};

/**
 * @brief 走法结构 / Move structure
 */
struct ChessMove {
    int fromRow, fromCol;
    int toRow, toCol;
    ChessPiece promotion;
    int score;

    ChessMove() : fromRow(0), fromCol(0), toRow(0), toCol(0), promotion(EMPTY), score(0) {}
    ChessMove(int fr, int fc, int tr, int tc, ChessPiece prom = EMPTY)
        : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc), promotion(prom), score(0) {}
};

/**
 * @brief 棋盘状态 / Board state
 */
struct ChessBoardState {
    ChessPiece board[8][8];
    ChessColor sideToMove;
    bool whiteKingside, whiteQueenside;
    bool blackKingside, blackQueenside;
    int enPassantRow, enPassantCol;

    ChessBoardState();
    void reset();
};

inline ChessBoardState::ChessBoardState() { reset(); }

inline void ChessBoardState::reset() {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            board[r][c] = EMPTY;
    sideToMove = WHITE;
    whiteKingside = whiteQueenside = true;
    blackKingside = blackQueenside = true;
    enPassantRow = enPassantCol = -1;
}

/**
 * =============================================================================
 * @class ChessEngine
 * @brief 国际象棋AI引擎 / Chess AI Engine
 * =============================================================================
 */
class ChessEngine {
public:
    ChessEngine();

    void setSearchDepth(int depth) { searchDepth_ = std::max(1, std::min(depth, 8)); }
    void reset();

    std::vector<ChessMove> generateAllMoves(ChessColor color);
    bool isLegalMove(const ChessMove& move);
    bool makeMove(const ChessMove& move);
    void undoMove();

    ChessMove getBestMove(int maxDepth = 4);
    int evaluate() const;

    bool isInCheck(ChessColor color) const;
    bool isGameOver() const { return false; }
    ChessColor getSideToMove() const { return board_.sideToMove; }

private:
    int minimax(int depth, int alpha, int beta, bool maximizing);
    int materialScore() const;
    int positionalScore() const;
    bool isSquareAttacked(int row, int col, ChessColor byColor) const;

    ChessBoardState board_;
    int searchDepth_;
    int nodesSearched_;
    ChessMove moveStack_[256];
    int moveCount_;
    int history_[8][8][8][8];
};

// Helper functions
inline ChessColor getPieceColor(ChessPiece p) {
    if (p == EMPTY) return NONE;
    if (p < 7) return WHITE;
    return BLACK;
}

inline int pieceValue(ChessPiece p) {
    switch (p % 7) {
        case 1: return 10000; // King - shouldn't happen in midgame
        case 2: return 900;   // Queen
        case 3: return 500;    // Rook
        case 4: return 330;   // Bishop
        case 5: return 320;    // Knight
        case 6: return 100;    // Pawn
        default: return 0;
    }
}

inline int positionalValue(ChessPiece p, int row, int col, ChessColor color) {
    int r = (color == WHITE) ? row : 7 - row;
    int c = col;

    // Simplified pawn table
    if (p == PAWN || p == BPAWN) {
        return (8 - r) * 10;
    }
    if (p == KNIGHT || p == BKNIGHT) {
        return (4 - std::abs(r - 3.5)) * 5 + (4 - std::abs(c - 3.5)) * 5;
    }
    return 0;
}

inline ChessEngine::ChessEngine() : searchDepth_(4), nodesSearched_(0), moveCount_(0) {
    memset(history_, 0, sizeof(history_));
    reset();
}

inline void ChessEngine::reset() {
    board_.reset();

    // Initial position
    board_.board[7][0] = ROOK;   board_.board[7][1] = KNIGHT; board_.board[7][2] = BISHOP;
    board_.board[7][3] = QUEEN;  board_.board[7][4] = KING;
    board_.board[7][5] = BISHOP; board_.board[7][6] = KNIGHT; board_.board[7][7] = ROOK;
    for (int c = 0; c < 8; c++) board_.board[6][c] = PAWN;

    board_.board[0][0] = BROOK;  board_.board[0][1] = BKNIGHT; board_.board[0][2] = BBISHOP;
    board_.board[0][3] = BQUEEN; board_.board[0][4] = BKING;
    board_.board[0][5] = BBISHOP; board_.board[0][6] = BKNIGHT; board_.board[0][7] = BROOK;
    for (int c = 0; c < 8; c++) board_.board[1][c] = BPAWN;
}

inline std::vector<ChessMove> ChessEngine::generateAllMoves(ChessColor color) {
    std::vector<ChessMove> moves;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            ChessPiece p = board_.board[r][c];
            if (p == EMPTY || getPieceColor(p) != color) continue;

            ChessPiece baseType = static_cast<ChessPiece>(p % 7);

            // Pawn moves
            if (baseType == PAWN) {
                int dir = (color == WHITE) ? -1 : 1;
                int startRow = (color == WHITE) ? 6 : 1;

                int nr = r + dir;
                if (nr >= 0 && nr < 8 && board_.board[nr][c] == EMPTY) {
                    if (nr == 0 || nr == 7) {
                        moves.push_back(ChessMove(r, c, nr, c, QUEEN));
                        moves.push_back(ChessMove(r, c, nr, c, ROOK));
                        moves.push_back(ChessMove(r, c, nr, c, BISHOP));
                        moves.push_back(ChessMove(r, c, nr, c, KNIGHT));
                    } else {
                        moves.push_back(ChessMove(r, c, nr, c));
                        if (r == startRow && board_.board[r + 2 * dir][c] == EMPTY) {
                            moves.push_back(ChessMove(r, c, r + 2 * dir, c));
                        }
                    }
                }

                // Captures
                for (int dc = -1; dc <= 1; dc += 2) {
                    int nc = c + dc;
                    if (nc >= 0 && nc < 8 && nr >= 0 && nr < 8) {
                        ChessPiece target = board_.board[nr][nc];
                        if (target != EMPTY && getPieceColor(target) != color) {
                            if (nr == 0 || nr == 7) {
                                moves.push_back(ChessMove(r, c, nr, nc, QUEEN));
                            } else {
                                moves.push_back(ChessMove(r, c, nr, nc));
                            }
                        }
                    }
                }
            }

            // Knight moves
            if (baseType == KNIGHT) {
                int moves_[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
                for (auto& m : moves_) {
                    int nr = r + m[0], nc = c + m[1];
                    if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                        ChessPiece target = board_.board[nr][nc];
                        if (target == EMPTY || getPieceColor(target) != color) {
                            moves.push_back(ChessMove(r, c, nr, nc));
                        }
                    }
                }
            }

            // Bishop moves
            if (baseType == BISHOP) {
                int dirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    while (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                        if (board_.board[nr][nc] == EMPTY) {
                            moves.push_back(ChessMove(r, c, nr, nc));
                        } else {
                            if (getPieceColor(board_.board[nr][nc]) != color) {
                                moves.push_back(ChessMove(r, c, nr, nc));
                            }
                            break;
                        }
                        nr += d[0]; nc += d[1];
                    }
                }
            }

            // Rook moves
            if (baseType == ROOK) {
                int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    while (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                        if (board_.board[nr][nc] == EMPTY) {
                            moves.push_back(ChessMove(r, c, nr, nc));
                        } else {
                            if (getPieceColor(board_.board[nr][nc]) != color) {
                                moves.push_back(ChessMove(r, c, nr, nc));
                            }
                            break;
                        }
                        nr += d[0]; nc += d[1];
                    }
                }
            }

            // Queen moves
            if (baseType == QUEEN) {
                int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    while (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                        if (board_.board[nr][nc] == EMPTY) {
                            moves.push_back(ChessMove(r, c, nr, nc));
                        } else {
                            if (getPieceColor(board_.board[nr][nc]) != color) {
                                moves.push_back(ChessMove(r, c, nr, nc));
                            }
                            break;
                        }
                        nr += d[0]; nc += d[1];
                    }
                }
            }

            // King moves
            if (baseType == KING) {
                for (int dr = -1; dr <= 1; dr++) {
                    for (int dc = -1; dc <= 1; dc++) {
                        if (dr == 0 && dc == 0) continue;
                        int nr = r + dr, nc = c + dc;
                        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                            ChessPiece target = board_.board[nr][nc];
                            if (target == EMPTY || getPieceColor(target) != color) {
                                moves.push_back(ChessMove(r, c, nr, nc));
                            }
                        }
                    }
                }
            }
        }
    }

    return moves;
}

inline bool ChessEngine::isLegalMove(const ChessMove& move) {
    ChessPiece piece = board_.board[move.fromRow][move.fromCol];
    if (piece == EMPTY || getPieceColor(piece) != board_.sideToMove) return false;

    // Generate all moves and check if this one is in it
    auto moves = generateAllMoves(board_.sideToMove);
    for (const auto& m : moves) {
        if (m.fromRow == move.fromRow && m.fromCol == move.fromCol &&
            m.toRow == move.toRow && m.toCol == move.toCol) {
            return true;
        }
    }
    return false;
}

inline bool ChessEngine::makeMove(const ChessMove& move) {
    if (!isLegalMove(move)) return false;

    moveStack_[moveCount_++] = move;
    ChessPiece piece = board_.board[move.fromRow][move.fromCol];
    ChessPiece captured = board_.board[move.toRow][move.toCol];

    board_.board[move.fromRow][move.fromCol] = EMPTY;

    ChessPiece newPiece = piece;
    if (move.promotion != EMPTY) {
        newPiece = (getPieceColor(piece) == WHITE) ? move.promotion :
                   static_cast<ChessPiece>(move.promotion + 7);
    }

    board_.board[move.toRow][move.toCol] = newPiece;
    board_.sideToMove = (board_.sideToMove == WHITE) ? BLACK : WHITE;

    return true;
}

inline void ChessEngine::undoMove() {
    if (moveCount_ == 0) return;
    moveCount_--;
}

inline bool ChessEngine::isSquareAttacked(int row, int col, ChessColor byColor) const {
    // Check pawns
    int pawnDir = (byColor == WHITE) ? 1 : -1;
    ChessPiece pawn = (byColor == WHITE) ? PAWN : BPAWN;
    if (row + pawnDir >= 0 && row + pawnDir < 8) {
        if (col > 0 && board_.board[row + pawnDir][col - 1] == pawn) return true;
        if (col < 7 && board_.board[row + pawnDir][col + 1] == pawn) return true;
    }

    // Check knights
    int knightMoves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    ChessPiece knight = (byColor == WHITE) ? KNIGHT : BKNIGHT;
    for (auto& m : knightMoves) {
        int nr = row + m[0], nc = col + m[1];
        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8 && board_.board[nr][nc] == knight) return true;
    }

    // Check diagonals (bishop, queen)
    int diag[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    ChessPiece bishop = (byColor == WHITE) ? BISHOP : BBISHOP;
    ChessPiece queen = (byColor == WHITE) ? QUEEN : BQUEEN;
    for (auto& d : diag) {
        int nr = row + d[0], nc = col + d[1];
        while (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            if (board_.board[nr][nc] != EMPTY) {
                if (board_.board[nr][nc] == bishop || board_.board[nr][nc] == queen) return true;
                break;
            }
            nr += d[0]; nc += d[1];
        }
    }

    // Check straights (rook, queen)
    int straight[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    ChessPiece rook = (byColor == WHITE) ? ROOK : BROOK;
    for (auto& d : straight) {
        int nr = row + d[0], nc = col + d[1];
        while (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            if (board_.board[nr][nc] != EMPTY) {
                if (board_.board[nr][nc] == rook || board_.board[nr][nc] == queen) return true;
                break;
            }
            nr += d[0]; nc += d[1];
        }
    }

    // Check king
    ChessPiece king = (byColor == WHITE) ? KING : BKING;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = row + dr, nc = col + dc;
            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8 && board_.board[nr][nc] == king) return true;
        }
    }

    return false;
}

inline bool ChessEngine::isInCheck(ChessColor color) const {
    // Find king
    ChessPiece king = (color == WHITE) ? KING : BKING;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board_.board[r][c] == king) {
                ChessColor enemy = (color == WHITE) ? BLACK : WHITE;
                return isSquareAttacked(r, c, enemy);
            }
        }
    }
    return false;
}

inline int ChessEngine::materialScore() const {
    int score = 0;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            ChessPiece p = board_.board[r][c];
            if (p == EMPTY) continue;
            ChessColor color = getPieceColor(p);
            int val = pieceValue(p);
            score += (color == WHITE) ? val : -val;
        }
    }
    return score;
}

inline int ChessEngine::positionalScore() const {
    int score = 0;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            ChessPiece p = board_.board[r][c];
            if (p == EMPTY) continue;
            ChessColor color = getPieceColor(p);
            int val = positionalValue(p, r, c, color);
            score += (color == WHITE) ? val : -val;
        }
    }
    return score;
}

inline int ChessEngine::evaluate() const {
    return materialScore() + positionalScore();
}

inline ChessMove ChessEngine::getBestMove(int maxDepth) {
    searchDepth_ = maxDepth;
    nodesSearched_ = 0;

    auto moves = generateAllMoves(board_.sideToMove);
    if (moves.empty()) return ChessMove();

    // Sort by history heuristic
    for (auto& m : moves) {
        m.score = history_[m.fromRow][m.fromCol][m.toRow][m.toCol];
    }
    std::sort(moves.begin(), moves.end(), [](const ChessMove& a, const ChessMove& b) {
        return a.score > b.score;
    });

    int bestScore = std::numeric_limits<int>::min();
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();
    ChessMove bestMove = moves[0];

    for (const auto& m : moves) {
        makeMove(m);
        int score = -minimax(searchDepth_ - 1, -beta, -alpha, false);
        undoMove();

        if (score > bestScore) {
            bestScore = score;
            bestMove = m;
            history_[m.fromRow][m.fromCol][m.toRow][m.toCol] += searchDepth_ * searchDepth_;
        }
        alpha = std::max(alpha, score);
    }

    return bestMove;
}

inline int ChessEngine::minimax(int depth, int alpha, int beta, bool maximizing) {
    nodesSearched_++;

    if (depth == 0) return evaluate();

    auto moves = generateAllMoves(maximizing ? board_.sideToMove :
                                 (board_.sideToMove == WHITE ? BLACK : WHITE));

    if (moves.empty()) {
        if (isInCheck(board_.sideToMove)) {
            return std::numeric_limits<int>::min() + depth;
        }
        return 0;
    }

    // Sort by history
    for (auto& m : moves) {
        m.score = history_[m.fromRow][m.fromCol][m.toRow][m.toCol];
    }
    std::sort(moves.begin(), moves.end(), [](const ChessMove& a, const ChessMove& b) {
        return a.score > b.score;
    });

    if (maximizing) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& m : moves) {
            makeMove(m);
            int eval = minimax(depth - 1, alpha, beta, false);
            undoMove();
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& m : moves) {
            makeMove(m);
            int eval = minimax(depth - 1, alpha, beta, true);
            undoMove();
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}