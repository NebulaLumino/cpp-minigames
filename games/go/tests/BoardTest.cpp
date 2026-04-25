/**
 * =============================================================================
 * @file BoardTest.cpp
 * @brief Go board tests / 围棋棋盘测试
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include <catch2/catch_test_macros.hpp>
#include "board.h"

TEST_CASE("GoBoard: Initial setup", "[GoBoard]") {
    GoBoard board(9);

    REQUIRE(board.getBoardSize() == 9);
    REQUIRE(board.getCurrentTurn() == BLACK);
    REQUIRE(board.isGameOver() == false);
    REQUIRE(board.getCapturedBlack() == 0);
    REQUIRE(board.getCapturedWhite() == 0);
}

TEST_CASE("GoBoard: Empty board stones", "[GoBoard]") {
    GoBoard board(9);

    // 初始棋盘应该全是空的 / Initial board should be empty
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            REQUIRE(board.getStone(r, c) == EMPTY);
        }
    }
}

TEST_CASE("GoBoard: Place stone", "[GoBoard]") {
    GoBoard board(9);

    // 放置黑棋 / Place black stone
    bool placed = board.placeStone(4, 4, BLACK);
    REQUIRE(placed == true);
    REQUIRE(board.getStone(4, 4) == BLACK);
    REQUIRE(board.getCurrentTurn() == WHITE);
}

TEST_CASE("GoBoard: Turn switching", "[GoBoard]") {
    GoBoard board(9);

    REQUIRE(board.getCurrentTurn() == BLACK);

    // 黑棋落子 / Black plays
    board.placeStone(4, 4, BLACK);
    REQUIRE(board.getCurrentTurn() == WHITE);

    // 白棋落子 / White plays
    board.placeStone(4, 5, WHITE);
    REQUIRE(board.getCurrentTurn() == BLACK);
}

TEST_CASE("GoBoard: Invalid move - occupied", "[GoBoard]") {
    GoBoard board(9);

    // 放置黑棋 / Place black
    board.placeStone(4, 4, BLACK);

    // 尝试在同位置放白棋 / Try to place white at same position
    bool placed = board.placeStone(4, 4, WHITE);
    REQUIRE(placed == false);
}

TEST_CASE("GoBoard: Invalid move - wrong turn", "[GoBoard]") {
    GoBoard board(9);

    // 白棋尝试先走 / White tries to move first
    bool placed = board.placeStone(4, 4, WHITE);
    REQUIRE(placed == false);
}

TEST_CASE("GoBoard: Stone counting", "[GoBoard]") {
    GoBoard board(9);

    board.placeStone(4, 4, BLACK);
    board.placeStone(4, 5, WHITE);
    board.placeStone(3, 4, BLACK);

    REQUIRE(board.countStones(BLACK) == 2);
    REQUIRE(board.countStones(WHITE) == 1);
}

TEST_CASE("GoBoard: Reset", "[GoBoard]") {
    GoBoard board(9);

    board.placeStone(4, 4, BLACK);
    board.placeStone(4, 5, WHITE);

    board.reset();

    REQUIRE(board.getCurrentTurn() == BLACK);
    REQUIRE(board.getStone(4, 4) == EMPTY);
    REQUIRE(board.getStone(4, 5) == EMPTY);
}

TEST_CASE("GoBoard: Pass", "[GoBoard]") {
    GoBoard board(9);

    board.pass();
    REQUIRE(board.getCurrentTurn() == WHITE);

    board.pass();
    // 连续pass应该结束游戏 / Consecutive passes should end game
    REQUIRE(board.isGameOver() == true);
}

TEST_CASE("GoBoard: Get valid moves", "[GoBoard]") {
    GoBoard board(9);

    auto moves = board.getValidMoves(BLACK);
    REQUIRE(moves.size() == 81);  // 9x9 empty board

    board.placeStone(4, 4, BLACK);
    moves = board.getValidMoves(WHITE);
    REQUIRE(moves.size() == 80);  // One position occupied
}

TEST_CASE("GoBoard: Liberties", "[GoBoard]") {
    GoBoard board(9);

    // 中心棋子应该有4口气 / Center stone should have 4 liberties
    board.placeStone(4, 4, BLACK);
    int libs = board.getLiberties(4, 4);
    REQUIRE(libs == 4);
}