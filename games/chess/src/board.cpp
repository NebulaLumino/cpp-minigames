/**
 * =============================================================================
 * @file board.cpp
 * @brief Chess board implementation / 国际象棋棋盘实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "board.h"
#include <iostream>
#include <cctype>
#include <sstream>

char Piece::toChar() const {
    if (type == EMPTY) return '.';

    char c = '?';
    switch (type) {
        case KING:   c = 'k'; break;
        case QUEEN:  c = 'q'; break;
        case ROOK:   c = 'r'; break;
        case BISHOP: c = 'b'; break;
        case KNIGHT: c = 'n'; break;
        case PAWN:   c = 'p'; break;
        default:     c = '?'; break;
    }

    return isWhite() ? toupper(c) : c;
}

ChessBoard::ChessBoard()
    : currentTurn_(WHITE)
    , gameOver_(false)
    , winner_(NONE)
    , inCheck_(false)
    , checkmate_(false)
    , stalemate_(false)
    , whiteKingsideCastle_(true)
    , whiteQueensideCastle_(true)
    , blackKingsideCastle_(true)
    , blackQueensideCastle_(true)
    , enPassantRow_(-1)
    , enPassantCol_(-1)
    , halfMoveClock_(0)
    , fullMoveNumber_(1) {

    reset();
}

void ChessBoard::reset() {
    // 清空棋盘 / Clear board
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            board_[r][c] = Piece();
        }
    }

    // 白方棋子 / White pieces
    board_[7][0] = Piece(ROOK, WHITE);
    board_[7][1] = Piece(KNIGHT, WHITE);
    board_[7][2] = Piece(BISHOP, WHITE);
    board_[7][3] = Piece(QUEEN, WHITE);
    board_[7][4] = Piece(KING, WHITE);
    board_[7][5] = Piece(BISHOP, WHITE);
    board_[7][6] = Piece(KNIGHT, WHITE);
    board_[7][7] = Piece(ROOK, WHITE);
    for (int c = 0; c < 8; c++) {
        board_[6][c] = Piece(PAWN, WHITE);
    }

    // 黑方棋子 / Black pieces
    board_[0][0] = Piece(ROOK, BLACK);
    board_[0][1] = Piece(KNIGHT, BLACK);
    board_[0][2] = Piece(BISHOP, BLACK);
    board_[0][3] = Piece(QUEEN, BLACK);
    board_[0][4] = Piece(KING, BLACK);
    board_[0][5] = Piece(BISHOP, BLACK);
    board_[0][6] = Piece(KNIGHT, BLACK);
    board_[0][7] = Piece(ROOK, BLACK);
    for (int c = 0; c < 8; c++) {
        board_[1][c] = Piece(PAWN, BLACK);
    }

    currentTurn_ = WHITE;
    gameOver_ = false;
    winner_ = NONE;
    inCheck_ = false;
    checkmate_ = false;
    stalemate_ = false;
    whiteKingsideCastle_ = true;
    whiteQueensideCastle_ = true;
    blackKingsideCastle_ = true;
    blackQueensideCastle_ = true;
    enPassantRow_ = -1;
    enPassantCol_ = -1;
}

Piece ChessBoard::getPiece(int row, int col) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return Piece();
    }
    return board_[row][col];
}

PieceType ChessBoard::getPieceType(int row, int col) const {
    return getPiece(row, col).type;
}

bool ChessBoard::isPathClear(int fromRow, int fromCol, int toRow, int toCol) const {
    int dr = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
    int dc = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;

    int r = fromRow + dr;
    int c = fromCol + dc;

    while (r != toRow || c != toCol) {
        if (!board_[r][c].isEmpty()) {
            return false;
        }
        r += dr;
        c += dc;
    }

    return true;
}

bool ChessBoard::isSquareAttacked(int row, int col, PieceColor byColor) const {
    // 检查兵的攻击 / Check pawn attacks
    int pawnDir = (byColor == WHITE) ? 1 : -1;
    if (row + pawnDir >= 0 && row + pawnDir < 8) {
        if (col - 1 >= 0 && board_[row + pawnDir][col - 1].type == PAWN &&
            board_[row + pawnDir][col - 1].color == byColor) {
            return true;
        }
        if (col + 1 < 8 && board_[row + pawnDir][col + 1].type == PAWN &&
            board_[row + pawnDir][col + 1].color == byColor) {
            return true;
        }
    }

    // 检查马的攻击 / Check knight attacks
    int knightMoves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for (auto& m : knightMoves) {
        int nr = row + m[0];
        int nc = col + m[1];
        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            if (board_[nr][nc].type == KNIGHT && board_[nr][nc].color == byColor) {
                return true;
            }
        }
    }

    // 检查对角线的攻击（主教、皇后）/ Check diagonal attacks (bishop, queen)
    int diagMoves[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    for (auto& m : diagMoves) {
        int nr = row + m[0];
        int nc = col + m[1];
        while (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            if (!board_[nr][nc].isEmpty()) {
                if (board_[nr][nc].color == byColor) {
                    PieceType t = board_[nr][nc].type;
                    if (t == BISHOP || t == QUEEN) {
                        return true;
                    }
                }
                break;
            }
            nr += m[0];
            nc += m[1];
        }
    }

    // 检查直线的攻击（车、皇后）/ Check straight attacks (rook, queen)
    int straightMoves[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (auto& m : straightMoves) {
        int nr = row + m[0];
        int nc = col + m[1];
        while (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            if (!board_[nr][nc].isEmpty()) {
                if (board_[nr][nc].color == byColor) {
                    PieceType t = board_[nr][nc].type;
                    if (t == ROOK || t == QUEEN) {
                        return true;
                    }
                }
                break;
            }
            nr += m[0];
            nc += m[1];
        }
    }

    // 检查国王的攻击 / Check king attacks
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = row + dr;
            int nc = col + dc;
            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                if (board_[nr][nc].type == KING && board_[nr][nc].color == byColor) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool ChessBoard::wouldCauseCheck(int fromRow, int fromCol, int toRow, int toCol) {
    // 找到国王位置 / Find king position
    PieceColor myColor = board_[fromRow][fromCol].color;
    int kingRow = -1, kingCol = -1;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board_[r][c].type == KING && board_[r][c].color == myColor) {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
    }

    if (kingRow == -1) return true;  // 没有国王，模拟移动

    // 临时移动棋子 / Temporarily move piece
    Piece piece = board_[fromRow][fromCol];
    Piece captured = board_[toRow][toCol];
    board_[fromRow][fromCol] = Piece();
    board_[toRow][toCol] = piece;

    // 如果移动的是国王，更新国王位置 / If moved king, update position
    if (piece.type == KING) {
        kingRow = toRow;
        kingCol = toCol;
    }

    // 检查是否被将军 / Check if in check
    bool inCheck = isSquareAttacked(kingRow, kingCol, myColor == WHITE ? BLACK : WHITE);

    // 恢复棋盘 / Restore board
    board_[fromRow][fromCol] = piece;
    board_[toRow][toCol] = captured;

    return inCheck;
}

bool ChessBoard::isValidMove(int fromRow, int fromCol, int toRow, int toCol) const {
    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
        return false;
    }

    Piece piece = board_[fromRow][fromCol];
    if (piece.isEmpty() || piece.color != currentTurn_) {
        return false;
    }

    Piece dest = board_[toRow][toCol];
    if (!dest.isEmpty() && dest.color == piece.color) {
        return false;
    }

    int dr = toRow - fromRow;
    int dc = toCol - fromCol;

    switch (piece.type) {
        case PAWN: {
            int dir = piece.isWhite() ? -1 : 1;
            // 前进 / Forward
            if (dc == 0 && dr == dir && dest.isEmpty()) {
                return true;
            }
            // 第一次可以走两格 / First move can go two squares
            if (dc == 0 && dr == 2 * dir && dest.isEmpty()) {
                int midRow = fromRow + dir;
                if (board_[midRow][fromCol].isEmpty()) {
                    return true;
                }
            }
            // 吃子 / Capture
            if (dc == 1 && dr == dir) {
                bool isCapture = (!dest.isEmpty() && dest.color != piece.color);
                bool isEnPassant = (toRow == enPassantRow_ && toCol == enPassantCol_);
                if (isCapture || isEnPassant) {
                    return true;
                }
            }
            if (dc == -1 && dr == dir) {
                bool isCapture = (!dest.isEmpty() && dest.color != piece.color);
                bool isEnPassant = (toRow == enPassantRow_ && toCol == enPassantCol_);
                if (isCapture || isEnPassant) {
                    return true;
                }
            }
            return false;
        }

        case KNIGHT: {
            if ((abs(dr) == 2 && abs(dc) == 1) || (abs(dr) == 1 && abs(dc) == 2)) {
                return true;
            }
            return false;
        }

        case BISHOP: {
            if (abs(dr) == abs(dc) && isPathClear(fromRow, fromCol, toRow, toCol)) {
                return true;
            }
            return false;
        }

        case ROOK: {
            if ((dr == 0 || dc == 0) && isPathClear(fromRow, fromCol, toRow, toCol)) {
                return true;
            }
            return false;
        }

        case QUEEN: {
            if ((dr == 0 || dc == 0 || abs(dr) == abs(dc)) &&
                isPathClear(fromRow, fromCol, toRow, toCol)) {
                return true;
            }
            return false;
        }

        case KING: {
            if (abs(dr) <= 1 && abs(dc) <= 1) {
                return true;
            }
            // 王车易位 / Castling
            if (dr == 0 && abs(dc) == 2) {
                if (piece.isWhite() && fromRow == 7 && fromCol == 4) {
                    if (dc == 2 && whiteKingsideCastle_ && !inCheck_) {
                        if (board_[7][5].isEmpty() && board_[7][6].isEmpty() &&
                            !isSquareAttacked(5, 5, BLACK) && !isSquareAttacked(6, 6, BLACK)) {
                            return true;
                        }
                    }
                    if (dc == -2 && whiteQueensideCastle_ && !inCheck_) {
                        if (board_[7][3].isEmpty() && board_[7][2].isEmpty() &&
                            board_[7][1].isEmpty() &&
                            !isSquareAttacked(2, 2, BLACK) && !isSquareAttacked(3, 3, BLACK)) {
                            return true;
                        }
                    }
                }
                if (piece.isBlack() && fromRow == 0 && fromCol == 4) {
                    if (dc == 2 && blackKingsideCastle_ && !inCheck_) {
                        if (board_[0][5].isEmpty() && board_[0][6].isEmpty() &&
                            !isSquareAttacked(5, 5, WHITE) && !isSquareAttacked(6, 6, WHITE)) {
                            return true;
                        }
                    }
                    if (dc == -2 && blackQueensideCastle_ && !inCheck_) {
                        if (board_[0][3].isEmpty() && board_[0][2].isEmpty() &&
                            board_[0][1].isEmpty() &&
                            !isSquareAttacked(2, 2, WHITE) && !isSquareAttacked(3, 3, WHITE)) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        default:
            return false;
    }
}

std::vector<Move> ChessBoard::getLegalMoves(int row, int col) {
    std::vector<Move> moves;

    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return moves;
    }

    Piece piece = board_[row][col];
    if (piece.isEmpty() || piece.color != currentTurn_) {
        return moves;
    }

    // 检查所有可能的移动 / Check all possible moves
    for (int toRow = 0; toRow < 8; toRow++) {
        for (int toCol = 0; toCol < 8; toCol++) {
            if (isValidMove(row, col, toRow, toCol)) {
                if (!wouldCauseCheck(row, col, toRow, toCol)) {
                    Move move(row, col, toRow, toCol);

                    // 检查兵的升变 / Check pawn promotion
                    if (piece.type == PAWN) {
                        if ((piece.isWhite() && toRow == 0) || (piece.isBlack() && toRow == 7)) {
                            // 添加所有可能的升变 / Add all possible promotions
                            for (PieceType prom : {QUEEN, ROOK, BISHOP, KNIGHT}) {
                                move.promotion = prom;
                                moves.push_back(move);
                            }
                        } else {
                            moves.push_back(move);
                        }
                    } else {
                        moves.push_back(move);
                    }
                }
            }
        }
    }

    return moves;
}

std::vector<Move> ChessBoard::getAllLegalMoves(PieceColor color) {
    std::vector<Move> allMoves;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board_[r][c].color == color) {
                std::vector<Move> pieceMoves = getLegalMoves(r, c);
                allMoves.insert(allMoves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }

    return allMoves;
}

bool ChessBoard::makeMove(const Move& move) {
    if (!move.isValid()) {
        return false;
    }

    Piece piece = board_[move.fromRow][move.fromCol];
    if (piece.isEmpty() || piece.color != currentTurn_) {
        return false;
    }

    if (!isValidMove(move.fromRow, move.fromCol, move.toRow, move.toCol)) {
        return false;
    }

    if (wouldCauseCheck(move.fromRow, move.fromCol, move.toRow, move.toCol)) {
        return false;
    }

    // 执行移动 / Make the move
    Piece captured = board_[move.toRow][move.toCol];
    board_[move.fromRow][move.fromCol] = Piece();

    // 处理吃过路兵 / Handle en passant
    if (piece.type == PAWN && move.toCol == enPassantCol_ && move.toRow == enPassantRow_) {
        int captureRow = piece.isWhite() ? enPassantRow_ + 1 : enPassantRow_ - 1;
        board_[captureRow][move.toCol] = Piece();
    }

    board_[move.toRow][move.toCol] = piece;

    // 处理兵的升变 / Handle promotion
    if (piece.type == PAWN && move.promotion != EMPTY) {
        board_[move.toRow][move.toCol] = Piece(move.promotion, piece.color);
    }

    // 处理王车易位 / Handle castling
    if (piece.type == KING && abs(move.toCol - move.fromCol) == 2) {
        if (move.toCol > move.fromCol) {
            // 短易位 / Kingside
            board_[move.fromRow][5] = board_[move.fromRow][7];
            board_[move.fromRow][7] = Piece();
        } else {
            // 长易位 / Queenside
            board_[move.fromRow][3] = board_[move.fromRow][0];
            board_[move.fromRow][0] = Piece();
        }
    }

    // 更新易权状态 / Update castling rights
    if (piece.type == KING) {
        if (piece.isWhite()) {
            whiteKingsideCastle_ = false;
            whiteQueensideCastle_ = false;
        } else {
            blackKingsideCastle_ = false;
            blackQueensideCastle_ = false;
        }
    }
    if (piece.type == ROOK) {
        if (move.fromRow == 7 && move.fromCol == 0) whiteQueensideCastle_ = false;
        if (move.fromRow == 7 && move.fromCol == 7) whiteKingsideCastle_ = false;
        if (move.fromRow == 0 && move.fromCol == 0) blackQueensideCastle_ = false;
        if (move.fromRow == 0 && move.fromCol == 7) blackKingsideCastle_ = false;
    }

    // 更新吃过路兵坐标 / Update en passant square
    if (piece.type == PAWN && abs(move.toRow - move.fromRow) == 2) {
        enPassantRow_ = (move.fromRow + move.toRow) / 2;
        enPassantCol_ = move.fromCol;
    } else {
        enPassantRow_ = -1;
        enPassantCol_ = -1;
    }

    // 切换回合 / Switch turn
    currentTurn_ = (currentTurn_ == WHITE) ? BLACK : WHITE;

    // 检查游戏状态 / Check game state
    PieceColor enemy = (currentTurn_ == WHITE) ? BLACK : WHITE;
    inCheck_ = false;

    // 找到国王位置 / Find king position
    int kingRow = -1, kingCol = -1;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board_[r][c].type == KING && board_[r][c].color == currentTurn_) {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
    }

    if (kingRow != -1 && isSquareAttacked(kingRow, kingCol, enemy)) {
        inCheck_ = true;
    }

    // 检查是否将死或逼和 / Check for checkmate or stalemate
    std::vector<Move> legalMoves = getAllLegalMoves(currentTurn_);
    if (legalMoves.empty()) {
        gameOver_ = true;
        if (inCheck_) {
            checkmate_ = true;
            winner_ = enemy;
        } else {
            stalemate_ = true;
            winner_ = NONE;
        }
    }

    return true;
}

bool ChessBoard::makeMove(const std::string& moveStr) {
    if (moveStr.length() < 4) {
        return false;
    }

    int fromCol = moveStr[0] - 'a';
    int fromRow = 8 - (moveStr[1] - '0');
    int toCol = moveStr[2] - 'a';
    int toRow = 8 - (moveStr[3] - '0');

    PieceType promotion = EMPTY;
    if (moveStr.length() >= 5) {
        char p = moveStr[4];
        switch (tolower(p)) {
            case 'q': promotion = QUEEN; break;
            case 'r': promotion = ROOK; break;
            case 'b': promotion = BISHOP; break;
            case 'n': promotion = KNIGHT; break;
        }
    }

    Move move(fromRow, fromCol, toRow, toCol, promotion);
    return makeMove(move);
}

bool ChessBoard::canCastle(PieceColor color, bool kingside) const {
    if (color == WHITE) {
        if (kingside) {
            return whiteKingsideCastle_ &&
                   board_[7][4].type == KING &&
                   board_[7][5].isEmpty() &&
                   board_[7][6].isEmpty() &&
                   !isSquareAttacked(7, 5, BLACK) &&
                   !isSquareAttacked(7, 6, BLACK);
        } else {
            return whiteQueensideCastle_ &&
                   board_[7][4].type == KING &&
                   board_[7][3].isEmpty() &&
                   board_[7][2].isEmpty() &&
                   board_[7][1].isEmpty() &&
                   !isSquareAttacked(7, 3, BLACK) &&
                   !isSquareAttacked(7, 2, BLACK);
        }
    } else {
        if (kingside) {
            return blackKingsideCastle_ &&
                   board_[0][4].type == KING &&
                   board_[0][5].isEmpty() &&
                   board_[0][6].isEmpty() &&
                   !isSquareAttacked(0, 5, WHITE) &&
                   !isSquareAttacked(0, 6, WHITE);
        } else {
            return blackQueensideCastle_ &&
                   board_[0][4].type == KING &&
                   board_[0][3].isEmpty() &&
                   board_[0][2].isEmpty() &&
                   board_[0][1].isEmpty() &&
                   !isSquareAttacked(0, 3, WHITE) &&
                   !isSquareAttacked(0, 2, WHITE);
        }
    }
}

bool ChessBoard::canEnPassant(int row, int col) const {
    return (row == enPassantRow_ && col == enPassantCol_);
}

bool ChessBoard::isValidPromotion(int row, int col, PieceType promoteTo) const {
    Piece piece = board_[row][col];
    if (piece.type != PAWN) return false;

    bool isPromotionRow = (piece.isWhite() && row == 0) || (piece.isBlack() && row == 7);
    if (!isPromotionRow) return false;

    return (promoteTo == QUEEN || promoteTo == ROOK || promoteTo == BISHOP || promoteTo == KNIGHT);
}

void ChessBoard::printBoard() const {
    std::cout << "\n  +-----------------+\n";
    for (int r = 0; r < 8; r++) {
        std::cout << 8 - r << " | ";
        for (int c = 0; c < 8; c++) {
            std::cout << board_[r][c].toChar() << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "  +-----------------+\n";
    std::cout << "    a b c d e f g h\n";
    std::cout << "\nCurrent turn: " << (currentTurn_ == WHITE ? "White" : "Black") << "\n";
}

std::string ChessBoard::toString() const {
    std::ostringstream oss;
    oss << "\n  +-----------------+\n";
    for (int r = 0; r < 8; r++) {
        oss << 8 - r << " | ";
        for (int c = 0; c < 8; c++) {
            oss << board_[r][c].toChar() << " ";
        }
        oss << "|\n";
    }
    oss << "  +-----------------+\n";
    oss << "    a b c d e f g h\n";
    return oss.str();
}