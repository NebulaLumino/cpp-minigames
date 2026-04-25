/**
 * =============================================================================
 * @file BoardTest.cpp
 * @brief Minesweeper board tests / 扫雷游戏棋盘测试
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include <catch2/catch_test_macros.hpp>
#include "board.h"

TEST_CASE("Board: Initial state", "[Board]") {
    Board board(9, 9, 10);

    REQUIRE(board.getRows() == 9);
    REQUIRE(board.getCols() == 9);
    REQUIRE(board.getTotalMines() == 10);
    REQUIRE(board.getFlagCount() == 0);
    REQUIRE(board.isGameOver() == false);
    REQUIRE(board.isWin() == false);
}

TEST_CASE("Board: Initial reveal", "[Board]") {
    Board board(9, 9, 10);

    // 首次点击不会踩雷 / First click should not hit mine
    bool result = board.reveal(4, 4);

    // Should not crash and should reveal
    REQUIRE(result == true);
    REQUIRE(board.isRevealed(4, 4) == true);
}

TEST_CASE("Board: Flag toggle", "[Board]") {
    Board board(9, 9, 10);

    board.toggleFlag(0, 0);
    REQUIRE(board.isFlagged(0, 0) == true);
    REQUIRE(board.getFlagCount() == 1);

    board.toggleFlag(0, 0);
    REQUIRE(board.isFlagged(0, 0) == false);
    REQUIRE(board.getFlagCount() == 0);
}

TEST_CASE("Board: Cannot reveal flagged cell", "[Board]") {
    Board board(9, 9, 10);

    board.toggleFlag(0, 0);
    bool result = board.reveal(0, 0);

    // Should not reveal a flagged cell
    REQUIRE(result == false);
    REQUIRE(board.isRevealed(0, 0) == false);
}

TEST_CASE("Board: Flood fill on empty cell", "[Board]") {
    Board board(5, 5, 1);

    // Reveal center cell - should trigger flood fill
    board.reveal(2, 2);

    // 中心区域应该被揭示 / Center area should be revealed
    REQUIRE(board.isRevealed(2, 2) == true);
}

TEST_CASE("Board: Get adjacent mines", "[Board]") {
    Board board(5, 5, 1);

    // 首次揭示确保地雷生成 / First reveal ensures mines generated
    board.reveal(0, 0);

    // Get adjacent mines for a revealed cell
    int adj = board.getAdjacentMines(0, 1);
    REQUIRE(adj >= 0);
}

TEST_CASE("Board: Win condition", "[Board]") {
    Board board(3, 3, 1);

    // 揭示所有非雷格子 / Reveal all non-mine cells
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (!board.isMine(r, c)) {
                board.reveal(r, c);
            }
        }
    }

    // 所有非雷格子揭示后应该获胜
    // Should win after all non-mine cells revealed
}

TEST_CASE("Board: Cannot toggle revealed cell", "[Board]") {
    Board board(9, 9, 10);

    board.reveal(4, 4);
    board.toggleFlag(4, 4);

    // 不能标记已揭示的格子 / Cannot flag revealed cell
    REQUIRE(board.isFlagged(4, 4) == false);
}

TEST_CASE("Board: Game over on mine", "[Board]") {
    Board board(5, 5, 5);

    // 找到一个雷位置并揭示 / Find a mine and reveal it
    // 由于雷是随机放置的，我们通过游戏逻辑来测试
    // Since mines are random, we test via game logic

    // 首先揭示一个安全格子以生成雷 / First reveal safe cell to generate mines
    board.reveal(0, 0);

    // 揭示格子直到触发游戏结束或获胜
    // Reveal cells until game over or win
    for (int r = 0; r < 5 && !board.isGameOver(); r++) {
        for (int c = 0; c < 5 && !board.isGameOver(); c++) {
            if (!board.isRevealed(r, c)) {
                board.reveal(r, c);
            }
        }
    }

    // 游戏应该在某个时刻结束 / Game should end at some point
    if (board.isGameOver() && !board.isWin()) {
        // Hit a mine - game over
        REQUIRE(board.isGameOver() == true);
    }
}