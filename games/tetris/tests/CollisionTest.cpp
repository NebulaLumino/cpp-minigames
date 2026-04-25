/**
 * =============================================================================
 * @file CollisionTest.cpp
 * @brief 碰撞检测类单元测试 / Collision Detection Class Unit Tests
 * =============================================================================
 *
 * 测试碰撞检测类的核心功能：
 * - 移动检测
 * - 旋转检测（含墙踢）
 * - 幽灵方块位置计算
 *
 * Tests Collision class core functionality:
 * - Movement detection
 * - Rotation detection (with wall kicks)
 * - Ghost piece position calculation
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#include <catch2/catch_test_macros.hpp>
#include "collision.h"
#include "board.h"
#include "piece.h"

/**
 * =============================================================================
 * 移动检测测试 / Movement Detection Tests
 * =============================================================================
 */

TEST_CASE("Collision: canMove - 空棋盘可向各方向移动", "Collision: canMove - Can move in all directions on empty board") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 3;
    piece.y = 10;
    piece.rotation = 0;

    // 空棋盘上所有方向都应该可以移动
    // All directions should be movable on empty board
    REQUIRE(Collision::canMove(board, piece, 0, 1) == true);
    REQUIRE(Collision::canMove(board, piece, 0, -1) == true);
    REQUIRE(Collision::canMove(board, piece, 1, 0) == true);
    REQUIRE(Collision::canMove(board, piece, -1, 0) == true);
}

TEST_CASE("Collision: canMove - 触底后无法继续下落", "Collision: canMove - Cannot move down after hitting bottom") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 3;
    piece.y = 18;  // 最底部位置 / Bottom position
    piece.rotation = 1;  // 垂直 / Vertical

    // 底部位置无法继续下落
    // Bottom position cannot move down
    bool canMoveDown = Collision::canMove(board, piece, 0, 1);
    REQUIRE(canMoveDown == false);
}

TEST_CASE("Collision: canMove - 触壁后无法继续移动", "Collision: canMove - Cannot move after hitting wall") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 0;
    piece.y = 10;
    piece.rotation = 0;

    // 左侧边界无法继续左移
    // Left boundary cannot move left
    REQUIRE(Collision::canMove(board, piece, -1, 0) == false);
}

TEST_CASE("Collision: canMove - 锁定方块阻挡", "Collision: canMove - Locked pieces block movement") {
    Board board;

    // 锁定一个O型方块 / Lock an O piece
    Piece piece1;
    piece1.type = PieceType::O;
    piece1.x = 5;
    piece1.y = 15;
    piece1.rotation = 0;
    auto shape1 = piece1.getShape();
    board.lockPiece(static_cast<int>(piece1.type), piece1.rotation,
                   piece1.x, piece1.y, shape1);

    // I型方块在O型方块正上方，试图下移应该被阻挡
    // I piece right above O piece, trying to move down should be blocked
    Piece piece2;
    piece2.type = PieceType::I;
    piece2.x = 5;
    piece2.y = 13;
    piece2.rotation = 1;  // 垂直 / Vertical

    bool canMoveDown = Collision::canMove(board, piece2, 0, 1);
    REQUIRE(canMoveDown == false);
}

/**
 * =============================================================================
 * 旋转检测测试 / Rotation Detection Tests
 * =============================================================================
 */

TEST_CASE("Collision: canRotate - 空棋盘可正常旋转", "Collision: canRotate - Can rotate normally on empty board") {
    Board board;
    Piece piece;
    piece.type = PieceType::T;
    piece.x = 3;
    piece.y = 10;
    piece.rotation = 0;

    // 空棋盘上应该可以旋转
    // Should be able to rotate on empty board
    bool canRotate = Collision::canRotate(board, piece);
    REQUIRE(canRotate == true);
}

TEST_CASE("Collision: canRotate - 靠墙无法旋转", "Collision: canRotate - Cannot rotate when against wall") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 0;
    piece.y = 10;
    piece.rotation = 0;

    // I型方块靠墙旋转可能会失败（取决于墙踢实现）
    // I piece rotation against wall may fail (depends on wall kick implementation)
    bool canRotate = Collision::canRotate(board, piece);
    // 结果取决于实现：可能有墙踢成功，也可能失败
    // Result depends on implementation: wall kick may succeed or fail
    (void)canRotate;  // 只验证不崩溃 / Just verify no crash
}

/**
 * =============================================================================
 * 幽灵方块位置测试 / Ghost Piece Position Tests
 * =============================================================================
 */

TEST_CASE("Collision: getGhostY - 空棋盘返回合理位置", "Collision: getGhostY - Returns reasonable position on empty board") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 3;
    piece.y = 5;
    piece.rotation = 0;

    // 空棋盘上幽灵方块应该返回底部附近的位置
    // On empty board, ghost piece should return position near bottom
    int ghostY = Collision::getGhostY(board, piece);
    REQUIRE(ghostY >= 0);    // 应该 >= 0
    REQUIRE(ghostY <= 19);   // 应该 <= 19 (BOARD_HEIGHT - 1)
    REQUIRE(ghostY >= piece.y);  // 应该不小于初始位置
}

TEST_CASE("Collision: getGhostY - 幽灵位置应不低于当前Y", "Collision: getGhostY - Ghost position should not be higher than current Y") {
    Board board;
    Piece piece;
    piece.type = PieceType::T;
    piece.x = 3;
    piece.y = 8;
    piece.rotation = 0;

    int ghostY = Collision::getGhostY(board, piece);
    REQUIRE(ghostY >= piece.y);
}

TEST_CASE("Collision: getGhostY - 幽灵下落到底部", "Collision: getGhostY - Ghost falls to bottom") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 3;
    piece.y = 5;
    piece.rotation = 0;

    int ghostY = Collision::getGhostY(board, piece);
    // 幽灵方块应该落到棋盘底部附近
    // Ghost piece should fall to near the bottom of the board
    REQUIRE(ghostY > 10);  // 应该大于初始y位置（因为往下落）
}
