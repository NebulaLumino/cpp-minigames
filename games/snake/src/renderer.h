/**
 * =============================================================================
 * @file renderer.h
 * @brief Snake game renderer / 贪吃蛇游戏渲染器
 * =============================================================================
 *
 * 使用ncurses库进行终端渲染。
 * Uses ncurses library for terminal rendering.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include <vector>
#include <utility>
#include <curses.h>

/**
 * =============================================================================
 * @class Renderer
 * @brief 贪吃蛇游戏渲染器类 / Snake Game Renderer Class
 * =============================================================================
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(const std::vector<std::pair<int,int>>& snake, int foodX, int foodY, int score, int level);
    void processEvents();
    void resetInputs();

    bool isOpen() const { return isOpen_; }
    bool shouldClose() const { return shouldClose_; }
    bool getMoveLeft() const { return moveLeft_; }
    bool getMoveRight() const { return moveRight_; }
    bool getMoveUp() const { return moveUp_; }
    bool getMoveDown() const { return moveDown_; }
    bool getPause() const { return pause_; }
    bool getHardDrop() const { return hardDrop_; }

    void drawGameOver(int x, int y);

    int getMaxX() const { return maxX_; }
    int getMaxY() const { return maxY_; }

private:
    bool isOpen_;
    bool shouldClose_;
    bool moveLeft_, moveRight_, moveUp_, moveDown_;
    bool pause_, hardDrop_;
    int inputChar_;
    int maxX_, maxY_;
};
