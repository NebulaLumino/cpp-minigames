/**
 * =============================================================================
 * @file game.h
 * @brief Snake game main logic / 贪吃蛇游戏主逻辑
 * =============================================================================
 *
 * 管理贪吃蛇游戏的所有状态和逻辑。
 * Manages all state and logic for the Snake game.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include <vector>
#include "renderer.h"

/**
 * =============================================================================
 * @class SnakeGame
 * @brief 贪吃蛇游戏主类 / Snake Game Main Class
 * =============================================================================
 */
class SnakeGame {
public:
    SnakeGame();
    void run();

    // 测试用接口 / Test interface
    int getScore() const { return score_; }
    int getLevel() const { return level_; }
    int getSnakeSize() const { return static_cast<int>(snake_.size()); }
    std::pair<int, int> getSnakeHead() const { return snake_.front(); }
    std::pair<int, int> getSnakeSegment(int index) const { return snake_.at(index); }
    int getFoodX() const { return foodX_; }
    int getFoodY() const { return foodY_; }
    bool isGameOver() const { return isGameOver_; }
    bool checkCollision(int x, int y);
    void update(float dt);

private:
    void handleInput();
    bool moveSnake();
    void spawnFood();

    Renderer renderer_;
    int score_;
    int level_;
    float moveTimer_;
    float moveInterval_;
    bool isGameOver_;
    bool isPaused_;

    // 蛇身，每段是一个{x, y}坐标
    // Snake body, each segment is a pair of {x, y}
    std::vector<std::pair<int, int>> snake_;

    // 食物位置 / Food position
    int foodX_;
    int foodY_;

    // 移动方向 / Movement direction
    enum Direction { UP, DOWN, LEFT, RIGHT };
    Direction direction_;
    Direction nextDirection_;
};
