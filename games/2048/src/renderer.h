/**
 * =============================================================================
 * @file renderer.h
 * @brief 2048 renderer / 2048渲染器
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
 * @brief 2048渲染器类 / 2048 Renderer Class
 * =============================================================================
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(const GameBoard& board);
    char processInput();

    bool shouldClose() const { return shouldClose_; }
    void setShouldClose(bool value) { shouldClose_ = value; }

private:
    void drawCell(int row, int col, int value);
    void drawUI(const GameBoard& board);
    int getColorForValue(int value);

    bool shouldClose_;
};