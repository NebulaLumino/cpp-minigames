/**
 * =============================================================================
 * @file renderer.h
 * @brief Chess renderer / 国际象棋渲染器
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
 * @brief 国际象棋渲染器类 / Chess Renderer Class
 * =============================================================================
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(const ChessBoard& board);
    bool processInput(std::string& moveStr);

    bool shouldClose() const { return shouldClose_; }
    void setShouldClose(bool value) { shouldClose_ = value; }

private:
    void drawPiece(int row, int col, const Piece& piece);
    void drawUI(const ChessBoard& board);

    bool shouldClose_;
    int cursorRow_;
    int cursorCol_;
    bool inputMode_;
    std::string inputBuffer_;
};