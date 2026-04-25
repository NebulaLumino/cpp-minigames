/**
 * =============================================================================
 * @file level.cpp
 * @brief Sokoban level implementation / 推箱子关卡实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "level.h"

// 预设关卡数据 / Predefined level data
// 符号: # = 墙, @ = 玩家, $ = 箱子, . = 目标, * = 箱子在目标, + = 玩家在目标
const std::vector<std::string> Level::levelData_ = {
    // Level 1 - 简单 / Simple
    "########\n"
    "#      #\n"
    "# $  . #\n"
    "#  @   #\n"
    "#      #\n"
    "########",

    // Level 2 - 简单 / Simple
    "########\n"
    "#      #\n"
    "# $ .  #\n"
    "# $ .  #\n"
    "#  @   #\n"
    "#      #\n"
    "########",

    // Level 3 - 中等 / Medium
    "##########\n"
    "#        #\n"
    "#  $$$   #\n"
    "#  ...   #\n"
    "#   @    #\n"
    "#        #\n"
    "##########",

    // Level 4 - 中等 / Medium
    "##########\n"
    "#        #\n"
    "#  $ $   #\n"
    "#  ...   #\n"
    "#   @    #\n"
    "#        #\n"
    "##########",

    // Level 5 - 困难 / Hard
    "############\n"
    "#          #\n"
    "# $$$$$    #\n"
    "# .....    #\n"
    "#   @      #\n"
    "#          #\n"
    "############"
};

Level::Level()
    : currentLevel_(0)
    , width_(0)
    , height_(0)
    , playerX_(0)
    , playerY_(0)
    , totalBoxes_(0)
    , boxesOnTarget_(0) {
    loadLevel(0);
}

bool Level::loadLevel(int levelNum) {
    if (levelNum < 0 || levelNum >= static_cast<int>(levelData_.size())) {
        return false;
    }

    currentLevel_ = levelNum;
    parseLevel(levelData_[levelNum]);
    return true;
}

void Level::parseLevel(const std::string& levelStr) {
    grid_.clear();
    totalBoxes_ = 0;
    boxesOnTarget_ = 0;

    std::vector<std::string> lines;
    std::string line;
    for (char c : levelStr) {
        if (c == '\n') {
            lines.push_back(line);
            line.clear();
        } else {
            line += c;
        }
    }
    if (!line.empty()) {
        lines.push_back(line);
    }

    height_ = static_cast<int>(lines.size());
    width_ = 0;
    for (const auto& l : lines) {
        if (static_cast<int>(l.size()) > width_) {
            width_ = static_cast<int>(l.size());
        }
    }

    grid_.assign(height_, std::vector<Cell>(width_, EMPTY));

    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < static_cast<int>(lines[y].size()); x++) {
            char c = lines[y][x];
            switch (c) {
                case '#':
                    grid_[y][x] = WALL;
                    break;
                case '$':
                    grid_[y][x] = BOX;
                    totalBoxes_++;
                    break;
                case '.':
                    grid_[y][x] = TARGET;
                    break;
                case '*':
                    grid_[y][x] = BOX_ON_TARGET;
                    totalBoxes_++;
                    boxesOnTarget_++;
                    break;
                case '@':
                    grid_[y][x] = EMPTY;
                    playerX_ = x;
                    playerY_ = y;
                    break;
                case '+':
                    grid_[y][x] = TARGET;
                    playerX_ = x;
                    playerY_ = y;
                    break;
                default:
                    grid_[y][x] = EMPTY;
                    break;
            }
        }
    }
}

Cell Level::getCell(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return WALL;
    }
    return grid_[y][x];
}

bool Level::isWall(int x, int y) const {
    return getCell(x, y) == WALL;
}

bool Level::isBox(int x, int y) const {
    Cell c = getCell(x, y);
    return c == BOX || c == BOX_ON_TARGET;
}

bool Level::isTarget(int x, int y) const {
    Cell c = getCell(x, y);
    return c == TARGET || c == BOX_ON_TARGET || c == PLAYER_ON_TARGET;
}

bool Level::canMove(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return false;
    }
    return !isWall(x, y);
}

bool Level::movePlayer(int dx, int dy) {
    int newX = playerX_ + dx;
    int newY = playerY_ + dy;

    if (!canMove(newX, newY)) {
        return false;
    }

    // 检查是否有箱子 / Check if there's a box
    if (isBox(newX, newY)) {
        // 尝试推动箱子 / Try to push box
        if (!pushBox(newX, newY, dx, dy)) {
            return false;
        }
    }

    // 更新玩家位置 / Update player position
    updatePlayerPosition();
    playerX_ = newX;
    playerY_ = newY;
    updatePlayerPosition();

    return true;
}

bool Level::pushBox(int boxX, int boxY, int dx, int dy) {
    int behindX = boxX + dx;
    int behindY = boxY + dy;

    // 检查箱子后面是否可以移动 / Check if behind box is movable
    if (!canMove(behindX, behindY)) {
        return false;
    }

    // 检查后面是否是另一个箱子 / Check if behind is another box
    if (isBox(behindX, behindY)) {
        return false;
    }

    // 移动箱子 / Move box
    Cell currentCell = getCell(boxX, boxY);
    bool wasOnTarget = (currentCell == BOX_ON_TARGET);

    grid_[boxY][boxX] = wasOnTarget ? TARGET : EMPTY;
    if (wasOnTarget) {
        boxesOnTarget_--;
    }

    Cell behindCell = getCell(behindX, behindY);
    bool nowOnTarget = (behindCell == TARGET);

    grid_[behindY][behindX] = nowOnTarget ? BOX_ON_TARGET : BOX;
    if (nowOnTarget) {
        boxesOnTarget_++;
    }

    return true;
}

void Level::updatePlayerPosition() {
    // 清除原位置的玩家 / Clear old player position
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            if (grid_[y][x] == PLAYER) {
                grid_[y][x] = EMPTY;
            } else if (grid_[y][x] == PLAYER_ON_TARGET) {
                grid_[y][x] = TARGET;
            }
        }
    }

    // 在新位置放置玩家 / Place player at new position
    if (isTarget(playerX_, playerY_)) {
        grid_[playerY_][playerX_] = PLAYER_ON_TARGET;
    } else {
        grid_[playerY_][playerX_] = PLAYER;
    }
}

bool Level::checkWin() const {
    return boxesOnTarget_ == totalBoxes_;
}

int Level::getBoxesOnTarget() const {
    int count = 0;
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            if (grid_[y][x] == BOX_ON_TARGET) {
                count++;
            }
        }
    }
    return count;
}

void Level::reset() {
    loadLevel(currentLevel_);
}