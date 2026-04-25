/**
 * =============================================================================
 * @file board.cpp
 * @brief Minesweeper board implementation / 扫雷游戏棋盘实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "board.h"
#include <cstdlib>
#include <ctime>
#include <queue>

Board::Board(int rows, int cols, int mines)
    : rows_(rows)
    , cols_(cols)
    , mines_(mines)
    , flagCount_(0)
    , gameOver_(false)
    , won_(false)
    , minesGenerated_(false) {

    // 初始化棋盘 / Initialize board
    board_.assign(rows_, std::vector<int>(cols_, 0));
    revealed_.assign(rows_, std::vector<bool>(cols_, false));
    flagged_.assign(rows_, std::vector<bool>(cols_, false));
}

void Board::generateMines(int firstRow, int firstCol) {
    srand(static_cast<unsigned>(time(nullptr)));

    int placed = 0;
    while (placed < mines_) {
        int r = rand() % rows_;
        int c = rand() % cols_;

        // 第一个点击位置周围不放雷 / No mines around first click
        if (abs(r - firstRow) <= 1 && abs(c - firstCol) <= 1) {
            continue;
        }

        if (board_[r][c] != -1) {
            board_[r][c] = -1;  // -1 表示地雷 / -1 means mine
            placed++;
        }
    }

    // 计算每个格子周围的地雷数 / Calculate adjacent mine count
    for (int r = 0; r < rows_; r++) {
        for (int c = 0; c < cols_; c++) {
            if (board_[r][c] == -1) continue;

            int count = 0;
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr;
                    int nc = c + dc;
                    if (inBounds(nr, nc) && board_[nr][nc] == -1) {
                        count++;
                    }
                }
            }
            board_[r][c] = count;
        }
    }

    minesGenerated_ = true;
}

bool Board::inBounds(int row, int col) const {
    return row >= 0 && row < rows_ && col >= 0 && col < cols_;
}

bool Board::reveal(int row, int col) {
    if (!inBounds(row, col) || revealed_[row][col] || flagged_[row][col]) {
        return false;
    }

    // 首次点击时生成地雷 / Generate mines on first click
    if (!minesGenerated_) {
        generateMines(row, col);
    }

    revealed_[row][col] = true;

    if (board_[row][col] == -1) {
        // 踩到地雷 / Hit a mine
        gameOver_ = true;
        return false;
    }

    // 如果是空格，进行洪水填充 / If empty, flood fill
    if (board_[row][col] == 0) {
        floodFill(row, col);
    }

    // 检查是否获胜 / Check win condition
    checkWin();

    return true;
}

void Board::floodFill(int row, int col) {
    std::queue<std::pair<int, int>> q;
    q.push({row, col});
    revealed_[row][col] = true;

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                if (dr == 0 && dc == 0) continue;
                int nr = r + dr;
                int nc = c + dc;

                if (!inBounds(nr, nc) || revealed_[nr][nc] || flagged_[nr][nc]) {
                    continue;
                }

                revealed_[nr][nc] = true;

                if (board_[nr][nc] == 0) {
                    q.push({nr, nc});
                }
            }
        }
    }
}

void Board::toggleFlag(int row, int col) {
    if (!inBounds(row, col) || revealed_[row][col]) {
        return;
    }

    flagged_[row][col] = !flagged_[row][col];
    flagCount_ += flagged_[row][col] ? 1 : -1;
}

bool Board::isMine(int row, int col) const {
    return inBounds(row, col) && board_[row][col] == -1;
}

bool Board::isRevealed(int row, int col) const {
    return inBounds(row, col) && revealed_[row][col];
}

bool Board::isFlagged(int row, int col) const {
    return inBounds(row, col) && flagged_[row][col];
}

int Board::getAdjacentMines(int row, int col) const {
    if (!inBounds(row, col)) return -1;
    return board_[row][col] == -1 ? -1 : board_[row][col];
}

int Board::getHiddenCount() const {
    int count = 0;
    for (int r = 0; r < rows_; r++) {
        for (int c = 0; c < cols_; c++) {
            if (!revealed_[r][c]) count++;
        }
    }
    return count;
}

int Board::getRevealedCount() const {
    int count = 0;
    for (int r = 0; r < rows_; r++) {
        for (int c = 0; c < cols_; c++) {
            if (revealed_[r][c]) count++;
        }
    }
    return count;
}

void Board::checkWin() {
    int hiddenNonMines = 0;
    for (int r = 0; r < rows_; r++) {
        for (int c = 0; c < cols_; c++) {
            if (!revealed_[r][c] && board_[r][c] != -1) {
                hiddenNonMines++;
            }
        }
    }
    if (hiddenNonMines == 0) {
        won_ = true;
        gameOver_ = true;
    }
}