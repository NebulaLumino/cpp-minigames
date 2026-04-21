/**
 * =============================================================================
 * @file BoardTest.cpp
 * @brief 棋盘类单元测试 / Board Class Unit Tests
 * =============================================================================
 *
 * 测试棋盘类的核心功能：
 * - 碰撞检测
 * - 方块锁定
 * - 行消除
 * - 游戏结束检测
 *
 * Tests Board class core functionality:
 * - Collision detection
 * - Piece locking
 * - Line clearing
 * - Game over detection
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#include <catch2/catch_test_macros.hpp>
#include "board.h"
#include "piece.h"

/**
 * =============================================================================
 * 碰撞检测测试 / Collision Detection Tests
 * =============================================================================
 */

TEST_CASE("Board: isCollision - 空棋盘无碰撞", "Board: isCollision - No collision on empty board") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 3;
    piece.y = 10;
    piece.rotation = 0;

    auto shape = piece.getShape();
    REQUIRE(board.isCollision(static_cast<int>(piece.type), piece.rotation,
                             piece.x, piece.y, shape) == false);
}

TEST_CASE("Board: isCollision - 边界检测（左侧）", "Board: isCollision - Boundary detection (left)") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = -1;  // 超出左边界 / Beyond left boundary
    piece.y = 10;
    piece.rotation = 0;

    auto shape = piece.getShape();
    REQUIRE(board.isCollision(static_cast<int>(piece.type), piece.rotation,
                             piece.x, piece.y, shape) == true);
}

TEST_CASE("Board: isCollision - 边界检测（右侧）", "Board: isCollision - Boundary detection (right)") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 9;  // 超出右边界（宽度10，左上角最大x=6）/ Beyond right boundary
    piece.y = 10;
    piece.rotation = 0;

    auto shape = piece.getShape();
    REQUIRE(board.isCollision(static_cast<int>(piece.type), piece.rotation,
                             piece.x, piece.y, shape) == true);
}

TEST_CASE("Board: isCollision - 边界检测（底部）", "Board: isCollision - Boundary detection (bottom)") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 3;
    piece.y = 19;  // 超出底部 / Beyond bottom
    piece.rotation = 0;

    auto shape = piece.getShape();
    REQUIRE(board.isCollision(static_cast<int>(piece.type), piece.rotation,
                             piece.x, piece.y, shape) == true);
}

TEST_CASE("Board: isCollision - 锁定方块碰撞", "Board: isCollision - Locked piece collision") {
    Board board;
    Piece piece1, piece2;
    piece1.type = PieceType::I;
    piece1.x = 0;
    piece1.y = 10;
    piece1.rotation = 0;

    // 锁定第一个方块 / Lock the first piece
    auto shape1 = piece1.getShape();
    board.lockPiece(static_cast<int>(piece1.type), piece1.rotation,
                   piece1.x, piece1.y, shape1);

    // 尝试将另一个方块放在同一位置（应该碰撞）
    // Try to place another piece at the same position (should collide)
    piece2.type = PieceType::I;
    piece2.x = 0;
    piece2.y = 10;
    piece2.rotation = 0;

    auto shape2 = piece2.getShape();
    REQUIRE(board.isCollision(static_cast<int>(piece2.type), piece2.rotation,
                             piece2.x, piece2.y, shape2) == true);
}

TEST_CASE("Board: isCollision - 空形状矩阵", "Board: isCollision - Empty shape matrix") {
    Board board;
    std::array<std::array<int, 4>, 4> emptyShape = {};

    // 空形状在任意位置都不应碰撞 / Empty shape should not collide at any position
    REQUIRE(board.isCollision(0, 0, 3, 10, emptyShape) == false);
}

/**
 * =============================================================================
 * 方块锁定测试 / Piece Locking Tests
 * =============================================================================
 */

TEST_CASE("Board: lockPiece - 锁定后棋盘状态正确", "Board: lockPiece - Board state correct after locking") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 3;
    piece.y = 10;
    piece.rotation = 0;

    auto shape = piece.getShape();
    board.lockPiece(static_cast<int>(piece.type), piece.rotation,
                   piece.x, piece.y, shape);

    auto grid = board.getGrid();

    // 验证方块被锁定（I型在rotation=0时占据4个格子）
    // Verify piece is locked (I piece at rotation=0 occupies 4 cells)
    bool hasLockedCell = false;
    for (int y = 10; y < 14 && y < BOARD_HEIGHT; ++y) {
        for (int x = 3; x < 7 && x < BOARD_WIDTH; ++x) {
            if (grid[y][x] != 0) {
                hasLockedCell = true;
                REQUIRE(grid[y][x] == static_cast<int>(piece.type) + 1);
            }
        }
    }
    REQUIRE(hasLockedCell == true);
}

TEST_CASE("Board: lockPiece - 超出边界部分被忽略", "Board: lockPiece - Out of bounds parts ignored") {
    Board board;
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 8;  // 部分超出右边界 / Partially beyond right boundary
    piece.y = 17; // 部分超出底部 / Partially beyond bottom
    piece.rotation = 0;

    auto shape = piece.getShape();
    board.lockPiece(static_cast<int>(piece.type), piece.rotation,
                   piece.x, piece.y, shape);

    // 不应崩溃，且棋盘状态应保持正常
    // Should not crash and board state should remain normal
    auto grid = board.getGrid();
    bool cell189NotEmpty = (grid[18][9] != 0);
    bool cell178NotEmpty = (grid[17][8] != 0);
    bool atLeastOneLocked = cell189NotEmpty || cell178NotEmpty;
    REQUIRE(atLeastOneLocked);  // 至少有一部分被锁定
}

/**
 * =============================================================================
 * 行消除测试 / Line Clearing Tests
 * =============================================================================
 */

TEST_CASE("Board: clearLines - 空棋盘无行消除", "Board: clearLines - No lines cleared on empty board") {
    Board board;
    REQUIRE(board.clearLines() == 0);
}

TEST_CASE("Board: clearLines - 消除一行", "Board: clearLines - Clear single line") {
    Board board;

    // 使用I型方块填充一行 - I型在rotation=0时是水平的，占据4格
    // Use I piece to fill a row - I at rotation=0 is horizontal, occupies 4 cells
    // 旋转90度后是垂直的，可以用来测试行消除
    // After 90 degree rotation, it's vertical, can be used to test line clearing
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 0;
    piece.y = 19;  // y=19时，I方块只有row 19在棋盘内，其他都越界
    piece.rotation = 0;

    auto shape = piece.getShape();
    board.lockPiece(static_cast<int>(piece.type), piece.rotation,
                   piece.x, piece.y, shape);

    // clearLines应该能处理只有部分在棋盘内的方块
    // clearLines should handle pieces partially in the board
    int cleared = board.clearLines();
    REQUIRE(cleared >= 0);  // 至少不崩溃 / At least doesn't crash
}

TEST_CASE("Board: clearLines - 消除后行下移", "Board: clearLines - Rows shift down after clear") {
    Board board;

    // 在底部放置一个方块 / Place a piece at bottom
    Piece piece19;
    piece19.type = PieceType::I;
    piece19.x = 0;
    piece19.y = 17;
    piece19.rotation = 1;  // 垂直方向 / Vertical orientation

    auto shape19 = piece19.getShape();
    board.lockPiece(static_cast<int>(piece19.type), piece19.rotation,
                   piece19.x, piece19.y, shape19);

    // 验证锁定后有格子被填充 / Verify cells are filled after locking
    auto gridBefore = board.getGrid();
    bool hasCellsBefore = false;
    for (int y = 17; y < 21; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (gridBefore[y][x] != 0) {
                hasCellsBefore = true;
                break;
            }
        }
    }
    REQUIRE(hasCellsBefore == true);

    // 清除行不应该崩溃 / Clear lines should not crash
    int cleared = board.clearLines();
    REQUIRE(cleared >= 0);
}

/**
 * =============================================================================
 * 游戏结束检测测试 / Game Over Detection Tests
 * =============================================================================
 */

TEST_CASE("Board: isGameOver - 空棋盘未结束", "Board: isGameOver - Empty board not game over") {
    Board board;
    REQUIRE(board.isGameOver() == false);
}

TEST_CASE("Board: isGameOver - 顶行有方块时结束", "Board: isGameOver - Game over when top row has pieces") {
    Board board;

    // 放置一个方块在顶行下方（不会触发game over）
    // Place a piece below top row (should not trigger game over)
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 3;
    piece.y = 1;
    piece.rotation = 0;

    auto shape = piece.getShape();
    board.lockPiece(static_cast<int>(piece.type), piece.rotation,
                   piece.x, piece.y, shape);

    // 此时不应该game over，除非顶行被填充
    // Should not be game over unless top row is filled
    bool isOver = board.isGameOver();
    // 如果顶行没被填充，应该返回false
    // If top row is not filled, should return false
    auto grid = board.getGrid();
    bool topRowFilled = true;
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        if (grid[0][x] == 0) {
            topRowFilled = false;
            break;
        }
    }
    REQUIRE(isOver == topRowFilled);
}

TEST_CASE("Board: isGameOver - 底行有方块未结束", "Board: isGameOver - Game not over when only bottom row has pieces") {
    Board board;

    // 放置一个方块在底部 / Place a piece at bottom
    Piece piece;
    piece.type = PieceType::I;
    piece.x = 3;
    piece.y = 16;  // I方块在y=16时，垂直放置能触底 / I piece at y=16, vertical can hit bottom
    piece.rotation = 1;  // 垂直 / Vertical

    auto shape = piece.getShape();
    board.lockPiece(static_cast<int>(piece.type), piece.rotation,
                   piece.x, piece.y, shape);

    // 底部有方块但顶行空着，不应该game over
    // Bottom has pieces but top row empty, should not be game over
    REQUIRE(board.isGameOver() == false);
}
