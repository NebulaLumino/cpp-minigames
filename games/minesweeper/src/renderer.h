/**
 * =============================================================================
 * @file renderer.h
 * @brief Minesweeper renderer / 扫雷游戏渲染器
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

class Board;

/**
 * =============================================================================
 * @class Renderer
 * @brief 扫雷游戏渲染器类 / Minesweeper Renderer Class
 * =============================================================================
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(const Board& board);
    bool processInput(int& row, int& col, char& action);

    bool shouldClose() const { return shouldClose_; }
    void setShouldClose(bool value) { shouldClose_ = value; }

    void showGameOver(bool won);
    void showMessage(const char* msg);

private:
    void drawCell(int row, int col, char c, bool highlight);
    void drawBoard(const Board& board);
    void drawUI(const Board& board);

    bool shouldClose_;
    int selectedRow_;
    int selectedCol_;
};