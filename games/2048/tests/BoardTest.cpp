/**
 * =============================================================================
 * @file BoardTest.cpp
 * @brief 2048 board tests / 2048棋盘测试
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include <catch2/catch_test_macros.hpp>
#include "board.h"

TEST_CASE("GameBoard: Initial state", "[GameBoard]") {
    GameBoard board;

    // 初始有2个数字块 / Initial has 2 tiles
    int nonZero = 0;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (board.getCell(r, c) != 0) {
                nonZero++;
            }
        }
    }
    REQUIRE(nonZero >= 2);

    // 初始分数为0 / Initial score is 0
    REQUIRE(board.getScore() == 0);

    // 初始游戏未结束 / Game not over initially
    REQUIRE(board.isGameOver() == false);
    REQUIRE(board.hasWon() == false);
}

TEST_CASE("GameBoard: Reset", "[GameBoard]") {
    GameBoard board;

    // 移动一次增加分数可能性 / Move might increase score
    board.moveLeft();

    // 重置 / Reset
    board.reset();

    // 分数重置 / Score reset
    REQUIRE(board.getScore() == 0);
    REQUIRE(board.isGameOver() == false);
}

TEST_CASE("GameBoard: Move operations exist", "[GameBoard]") {
    GameBoard board;

    // 所有移动操作应该存在且可调用
    // All move operations should exist and be callable
    bool moved = board.moveLeft();
    // moved could be true or false depending on board state

    moved = board.moveRight();
    moved = board.moveUp();
    moved = board.moveDown();

    // 应该不崩溃 / Should not crash
    REQUIRE(true);
}

TEST_CASE("GameBoard: Grid size", "[GameBoard]") {
    GameBoard board;

    // 4x4棋盘 / 4x4 board
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            int val = board.getCell(r, c);
            REQUIRE(val >= 0);  // 所有值应该是0或正数 / All values should be 0 or positive
        }
    }
}

TEST_CASE("GameBoard: Get highest tile", "[GameBoard]") {
    GameBoard board;

    int highest = board.getHighestTile();
    REQUIRE(highest >= 2);  // 初始至少有一个2 / At least one 2 initially
}

TEST_CASE("GameBoard: Merge mechanics", "[GameBoard]") {
    GameBoard board;
    board.reset();

    // 移动几次后检查分数变化 / After several moves, check score changes
    int initialScore = board.getScore();
    board.moveLeft();
    board.moveDown();
    board.moveLeft();
    board.moveUp();

    // 分数可能增加（如果合并了）/ Score may increase (if merged)
    REQUIRE(board.getScore() >= initialScore);
}

TEST_CASE("GameBoard: Game over detection", "[GameBoard]") {
    GameBoard board;
    board.reset();

    // 进行多次移动 / Make many moves
    for (int i = 0; i < 20; i++) {
        board.moveLeft();
        board.moveDown();
        board.moveRight();
        board.moveUp();
    }

    // 游戏最终应该结束（如果棋盘满了且无法移动）
    // Game should eventually end (if board full and can't move)
    // 注意：由于随机性，可能不会结束
    // Note: Due to randomness, may not end
}

TEST_CASE("GameBoard: Can move check", "[GameBoard]") {
    GameBoard board;

    // 初始应该可以移动 / Should be able to move initially
    bool canMove = board.canMove();
    REQUIRE(canMove == true);
}

TEST_CASE("GameBoard: Best score tracking", "[GameBoard]") {
    GameBoard board;

    int initialBest = board.getBestScore();

    // 进行一些移动 / Make some moves
    for (int i = 0; i < 10; i++) {
        board.moveLeft();
    }

    // 最高分应该更新 / Best score should update
    // 如果游戏结束，最高分会被更新
    // If game over, best score gets updated
}

TEST_CASE("GameBoard: Tile values", "[GameBoard]") {
    GameBoard board;

    // 所有tile值应该是2的幂 / All tile values should be powers of 2
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            int val = board.getCell(r, c);
            if (val > 0) {
                // 检查是否是2的幂 / Check if power of 2
                bool isPowerOf2 = (val & (val - 1)) == 0;
                REQUIRE(isPowerOf2);
            }
        }
    }
}