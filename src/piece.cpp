/**
 * @file piece.cpp
 * @brief 方块类实现 / Piece Class Implementation
 *
 * 包含所有七种方块的旋转状态定义。
 * Contains rotation state definitions for all seven pieces.
 *
 * 旋转系统采用SRS（超级旋转系统）标准。
 * Uses SRS (Super Rotation System) standard for rotation.
 *
 * @author NebulaLumino
 * @date 2026
 */

#include "piece.h"
#include <random>

std::array<std::array<int, 4>, 4> Piece::getShape() const {
    return PieceManager::getShape(type, rotation);
}

namespace PieceShapes {

/**
 * @namespace PieceShapes
 * @brief 方块形状定义命名空间 / Piece Shapes Definition Namespace
 *
 * 每个方块有4种旋转状态（0-3），存储在4×4×4的三维数组中。
 * Each piece has 4 rotation states (0-3), stored in 4×4×4 3D arrays.
 */

// ============================================================================
// 旋转状态表 / Rotation State Tables
// ============================================================================
// 索引格式：[rotation][row][col]，值为1表示有方块，0表示空
// Index format: [rotation][row][col], value 1 = filled, 0 = empty

/** I方块旋转状态 / I Piece Rotation States */
const int I_ROTATIONS[4][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},  // 0: 水平 / horizontal
    {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}},  // 1: 垂直 / vertical
    {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},  // 2: 水平 / horizontal
    {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}   // 3: 垂直 / vertical
};

/** T方块旋转状态 / T Piece Rotation States */
const int T_ROTATIONS[4][4][4] = {
    {{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
    {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
    {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}
};

/** S方块旋转状态 / S Piece Rotation States */
const int S_ROTATIONS[4][4][4] = {
    {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
    {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}
};

/** Z方块旋转状态 / Z Piece Rotation States */
const int Z_ROTATIONS[4][4][4] = {
    {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
    {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
    {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}}
};

/** J方块旋转状态 / J Piece Rotation States */
const int J_ROTATIONS[4][4][4] = {
    {{0,0,0,0},{1,0,0,0},{1,1,1,0},{0,0,0,0}},
    {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
    {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},
    {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}
};

/** L方块旋转状态 / L Piece Rotation States */
const int L_ROTATIONS[4][4][4] = {
    {{0,0,0,0},{0,0,1,0},{1,1,1,0},{0,0,0,0}},
    {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}},
    {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}
};

/** O方块旋转状态（全部相同，方块不旋转）/ O Piece Rotation States (all same, O doesn't rotate) */
const int O_ROTATIONS[4][4][4] = {
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}
};

} // namespace PieceShapes

// ============================================================================
// 方块管理器实现 / Piece Manager Implementation
// ============================================================================

Piece PieceManager::createRandomPiece() {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 6);

    Piece p;
    p.type = static_cast<PieceType>(dist(rng));
    p.rotation = 0;
    p.x = 3;  // 水平居中 / Horizontally centered
    p.y = 0;  // 顶部 / Top
    return p;
}

std::array<std::array<int, 4>, 4> PieceManager::getShape(PieceType type, int rotation) {
    std::array<std::array<int, 4>, 4> shape{};

    // 初始化为空 / Initialize to empty
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            shape[y][x] = 0;
        }
    }

    int r = rotation % 4;

    switch (type) {
        case PieceType::I:
            for (int y = 0; y < 4; ++y)
                for (int x = 0; x < 4; ++x)
                    shape[y][x] = PieceShapes::I_ROTATIONS[r][y][x];
            break;
        case PieceType::O:
            for (int y = 0; y < 4; ++y)
                for (int x = 0; x < 4; ++x)
                    shape[y][x] = PieceShapes::O_ROTATIONS[r][y][x];
            break;
        case PieceType::T:
            for (int y = 0; y < 4; ++y)
                for (int x = 0; x < 4; ++x)
                    shape[y][x] = PieceShapes::T_ROTATIONS[r][y][x];
            break;
        case PieceType::S:
            for (int y = 0; y < 4; ++y)
                for (int x = 0; x < 4; ++x)
                    shape[y][x] = PieceShapes::S_ROTATIONS[r][y][x];
            break;
        case PieceType::Z:
            for (int y = 0; y < 4; ++y)
                for (int x = 0; x < 4; ++x)
                    shape[y][x] = PieceShapes::Z_ROTATIONS[r][y][x];
            break;
        case PieceType::J:
            for (int y = 0; y < 4; ++y)
                for (int x = 0; x < 4; ++x)
                    shape[y][x] = PieceShapes::J_ROTATIONS[r][y][x];
            break;
        case PieceType::L:
            for (int y = 0; y < 4; ++y)
                for (int x = 0; x < 4; ++x)
                    shape[y][x] = PieceShapes::L_ROTATIONS[r][y][x];
            break;
    }

    return shape;
}
