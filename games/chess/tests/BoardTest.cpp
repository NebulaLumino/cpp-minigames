/**
 * =============================================================================
 * @file BoardTest.cpp
 * @brief Chess board tests / 国际象棋棋盘测试
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include <catch2/catch_test_macros.hpp>
#include "board.h"

TEST_CASE("ChessBoard: Initial setup", "[ChessBoard]") {
    ChessBoard board;

    // 白方棋子 / White pieces
    REQUIRE(board.getPiece(7, 4).type == KING);
    REQUIRE(board.getPiece(7, 4).color == WHITE);
    REQUIRE(board.getPiece(7, 0).type == ROOK);
    REQUIRE(board.getPiece(7, 0).color == WHITE);

    // 黑方棋子 / Black pieces
    REQUIRE(board.getPiece(0, 4).type == KING);
    REQUIRE(board.getPiece(0, 4).color == BLACK);
    REQUIRE(board.getPiece(0, 0).type == ROOK);
    REQUIRE(board.getPiece(0, 0).color == BLACK);

    // 兵 / Pawns
    REQUIRE(board.getPiece(6, 0).type == PAWN);
    REQUIRE(board.getPiece(6, 0).color == WHITE);
    REQUIRE(board.getPiece(1, 0).type == PAWN);
    REQUIRE(board.getPiece(1, 0).color == BLACK);
}

TEST_CASE("ChessBoard: White first move", "[ChessBoard]") {
    ChessBoard board;

    // 白方先走 / White moves first
    REQUIRE(board.getCurrentTurn() == WHITE);

    // 走e2e4 / Move e2e4
    bool moved = board.makeMove("e2e4");
    REQUIRE(moved == true);
    REQUIRE(board.getCurrentTurn() == BLACK);

    // 检查兵的位置 / Check pawn position
    REQUIRE(board.getPiece(4, 4).type == PAWN);
    REQUIRE(board.getPiece(4, 4).color == WHITE);
}

TEST_CASE("ChessBoard: Invalid move", "[ChessBoard]") {
    ChessBoard board;

    // 黑方不能先走 / Black cannot move first
    bool moved = board.makeMove("e7e5");
    REQUIRE(moved == false);

    // 白方走后黑方可以走 / After white moves, black can
    board.makeMove("e2e4");
    moved = board.makeMove("e7e5");
    REQUIRE(moved == true);
}

TEST_CASE("ChessBoard: Full game sequence", "[ChessBoard]") {
    ChessBoard board;

    // 一些基本移动序列 / Some basic move sequence
    REQUIRE(board.makeMove("e2e4") == true);
    REQUIRE(board.makeMove("e7e5") == true);
    REQUIRE(board.makeMove("g1f3") == true);
    REQUIRE(board.makeMove("b8c6") == true);

    // 回合切换正确 / Turn switching is correct
    REQUIRE(board.getCurrentTurn() == WHITE);
}

TEST_CASE("ChessBoard: Reset", "[ChessBoard]") {
    ChessBoard board;

    // 走几步 / Make some moves
    board.makeMove("e2e4");
    board.makeMove("e7e5");

    // 重置 / Reset
    board.reset();

    // 检查初始状态 / Check initial state
    REQUIRE(board.getCurrentTurn() == WHITE);
    REQUIRE(board.getPiece(7, 4).type == KING);
    REQUIRE(board.getPiece(1, 4).type == PAWN);
}

TEST_CASE("ChessBoard: Move notation parsing", "[ChessBoard]") {
    ChessBoard board;

    // 测试移动字符串解析 / Test move string parsing
    bool moved = board.makeMove("e2e4");
    REQUIRE(moved == true);
    REQUIRE(board.getCurrentTurn() == BLACK);
}

TEST_CASE("ChessBoard: Get all legal moves", "[ChessBoard]") {
    ChessBoard board;

    // 获取白方所有合法移动 / Get all white legal moves
    auto moves = board.getAllLegalMoves(WHITE);
    REQUIRE(moves.size() > 0);

    // 获取黑方所有合法移动 / Get all black legal moves
    moves = board.getAllLegalMoves(BLACK);
    REQUIRE(moves.size() == 0);  // 黑方还没走 / Black hasn't moved yet

    board.makeMove("e2e4");
    moves = board.getAllLegalMoves(BLACK);
    REQUIRE(moves.size() > 0);  // 黑方现在可以走 / Black can now move
}

TEST_CASE("ChessBoard: Game over detection", "[ChessBoard]") {
    ChessBoard board;

    // 初始游戏未结束 / Initial game not over
    REQUIRE(board.isGameOver() == false);
    REQUIRE(board.getWinner() == NONE);
}

TEST_CASE("ChessBoard: Piece colors", "[ChessBoard]") {
    ChessBoard board;

    // 白方棋子 / White pieces
    Piece whiteKing = board.getPiece(7, 4);
    REQUIRE(whiteKing.isWhite() == true);
    REQUIRE(whiteKing.isBlack() == false);

    // 黑方棋子 / Black pieces
    Piece blackKing = board.getPiece(0, 4);
    REQUIRE(blackKing.isBlack() == true);
    REQUIRE(blackKing.isWhite() == false);
}