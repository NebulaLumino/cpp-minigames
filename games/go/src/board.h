/**
 * =============================================================================
 * @file board.h
 * @brief Go board / 围棋棋盘
 * =============================================================================
 *
 * 管理围棋游戏的棋盘状态、棋子放置和提子逻辑。
 * Manages Go board state, stone placement, and capture logic.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include <vector>
#include <string>

/**
 * @brief 棋子颜色 / Stone color
 */
enum StoneColor {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

/**
 * @brief 棋子结构 / Stone structure
 */
struct Stone {
    StoneColor color;
    int liberties;

    Stone() : color(EMPTY), liberties(0) {}
    Stone(StoneColor c) : color(c), liberties(0) {}
};

/**
 * @brief 组结构 / Group structure
 */
struct Group {
    std::vector<std::pair<int, int>> stones;
    StoneColor color;
    int liberties;
};

/**
 * =============================================================================
 * @class GoBoard
 * @brief 围棋棋盘类 / Go Board Class
 * =============================================================================
 */
class GoBoard {
public:
    GoBoard(int size = 9);

    // 重置棋盘 / Reset board
    void reset();

    // 获取棋子颜色 / Get stone color
    StoneColor getStone(int row, int col) const;

    // 放置棋子 / Place stone
    bool placeStone(int row, int col, StoneColor color);

    // 获取气数 / Get liberties
    int getLiberties(int row, int col);

    // 检查是否可以放置 / Check if placement is valid
    bool isValidMove(int row, int col, StoneColor color) const;

    // 获取所有可落子位置 / Get all valid moves
    std::vector<std::pair<int, int>> getValidMoves(StoneColor color) const;

    // 游戏状态 / Game state
    int getBoardSize() const { return size_; }
    StoneColor getCurrentTurn() const { return currentTurn_; }
    int getCapturedBlack() const { return capturedBlack_; }
    int getCapturedWhite() const { return capturedWhite_; }
    bool isGameOver() const { return gameOver_; }

    // 数子 / Count territory
    int countTerritory(StoneColor color) const;
    int countStones(StoneColor color) const;

    // 放弃之手 / Pass
    void pass();
    bool bothPassed() const { return lastPass_ && previousPass_; }

    // 棋盘输出 / Board output
    void printBoard() const;
    std::string toString() const;

private:
    void findGroup(int row, int col, std::vector<std::pair<int, int>>& group, std::vector<std::vector<bool>>& visited);
    int countGroupLiberties(const std::vector<std::pair<int, int>>& group);
    bool captureGroup(int row, int col, StoneColor byColor);
    bool wouldCapture(int row, int col, StoneColor color);
    bool checkKo(int row, int col, StoneColor color) const;

    int size_;
    std::vector<std::vector<Stone>> board_;
    StoneColor currentTurn_;
    int capturedBlack_;
    int capturedWhite_;
    bool gameOver_;
    bool lastPass_;
    bool previousPass_;
};