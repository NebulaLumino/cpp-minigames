/**
 * @file board.cpp
 * @brief 棋盘类实现 / Board Class Implementation
 *
 * @author NebulaLumino
 * @date 2026
 */

#include "board.h"
#include <algorithm>

Board::Board() {
    // 初始化空棋盘 / Initialize empty board
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            grid_[y][x] = 0;
        }
    }
}

void Board::clear() {
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            grid_[y][x] = 0;
        }
    }
}

bool Board::isCollision(int pieceType, int rotation, int x, int y,
                         const std::array<std::array<int, 4>, 4>& shape) const {
    (void)pieceType;   // 未使用 / Unused
    (void)rotation;    // 未使用 / Unused

    for (int py = 0; py < 4; ++py) {
        for (int px = 0; px < 4; ++px) {
            if (shape[py][px] == 0) continue;

            int bx = x + px;
            int by = y + py;

            // 水平方向或下方越界 / Horizontal or bottom out of bounds
            if (bx < 0 || bx >= BOARD_WIDTH || by >= BOARD_HEIGHT) {
                return true;
            }

            // 上方越界时仅与已有方块检测碰撞
            // When above board, only check collision with existing pieces
            if (by < 0) continue;

            // 与已锁定格子碰撞 / Collision with locked cells
            if (grid_[by][bx] != 0) return true;
        }
    }
    return false;
}

void Board::lockPiece(int pieceType, int rotation, int x, int y,
                       const std::array<std::array<int, 4>, 4>& shape) {
    (void)rotation;  // 未使用 / Unused

    for (int py = 0; py < 4; ++py) {
        for (int px = 0; px < 4; ++px) {
            if (shape[py][px] == 0) continue;

            int bx = x + px;
            int by = y + py;

            if (by >= 0 && by < BOARD_HEIGHT && bx >= 0 && bx < BOARD_WIDTH) {
                // 存储为 pieceType+1 (1-7)，避免与空单元格(0)混淆
                // Store as pieceType+1 (1-7) to avoid confusion with empty cell (0)
                grid_[by][bx] = pieceType + 1;
            }
        }
    }
}

int Board::clearLines() {
    int linesCleared = 0;

    // 从底部向上检测 / Check from bottom to top
    for (int y = BOARD_HEIGHT - 1; y >= 0; ) {
        bool isFull = true;
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (grid_[y][x] == 0) {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            // 满行：所有行下移，清除顶部 / Full line: shift all rows down, clear top
            for (int ny = y; ny > 0; --ny) {
                for (int x = 0; x < BOARD_WIDTH; ++x) {
                    grid_[ny][x] = grid_[ny - 1][x];
                }
            }
            // 清除顶部行 / Clear top row
            for (int x = 0; x < BOARD_WIDTH; ++x) {
                grid_[0][x] = 0;
            }
            ++linesCleared;
            // 保持在当前位置，检查下一行（原上行下移后）
            // Stay at current position, check next row (old row above shifted down)
        } else {
            --y;
        }
    }
    return linesCleared;
}

bool Board::isGameOver() const {
    // 检查顶行是否有方块（会阻挡新方块生成）
    // Check if top row has blocks (would block new piece spawning)
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        if (grid_[0][x] != 0) return true;
    }
    return false;
}
