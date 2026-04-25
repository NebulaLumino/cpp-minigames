/**
 * =============================================================================
 * @file board.h
 * @brief Chess board / 国际象棋棋盘
 * =============================================================================
 *
 * 管理国际象棋游戏的棋盘状态、棋子移动和游戏规则。
 * Manages chess board state, piece movement, and game rules.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include <vector>
#include <string>

/**
 * @brief 棋子类型 / Piece types
 */
enum PieceType {
    EMPTY = 0,
    KING = 1,
    QUEEN = 2,
    ROOK = 3,
    BISHOP = 4,
    KNIGHT = 5,
    PAWN = 6
};

/**
 * @brief 棋子颜色 / Piece color
 */
enum PieceColor {
    NONE = 0,
    WHITE = 1,
    BLACK = 2
};

/**
 * @brief 棋子结构 / Piece structure
 */
struct Piece {
    PieceType type;
    PieceColor color;

    Piece() : type(EMPTY), color(NONE) {}
    Piece(PieceType t, PieceColor c) : type(t), color(c) {}

    bool isEmpty() const { return type == EMPTY; }
    bool isWhite() const { return color == WHITE; }
    bool isBlack() const { return color == BLACK; }

    char toChar() const;
};

/**
 * @brief 移动结构 / Move structure
 */
struct Move {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
    PieceType promotion;  // 兵升变 / Pawn promotion

    Move() : fromRow(0), fromCol(0), toRow(0), toCol(0), promotion(EMPTY) {}
    Move(int fr, int fc, int tr, int tc, PieceType prom = EMPTY)
        : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc), promotion(prom) {}

    bool isValid() const { return fromRow >= 0 && fromRow < 8 && fromCol >= 0 && fromCol < 8 &&
                                    toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8; }
};

/**
 * =============================================================================
 * @class ChessBoard
 * @brief 国际象棋棋盘类 / Chess Board Class
 * =============================================================================
 */
class ChessBoard {
public:
    ChessBoard();

    // 重置棋盘 / Reset board
    void reset();

    // 获取棋子 / Get piece
    Piece getPiece(int row, int col) const;
    PieceType getPieceType(int row, int col) const;

    // 获取可行的移动 / Get legal moves
    std::vector<Move> getLegalMoves(int row, int col);
    std::vector<Move> getAllLegalMoves(PieceColor color);

    // 执行移动 / Make move
    bool makeMove(const Move& move);
    bool makeMove(const std::string& moveStr);  // e.g., "e2e4"

    // 游戏状态 / Game state
    PieceColor getCurrentTurn() const { return currentTurn_; }
    bool isGameOver() const { return gameOver_; }
    PieceColor getWinner() const { return winner_; }
    bool isCheck() const { return inCheck_; }
    bool isCheckmate() const { return checkmate_; }
    bool isStalemate() const { return stalemate_; }

    // 特殊移动检查 / Special move checks
    bool canCastle(PieceColor color, bool kingside) const;
    bool canEnPassant(int row, int col) const;
    bool isValidPromotion(int row, int col, PieceType promoteTo) const;

    // 棋盘输出 / Board output
    void printBoard() const;
    std::string toString() const;

private:
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) const;
    bool wouldCauseCheck(int fromRow, int fromCol, int toRow, int toCol);
    bool isSquareAttacked(int row, int col, PieceColor byColor) const;

    mutable Piece board_[8][8];
    PieceColor currentTurn_;
    bool gameOver_;
    PieceColor winner_;
    bool inCheck_;
    bool checkmate_;
    bool stalemate_;

    // 特殊移动状态 / Special move state
    bool whiteKingsideCastle_;
    bool whiteQueensideCastle_;
    bool blackKingsideCastle_;
    bool blackQueensideCastle_;
    int enPassantRow_;
    int enPassantCol_;
    int halfMoveClock_;
    int fullMoveNumber_;
};