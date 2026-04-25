/**
 * =============================================================================
 * @file game.cpp
 * @brief Snake game implementation / 贪吃蛇游戏实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "game.h"
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

SnakeGame::SnakeGame()
    : score_(0)
    , level_(1)
    , moveTimer_(0.0f)
    , moveInterval_(0.3f)  // 每0.3秒移动一次 / Move every 0.3 seconds
    , isGameOver_(false)
    , isPaused_(false)
    , direction_(RIGHT)
    , nextDirection_(RIGHT) {

    // 初始化蛇在中间位置 / Initialize snake in middle
    snake_.push_back({10, 10});  // 头 / Head
    snake_.push_back({9, 10});   // 身体 / Body
    snake_.push_back({8, 10});   // 尾巴 / Tail

    spawnFood();
}

void SnakeGame::run() {
    using namespace std::chrono;
    using namespace std::this_thread;

    while (renderer_.isOpen()) {
        renderer_.processEvents();

        if (renderer_.shouldClose()) break;

        if (renderer_.getPause()) {
            isPaused_ = !isPaused_;
            renderer_.resetInputs();
            sleep_for(milliseconds(16));
            continue;
        }

        if (isGameOver_) {
            if (renderer_.getHardDrop()) {
                // 重置游戏 / Reset game
                score_ = 0;
                level_ = 1;
                moveInterval_ = 0.3f;
                snake_.clear();
                snake_.push_back({10, 10});
                snake_.push_back({9, 10});
                snake_.push_back({8, 10});
                direction_ = RIGHT;
                nextDirection_ = RIGHT;
                spawnFood();
                isGameOver_ = false;
            }
            renderer_.resetInputs();
            sleep_for(milliseconds(16));
            continue;
        }

        handleInput();
        update(0.016f);
        renderer_.draw(snake_, foodX_, foodY_, score_, level_);

        sleep_for(milliseconds(16));
    }
}

void SnakeGame::handleInput() {
    if (renderer_.getMoveLeft() && direction_ != RIGHT) {
        nextDirection_ = LEFT;
    } else if (renderer_.getMoveRight() && direction_ != LEFT) {
        nextDirection_ = RIGHT;
    } else if (renderer_.getMoveDown() && direction_ != UP) {
        nextDirection_ = DOWN;
    } else if (renderer_.getMoveUp() && direction_ != DOWN) {
        nextDirection_ = UP;
    }
}

void SnakeGame::update(float dt) {
    moveTimer_ += dt;

    if (moveTimer_ >= moveInterval_) {
        moveTimer_ -= moveInterval_;
        direction_ = nextDirection_;

        if (!moveSnake()) {
            isGameOver_ = true;
        }
    }
}

bool SnakeGame::moveSnake() {
    // 计算新的头部位置 / Calculate new head position
    int newX = snake_[0].first;
    int newY = snake_[0].second;

    switch (direction_) {
        case UP:    newY--; break;
        case DOWN:  newY++; break;
        case LEFT:  newX--; break;
        case RIGHT: newX++; break;
    }

    // 检查碰撞 / Check collision
    if (checkCollision(newX, newY)) {
        return false;
    }

    // 在前面添加新头部 / Add new head at front
    snake_.insert(snake_.begin(), {newX, newY});

    // 检查是否吃到食物 / Check if eating food
    if (newX == foodX_ && newY == foodY_) {
        score_ += 10;
        spawnFood();
        // 每50分升一级 / Level up every 50 points
        level_ = score_ / 50 + 1;
        moveInterval_ = std::max(0.1f, 0.3f - (level_ - 1) * 0.02f);
    } else {
        // 没吃到食物，移除尾巴 / No food, remove tail
        snake_.pop_back();
    }

    return true;
}

void SnakeGame::spawnFood() {
    // 简单实现：在随机位置生成食物，不与蛇体重叠
    // Simple: spawn food at random position, not overlapping snake
    srand(static_cast<unsigned>(time(nullptr)));

    do {
        foodX_ = rand() % 20 + 1;
        foodY_ = rand() % 20 + 1;
    } while (checkCollision(foodX_, foodY_));
}

bool SnakeGame::checkCollision(int x, int y) {
    // 边界碰撞 / Boundary collision
    if (x < 0 || x >= 22 || y < 0 || y >= 22) {
        return true;
    }

    // 蛇身碰撞 / Self collision
    for (const auto& seg : snake_) {
        if (seg.first == x && seg.second == y) {
            return true;
        }
    }

    return false;
}
