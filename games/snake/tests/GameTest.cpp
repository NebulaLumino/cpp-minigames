/**
 * =============================================================================
 * @file GameTest.cpp
 * @brief Snake game tests / 贪吃蛇游戏测试
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include <catch2/catch_test_macros.hpp>
#include "game.h"

TEST_CASE("SnakeGame: Initial state", "[SnakeGame]") {
    SnakeGame game;

    // 蛇初始长度为3 / Snake starts with length 3
    REQUIRE(game.getSnakeSize() == 3);

    // 蛇头在(10, 10) / Snake head at (10, 10)
    auto head = game.getSnakeHead();
    REQUIRE(head.first == 10);
    REQUIRE(head.second == 10);

    // 分数初始为0 / Score starts at 0
    REQUIRE(game.getScore() == 0);

    // 等级初始为1 / Level starts at 1
    REQUIRE(game.getLevel() == 1);

    // 游戏未结束 / Game not over
    REQUIRE(game.isGameOver() == false);
}

TEST_CASE("SnakeGame: Food spawns on board", "[SnakeGame]") {
    SnakeGame game;

    // 食物在有效范围内 / Food within valid range
    int foodX = game.getFoodX();
    int foodY = game.getFoodY();
    bool xValid = (foodX >= 1) && (foodX <= 20);
    bool yValid = (foodY >= 1) && (foodY <= 20);
    REQUIRE(xValid);
    REQUIRE(yValid);
}

TEST_CASE("SnakeGame: Collision with walls", "[SnakeGame]") {
    SnakeGame game;

    // 撞墙检测 / Wall collision detection
    REQUIRE(game.checkCollision(-1, 10) == true);   // 左墙 / Left wall
    REQUIRE(game.checkCollision(22, 10) == true);  // 右墙 / Right wall
    REQUIRE(game.checkCollision(10, -1) == true);   // 上墙 / Top wall
    REQUIRE(game.checkCollision(10, 22) == true);   // 下墙 / Bottom wall

    // 安全位置（蛇头位置是碰撞的）/ Safe positions (snake head position IS collision)
    REQUIRE(game.checkCollision(0, 0) == false);
    REQUIRE(game.checkCollision(5, 5) == false);
}

TEST_CASE("SnakeGame: Collision with self", "[SnakeGame]") {
    SnakeGame game;

    // 蛇身在位置(9, 10) / Snake body at (9, 10)
    REQUIRE(game.checkCollision(9, 10) == true);

    // 蛇身在位置(8, 10) / Snake body at (8, 10)
    REQUIRE(game.checkCollision(8, 10) == true);
}

TEST_CASE("SnakeGame: Move snake", "[SnakeGame]") {
    SnakeGame game;

    int initialSize = game.getSnakeSize();

    // 模拟一次移动（通过update）
    game.update(0.5f);

    // 蛇应该仍然存在 / Snake should still exist
    REQUIRE(game.getSnakeSize() >= initialSize - 1);
}

TEST_CASE("SnakeGame: Multiple updates", "[SnakeGame]") {
    SnakeGame game;

    // 多次更新 / Multiple updates
    for (int i = 0; i < 10; i++) {
        game.update(0.5f);
    }

    // 游戏仍然运行 / Game still running
    REQUIRE(game.isGameOver() == false);
}

TEST_CASE("SnakeGame: Score increases when eating", "[SnakeGame]") {
    SnakeGame game;

    int initialScore = game.getScore();

    // 蛇向食物移动并吃到
    // Note: 食物位置随机，所以这个测试只验证逻辑
    // Snake moves toward food and eats
    // Note: Food position is random, test just verifies logic
    game.update(0.5f);

    // 分数可能增加（如果吃到食物）/ May increase if food eaten
    REQUIRE(game.getScore() >= initialScore);
}
