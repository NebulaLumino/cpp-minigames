/**
 * =============================================================================
 * @file collision.h
 * @brief 碰撞检测与旋转系统 / Collision Detection and Rotation System
 * =============================================================================
 *
 * 实现SRS（超级旋转系统）的碰撞检测和墙踢（WALL KICK）功能。
 * Implements SRS (Super Rotation System) collision detection and wall kicks.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include "board.h"
#include "piece.h"

/**
 * =============================================================================
 * @name SRS墙踢偏移表 / SRS Wall Kick Offset Tables
 * =============================================================================
 *
 * 标准SRS墙踢数据，用于J、L、S、T、Z型方块。
 * Standard SRS wall kick data for J, L, S, T, Z pieces.
 *
 * 格式：{dx, dy} - 相对原位置的偏移量
 * 当直接旋转失败时，依次尝试这些偏移位置。
 *
 * Format: {dx, dy} - offset from original position
 * Tried in order when direct rotation fails.
 */
/// @{
/** J/L/S/T/Z型方块墙踢偏移（8组偏移）/ Wall kicks for J/L/S/T/Z pieces */
extern const int WALL_KICK_JLSTZ[8][2];

/** I型方块墙踢偏移（更大偏移范围）/ Wall kicks for I piece (larger offset range) */
extern const int WALL_KICK_I[8][2];
/// @}

/**
 * =============================================================================
 * @class Collision
 * @brief 碰撞检测类 / Collision Detection Class
 * =============================================================================
 *
 * 提供方块移动、旋转的碰撞检测功能。
 * Provides collision detection for piece movement and rotation.
 */
class Collision {
public:
    /**
     * @brief 检测方块是否与棋盘碰撞
     * @brief Checks if piece collides with the board
     *
     * @param board 棋盘引用 / Board reference
     * @param piece 方块引用 / Piece reference
     * @return 是否发生碰撞 / Whether collision occurred
     */
    static bool check(const Board& board, const Piece& piece);

    /**
     * @brief 检测方块能否向指定方向移动
     * @brief Checks if piece can move in the specified direction
     *
     * @param board 棋盘引用 / Board reference
     * @param piece 方块引用 / Piece reference
     * @param dx    X方向移动量 / X direction movement
     * @param dy    Y方向移动量 / Y direction movement
     * @return 是否可以移动 / Whether movement is possible
     */
    static bool canMove(const Board& board, const Piece& piece, int dx, int dy);

    /**
     * @brief 检测方块能否旋转（含墙踢尝试）
     * @brief Checks if piece can rotate (with wall kick attempts)
     *
     * @param board 棋盘引用 / Board reference
     * @param piece 方块引用 / Piece reference
     * @return 是否可以旋转 / Whether rotation is possible
     */
    static bool canRotate(const Board& board, const Piece& piece);

    /**
     * @brief 获取旋转后的方块（含墙踢偏移）
     * @brief Gets the rotated piece (with wall kick offset applied)
     *
     * @param board 棋盘引用 / Board reference
     * @param piece 方块引用 / Piece reference
     * @return 旋转并应用墙踢后的方块；如果失败返回原方块
     * @note 建议先调用canRotate()检查是否可以旋转
     */
    static Piece getRotatedPiece(const Board& board, const Piece& piece);

    /**
     * @brief 计算幽灵方块的Y位置（落点）
     * @brief Calculates ghost piece Y position (landing position)
     *
     * @param board 棋盘引用 / Board reference
     * @param piece 方块引用 / Piece reference
     * @return 幽灵方块的Y坐标 / Ghost piece Y coordinate
     */
    static int getGhostY(const Board& board, const Piece& piece);
};
