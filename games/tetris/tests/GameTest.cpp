/**
 * =============================================================================
 * @file GameTest.cpp
 * @brief 游戏逻辑类单元测试 / Game Logic Class Unit Tests
 * =============================================================================
 *
 * 测试游戏逻辑类的计分系统：
 * - 根据消除行数计算得分
 * - 根据总得分计算等级
 *
 * Tests Game class scoring system:
 * - Score calculation based on lines cleared
 * - Level calculation based on total score
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#include <catch2/catch_test_macros.hpp>
#include "game.h"

/**
 * =============================================================================
 * 计分系统测试 / Scoring System Tests
 * =============================================================================
 */

TEST_CASE("Game: calculateScore - 0行消除", "Game: calculateScore - 0 lines cleared") {
    REQUIRE(Game::calculateScore(0) == 0);
}

TEST_CASE("Game: calculateScore - 1行消除（Single）", "Game: calculateScore - 1 line cleared (Single)") {
    REQUIRE(Game::calculateScore(1) == 100);
}

TEST_CASE("Game: calculateScore - 2行消除（Double）", "Game: calculateScore - 2 lines cleared (Double)") {
    REQUIRE(Game::calculateScore(2) == 300);
}

TEST_CASE("Game: calculateScore - 3行消除（Triple）", "Game: calculateScore - 3 lines cleared (Triple)") {
    REQUIRE(Game::calculateScore(3) == 500);
}

TEST_CASE("Game: calculateScore - 4行消除（Tetris）", "Game: calculateScore - 4 lines cleared (Tetris)") {
    REQUIRE(Game::calculateScore(4) == 800);
}

TEST_CASE("Game: calculateScore - 超过4行被截断", "Game: calculateScore - Beyond 4 lines truncated") {
    REQUIRE(Game::calculateScore(5) == 800);
    REQUIRE(Game::calculateScore(10) == 800);
}

/**
 * =============================================================================
 * 等级系统测试 / Level System Tests
 * =============================================================================
 */

TEST_CASE("Game: calculateLevel - 0分时为1级", "Game: calculateLevel - Level 1 at 0 score") {
    REQUIRE(Game::calculateLevel(0) == 1);
}

TEST_CASE("Game: calculateLevel - 1000分时升为2级", "Game: calculateLevel - Level 2 at 1000 score") {
    REQUIRE(Game::calculateLevel(1000) == 2);
}

TEST_CASE("Game: calculateLevel - 2000分时升为3级", "Game: calculateLevel - Level 3 at 2000 score") {
    REQUIRE(Game::calculateLevel(2000) == 3);
}

TEST_CASE("Game: calculateLevel - 中间分数向上取整", "Game: calculateLevel - Rounds up for mid scores") {
    REQUIRE(Game::calculateLevel(1) == 1);
    REQUIRE(Game::calculateLevel(500) == 1);
    REQUIRE(Game::calculateLevel(1001) == 2);
}

TEST_CASE("Game: calculateLevel - 最高15级", "Game: calculateLevel - Maximum level 15") {
    REQUIRE(Game::calculateLevel(15000) == 15);
    REQUIRE(Game::calculateLevel(20000) == 15);
    REQUIRE(Game::calculateLevel(100000) == 15);
}
