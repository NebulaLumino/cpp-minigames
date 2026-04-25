/**
 * =============================================================================
 * @file level.h
 * @brief Sokoban level management / 推箱子关卡管理
 * =============================================================================
 *
 * 管理推箱子游戏的关卡数据、玩家位置、箱子和目标。
 * Manages Sokoban level data, player position, boxes and targets.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include <vector>
#include <string>

/**
 * @brief 推箱子游戏元素 / Sokoban game elements
 */
enum Cell {
    EMPTY = 0,
    WALL = 1,
    BOX = 2,
    TARGET = 3,
    BOX_ON_TARGET = 4,
    PLAYER = 5,
    PLAYER_ON_TARGET = 6
};

/**
 * =============================================================================
 * @class Level
 * @brief 推箱子关卡类 / Sokoban Level Class
 * =============================================================================
 */
class Level {
public:
    Level();

    // 加载关卡 / Load level
    bool loadLevel(int levelNum);
    int getCurrentLevel() const { return currentLevel_; }
    int getTotalLevels() const { return static_cast<int>(levelData_.size()); }

    // 获取格子类型 / Get cell type
    Cell getCell(int x, int y) const;
    bool isWall(int x, int y) const;
    bool isBox(int x, int y) const;
    bool isTarget(int x, int y) const;

    // 玩家位置 / Player position
    int getPlayerX() const { return playerX_; }
    int getPlayerY() const { return playerY_; }

    // 移动 / Move
    bool movePlayer(int dx, int dy);
    bool pushBox(int boxX, int boxY, int dx, int dy);

    // 胜利检测 / Win check
    bool checkWin() const;
    int getBoxesOnTarget() const;
    int getTotalBoxes() const { return totalBoxes_; }

    // 重置 / Reset
    void reset();

    // 获取关卡尺寸 / Get level size
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    void parseLevel(const std::string& levelStr);
    bool canMove(int x, int y) const;
    void updatePlayerPosition();

    int currentLevel_;
    int width_;
    int height_;
    int playerX_;
    int playerY_;
    int totalBoxes_;
    int boxesOnTarget_;

    std::vector<std::vector<Cell>> grid_;

    // 预设关卡 / Predefined levels
    static const std::vector<std::string> levelData_;
};