/**
 * =============================================================================
 * @file board.cpp
 * @brief 2048 game board implementation / 2048游戏棋盘实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "board.h"
#include <cstdlib>
#include <algorithm>

GameBoard::GameBoard()
    : score_(0)
    , bestScore_(0)
    , gameOver_(false)
    , won_(false) {

    grid_.assign(4, std::vector<int>(4, 0));
    addRandomTile();
    addRandomTile();
}

void GameBoard::reset() {
    score_ = 0;
    gameOver_ = false;
    won_ = false;
    grid_.assign(4, std::vector<int>(4, 0));
    addRandomTile();
    addRandomTile();
}

void GameBoard::addRandomTile() {
    // 找到所有空位置 / Find all empty positions
    std::vector<std::pair<int, int>> emptyCells;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (grid_[r][c] == 0) {
                emptyCells.push_back({r, c});
            }
        }
    }

    if (emptyCells.empty()) {
        return;
    }

    // 随机选择一个空位置 / Randomly select an empty position
    int idx = rand() % emptyCells.size();
    int row = emptyCells[idx].first;
    int col = emptyCells[idx].second;

    // 90%概率生成2, 10%概率生成4 / 90% chance of 2, 10% chance of 4
    grid_[row][col] = (rand() % 10 < 9) ? 2 : 4;
}

bool GameBoard::mergeRow(std::vector<int>& row) {
    bool moved = false;

    // 移除空格 / Remove zeros
    std::vector<int> nonZero;
    for (int val : row) {
        if (val != 0) {
            nonZero.push_back(val);
        }
    }

    // 合并相邻的相同数字 / Merge adjacent equal numbers
    std::vector<int> merged;
    int i = 0;
    while (i < nonZero.size()) {
        if (i + 1 < nonZero.size() && nonZero[i] == nonZero[i + 1]) {
            int combined = nonZero[i] * 2;
            merged.push_back(combined);
            score_ += combined;

            // 检查是否达到2048 / Check for 2048 win
            if (combined == 2048) {
                won_ = true;
            }

            i += 2;
        } else {
            merged.push_back(nonZero[i]);
            i++;
        }
    }

    // 补零使长度回到4 / Pad with zeros to length 4
    while (merged.size() < 4) {
        merged.push_back(0);
    }

    // 检查是否有变化 / Check if changed
    for (int j = 0; j < 4; j++) {
        if (row[j] != merged[j]) {
            moved = true;
            row[j] = merged[j];
        }
    }

    return moved;
}

bool GameBoard::moveLeft() {
    if (!canMoveLeft()) {
        return false;
    }

    bool moved = false;
    for (int r = 0; r < 4; r++) {
        std::vector<int> row = grid_[r];
        bool rowMoved = mergeRow(row);
        if (rowMoved) {
            moved = true;
            grid_[r] = row;
        }
    }

    if (moved) {
        addRandomTile();
    }

    checkGameOver();
    return moved;
}

bool GameBoard::moveRight() {
    if (!canMoveRight()) {
        return false;
    }

    bool moved = false;
    for (int r = 0; r < 4; r++) {
        std::vector<int> row = grid_[r];
        std::reverse(row.begin(), row.end());
        bool rowMoved = mergeRow(row);
        if (rowMoved) {
            moved = true;
            std::reverse(row.begin(), row.end());
            grid_[r] = row;
        }
    }

    if (moved) {
        addRandomTile();
    }

    checkGameOver();
    return moved;
}

bool GameBoard::moveUp() {
    if (!canMoveUp()) {
        return false;
    }

    bool moved = false;
    for (int c = 0; c < 4; c++) {
        std::vector<int> col = {grid_[0][c], grid_[1][c], grid_[2][c], grid_[3][c]};
        bool colMoved = mergeRow(col);
        if (colMoved) {
            moved = true;
            for (int r = 0; r < 4; r++) {
                grid_[r][c] = col[r];
            }
        }
    }

    if (moved) {
        addRandomTile();
    }

    checkGameOver();
    return moved;
}

bool GameBoard::moveDown() {
    if (!canMoveDown()) {
        return false;
    }

    bool moved = false;
    for (int c = 0; c < 4; c++) {
        std::vector<int> col = {grid_[3][c], grid_[2][c], grid_[1][c], grid_[0][c]};
        bool colMoved = mergeRow(col);
        if (colMoved) {
            moved = true;
            for (int r = 0; r < 4; r++) {
                grid_[3 - r][c] = col[r];
            }
        }
    }

    if (moved) {
        addRandomTile();
    }

    checkGameOver();
    return moved;
}

int GameBoard::getCell(int row, int col) const {
    if (row < 0 || row >= 4 || col < 0 || col >= 4) {
        return -1;
    }
    return grid_[row][col];
}

int GameBoard::getHighestTile() const {
    int highest = 0;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (grid_[r][c] > highest) {
                highest = grid_[r][c];
            }
        }
    }
    return highest;
}

bool GameBoard::canMoveLeft() const {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (c > 0 && grid_[r][c] != 0) {
                if (grid_[r][c - 1] == 0 || grid_[r][c - 1] == grid_[r][c]) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameBoard::canMoveRight() const {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (c < 3 && grid_[r][c] != 0) {
                if (grid_[r][c + 1] == 0 || grid_[r][c + 1] == grid_[r][c]) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameBoard::canMoveUp() const {
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            if (r > 0 && grid_[r][c] != 0) {
                if (grid_[r - 1][c] == 0 || grid_[r - 1][c] == grid_[r][c]) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameBoard::canMoveDown() const {
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            if (r < 3 && grid_[r][c] != 0) {
                if (grid_[r + 1][c] == 0 || grid_[r + 1][c] == grid_[r][c]) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameBoard::canMove() const {
    return canMoveLeft() || canMoveRight() || canMoveUp() || canMoveDown();
}

void GameBoard::checkGameOver() {
    if (!canMove()) {
        gameOver_ = true;
        if (score_ > bestScore_) {
            bestScore_ = score_;
        }
    }
}