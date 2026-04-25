/**
 * =============================================================================
 * @file renderer.h
 * @brief Sokoban renderer / 推箱子渲染器
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
#include "level.h"

/**
 * =============================================================================
 * @class Renderer
 * @brief 推箱子渲染器类 / Sokoban Renderer Class
 * =============================================================================
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(const Level& level);
    char processInput();

    bool shouldClose() const { return shouldClose_; }
    void setShouldClose(bool value) { shouldClose_ = value; }

private:
    void drawCell(int x, int y, Cell cell, bool highlight);
    void drawUI(const Level& level);

    bool shouldClose_;
};