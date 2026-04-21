/**
 * @file collision.cpp
 * @brief 碰撞检测实现 / Collision Detection Implementation
 *
 * @author NebulaLumino
 * @date 2026
 */

#include "collision.h"

// ============================================================================
// SRS墙踢数据 / SRS Wall Kick Data
// ============================================================================
// 格式: {dx, dy} - 从原位置测试的偏移量
// Format: {dx, dy} - offset from original position to test

/** J/L/S/T/Z型方块墙踢偏移表（8组偏移）/ Wall kick offset table for J/L/S/T/Z (8 offset pairs) */
const int WALL_KICK_JLSTZ[8][2] = {
    { 0,  0},   // 原位置 / original position
    { 0, -1},   // 上 / up
    { 0,  1},   // 下 / down
    {-1,  0},   // 左 / left
    { 1,  0},   // 右 / right
    {-1, -1},   // 左上 / up-left
    { 1, -1},   // 右上 / up-right
    {-1,  1}    // 左下 / down-left
};

/** I型方块墙踢偏移表（8组偏移）/ Wall kick offset table for I piece (8 offset pairs) */
const int WALL_KICK_I[8][2] = {
    { 0,  0},   // 原位置 / original position
    { 0, -2},   // 上2格 / up 2
    { 0,  2},   // 下2格 / down 2
    {-1,  0},   // 左 / left
    { 1,  0},   // 右 / right
    {-2,  0},   // 左2格 / left 2
    { 2,  0},   // 右2格 / right 2
    {-1, -1}    // 左上 / up-left
};

// ============================================================================
// 碰撞检测实现 / Collision Detection Implementation
// ============================================================================

bool Collision::check(const Board& board, const Piece& piece) {
    auto shape = piece.getShape();
    return board.isCollision(static_cast<int>(piece.type), piece.rotation,
                             piece.x, piece.y, shape);
}

bool Collision::canMove(const Board& board, const Piece& piece, int dx, int dy) {
    Piece moved = piece;
    moved.x += dx;
    moved.y += dy;
    return !check(board, moved);
}

bool Collision::canRotate(const Board& board, const Piece& piece) {
    // 使用getRotatedPiece检查，不相等说明可以旋转
    // Use getRotatedPiece, if not equal means rotation succeeded
    return getRotatedPiece(board, piece).rotation != piece.rotation ||
           (getRotatedPiece(board, piece).x == piece.x && getRotatedPiece(board, piece).y == piece.y);
}

Piece Collision::getRotatedPiece(const Board& board, const Piece& piece) {
    // 选择墙踢表 / Select wall kick table
    const int (*kicks)[2];
    if (piece.type == PieceType::I) {
        kicks = WALL_KICK_I;
    } else {
        kicks = WALL_KICK_JLSTZ;
    }

    // 尝试原位置和所有墙踢偏移 / Try original position and all wall kick offsets
    for (int i = 0; i < 8; ++i) {
        Piece rotated = piece;
        rotated.rotation = (rotated.rotation + 1) % 4;
        rotated.x += kicks[i][0];
        rotated.y += kicks[i][1];

        if (!check(board, rotated)) {
            return rotated;  // 成功返回旋转后的方块 / Return rotated piece on success
        }
    }

    // 所有尝试均失败，返回原方块（旋转状态会改变，但位置不变）
    // All attempts failed, return original piece (rotation changes but position stays)
    Piece result = piece;
    result.rotation = (piece.rotation + 1) % 4;
    return result;
}

int Collision::getGhostY(const Board& board, const Piece& piece) {
    int ghostY = piece.y;
    Piece testPiece = piece;

    // 向下移动直到碰撞 / Move down until collision
    while (!check(board, testPiece)) {
        ghostY = testPiece.y;
        testPiece.y++;
    }

    return ghostY;
}
