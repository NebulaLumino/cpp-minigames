/**
 * @file board.h
 * @brief 游戏棋盘数据结构 / Game Board Data Structure
 *
 * 棋盘是一个10列×20行的网格，存储已固定到位的方块。
 * The board is a 10-column × 20-row grid that stores locked pieces.
 *
 * @author NebulaLumino
 * @date 2026
 */

#pragma once

#include <array>

/** 棋盘宽度（列数）/ Board width (columns) */
constexpr int BOARD_WIDTH = 10;

/** 棋盘高度（行数）/ Board height (rows) */
constexpr int BOARD_HEIGHT = 20;

/**
 * @class Board
 * @brief 俄罗斯方块游戏棋盘 / Tetris Game Board
 *
 * 管理游戏区域的网格数据，包括：
 * - 存储已锁定方块的颜色信息（1-7对应7种方块类型，0表示空）
 * - 检测碰撞（边界和其他已锁定方块）
 * - 消除满行并计分
 *
 * Manages the grid data for the play area:
 * - Stores locked piece color info (1-7 for piece types, 0 for empty)
 * - Detects collisions (boundaries and locked pieces)
 * - Clears full lines and scores
 */
class Board {
public:
    /**
     * @brief 构造函数，初始化空棋盘 / Constructor, initializes empty board
     */
    Board();

    /**
     * @brief 清空棋盘，重置游戏状态 / Clears the board, resets game state
     */
    void clear();

    /**
     * @brief 检测指定位置的碰撞
     * @param pieceType 方块类型(0-6) / Piece type (0-6)
     * @param rotation 旋转状态(0-3) / Rotation state (0-3)
     * @param x 方块左上角X坐标 / X coordinate of piece's top-left corner
     * @param y 方块左上角Y坐标 / Y coordinate of piece's top-left corner
     * @param shape 4×4方块形状矩阵 / 4×4 piece shape matrix
     * @return 是否发生碰撞 / Whether collision occurred
     */
    bool isCollision(int pieceType, int rotation, int x, int y,
                     const std::array<std::array<int, 4>, 4>& shape) const;

    /**
     * @brief 将方块锁定到棋盘 / Locks a piece onto the board
     * @param pieceType 方块类型(0-6) / Piece type (0-6)
     * @param rotation 旋转状态(0-3) / Rotation state (0-3)
     * @param x 方块左上角X坐标 / X coordinate of piece's top-left corner
     * @param y 方块左上角Y坐标 / Y coordinate of piece's top-left corner
     * @param shape 4×4方块形状矩阵 / 4×4 piece shape matrix
     */
    void lockPiece(int pieceType, int rotation, int x, int y,
                   const std::array<std::array<int, 4>, 4>& shape);

    /**
     * @brief 清除满行并返回清除的行数
     * @return 被清除的行数 / Number of lines cleared
     *
     * 从下往上检测满行，清除后上面的行下移。
     * Checks from bottom to top, shifts rows above down after clearing.
     */
    int clearLines();

    /**
     * @brief 检查游戏是否结束
     * @return 游戏是否结束 / Whether game is over
     */
    bool isGameOver() const;

    /**
     * @brief 获取棋盘网格数据的只读引用
     * @return 棋盘网格引用 / Reference to board grid
     */
    const std::array<std::array<int, BOARD_WIDTH>, BOARD_HEIGHT>& getGrid() const {
        return grid_;
    }

private:
    /** 棋盘网格：存储每个格子的颜色值(0=空, 1-7=方块类型) / Grid: stores cell colors (0=empty, 1-7=piece type) */
    std::array<std::array<int, BOARD_WIDTH>, BOARD_HEIGHT> grid_;
};
