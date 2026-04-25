/**
 * =============================================================================
 * @file renderer.h
 * @brief Go renderer / 围棋渲染器
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

#include <curses.h>
#include "board.h"

/**
 * =============================================================================
 * @class Renderer
 * @brief 围棋渲染器类 / Go Renderer Class
 * =============================================================================
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(const GoBoard& board);
    bool processInput(int& row, int& col, bool& pass);

    bool shouldClose() const { return shouldClose_; }
    void setShouldClose(bool value) { shouldClose_ = value; }

private:
    void drawStone(int row, int col, StoneColor color);
    void drawUI(const GoBoard& board);

    bool shouldClose_;
    int cursorRow_;
    int cursorCol_;
};