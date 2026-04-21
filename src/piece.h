/**
 * =============================================================================
 * @file piece.h
 * @brief 方块类型与数据结构 / Piece Types and Data Structures
 * =============================================================================
 *
 * 定义七种经典俄罗斯方块：I、O、T、S、Z、J、L
 * Defines the seven classic Tetris pieces: I, O, T, S, Z, J, L
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include <array>

/**
 * =============================================================================
 * @enum PieceType
 * @brief 方块类型枚举 / Piece Type Enumeration
 * =============================================================================
 *
 * 七种标准俄罗斯方块及其颜色：
 * - I: 直线形（青色/Cyan）
 * - O: 正方形（黄色/Yellow）
 * - T: T形（紫色/Magenta）
 * - S: S形（绿色/Green）
 * - Z: Z形（红色/Red）
 * - J: J形（蓝色/Blue）
 * - L: L形（白色/White）
 */
enum class PieceType { I, O, T, S, Z, J, L };

/**
 * =============================================================================
 * @struct Piece
 * @brief 方块数据结构 / Piece Data Structure
 * =============================================================================
 *
 * 包含方块的类型、旋转状态和位置。
 * Pieces使用4×4矩阵表示其形状。
 *
 * Contains piece type, rotation state, and position.
 * Pieces use a 4×4 matrix to represent their shape.
 */
struct Piece {
    PieceType type;   /**< 方块类型 / Piece type */
    int rotation;     /**< 旋转状态(0-3) / Rotation state (0-3) */
    int x;            /**< 棋盘X坐标（列）/ Board X coordinate (column) */
    int y;            /**< 棋盘Y坐标（行）/ Board Y coordinate (row) */

    /**
     * @brief 获取当前方块的4×4形状矩阵
     * @brief Gets the 4×4 shape matrix for the current piece
     * @return 4×4形状矩阵，1表示有方块，0表示空
     */
    std::array<std::array<int, 4>, 4> getShape() const;
};

/**
 * =============================================================================
 * @class PieceManager
 * @brief 方块管理器 / Piece Manager
 * =============================================================================
 *
 * 负责创建随机方块和获取指定方块的形状。
 * Responsible for creating random pieces and getting piece shapes.
 */
class PieceManager {
public:
    /**
     * @brief 创建随机方块
     * @brief Creates a random piece
     * @return 新创建的方块，初始位置(3,0)（顶部中央）
     */
    static Piece createRandomPiece();

    /**
     * @brief 获取指定类型和旋转状态的形状
     * @brief Gets the shape for a given type and rotation
     * @param type     方块类型 / Piece type
     * @param rotation 旋转状态(0-3) / Rotation state (0-3)
     * @return 4×4形状矩阵 / 4×4 shape matrix
     */
    static std::array<std::array<int, 4>, 4> getShape(PieceType type, int rotation);
};
