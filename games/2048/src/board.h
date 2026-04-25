/**
 * =============================================================================
 * @file board.h
 * @brief 2048 game board / 2048游戏棋盘
 * =============================================================================
 *
 * 管理2048游戏的棋盘状态、滑块合并和游戏逻辑。
 * Manages 2048 board state, tile merging, and game logic.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include <vector>

/**
 * =============================================================================
 * @class GameBoard
 * @brief 2048棋盘类 / 2048 Game Board Class
 * =============================================================================
 */
class GameBoard {
public:
    GameBoard();

    // 重置游戏 / Reset game
    void reset();

    // 滑块移动 / Tile movement
    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();

    // 获取棋盘状态 / Get board state
    int getCell(int row, int col) const;
    int getScore() const { return score_; }
    int getBestScore() const { return bestScore_; }
    int getHighestTile() const;
    bool isGameOver() const { return gameOver_; }
    bool hasWon() const { return won_; }

    // 检查是否可以移动 / Check if move possible
    bool canMove() const;

private:
    bool mergeRow(std::vector<int>& row);
    void addRandomTile();
    bool canMoveLeft() const;
    bool canMoveRight() const;
    bool canMoveUp() const;
    bool canMoveDown() const;
    void checkGameOver();

    int score_;
    int bestScore_;
    bool gameOver_;
    bool won_;
    std::vector<std::vector<int>> grid_;
};