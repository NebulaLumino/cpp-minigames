/**
 * =============================================================================
 * @file board.h
 * @brief Minesweeper board / 扫雷游戏棋盘
 * =============================================================================
 *
 * 管理扫雷游戏的棋盘状态、地雷布局和游戏逻辑。
 * Manages all state and logic for the Minesweeper board.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include <vector>

/**
 * =============================================================================
 * @class Board
 * @brief 扫雷游戏棋盘类 / Minesweeper Board Class
 * =============================================================================
 */
class Board {
public:
    Board(int rows = 9, int cols = 9, int mines = 10);

    // 翻转指定位置的格子 / Reveal cell at position
    bool reveal(int row, int col);

    // 标记/取消标记地雷 / Toggle mine flag
    void toggleFlag(int row, int col);

    // 获取格子状态 / Get cell state
    bool isMine(int row, int col) const;
    bool isRevealed(int row, int col) const;
    bool isFlagged(int row, int col) const;
    int getAdjacentMines(int row, int col) const;

    // 游戏状态 / Game state
    bool isGameOver() const { return gameOver_; }
    bool isWin() const { return won_; }
    int getRows() const { return rows_; }
    int getCols() const { return cols_; }
    int getTotalMines() const { return mines_; }
    int getFlagCount() const { return flagCount_; }

    // 测试接口 / Test interface
    int getHiddenCount() const;
    int getRevealedCount() const;

    // 游戏结束 / End game
    void setGameOver() { gameOver_ = true; }
    void setWon() { won_ = true; }

private:
    void generateMines(int firstRow, int firstCol);
    void floodFill(int row, int col);
    bool inBounds(int row, int col) const;
    void checkWin();

    int rows_;
    int cols_;
    int mines_;
    int flagCount_;
    bool gameOver_;
    bool won_;
    bool minesGenerated_;

    // 0 = hidden, 1 = revealed
    std::vector<std::vector<bool>> revealed_;
    // 0 = empty, -1 = mine, 1-8 = adjacent count
    std::vector<std::vector<int>> board_;
    // 0 = unflagged, 1 = flagged
    std::vector<std::vector<bool>> flagged_;
};