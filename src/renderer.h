/**
 * @file renderer.h
 * @brief 渲染器接口 / Renderer Interface
 *
 * 使用ncurses库进行终端渲染。
 * Uses ncurses library for terminal rendering.
 *
 * @author NebulaLumino
 * @date 2026
 */

#pragma once

#include "board.h"
#include "piece.h"
#include "collision.h"

/**
 * @class Renderer
 * @brief 游戏渲染器类 / Game Renderer Class
 *
 * 负责：
 * - ncurses初始化和清理
 * - 游戏画面绘制（棋盘、方块、UI）
 * - 玩家输入事件处理
 * - 终端自适应居中显示
 *
 * Responsible for:
 * - ncurses initialization and cleanup
 * - Game画面 rendering (board, pieces, UI)
 * - Player input event processing
 * - Terminal-adaptive centered display
 */
class Renderer {
public:
    /**
     * @brief 构造函数，初始化ncurses / Constructor, initializes ncurses
     */
    Renderer();

    /**
     * @brief 析构函数，清理ncurses / Destructor, cleans up ncurses
     */
    ~Renderer();

    /**
     * @brief 绘制完整游戏画面 / Draws the complete game画面
     * @param board 棋盘引用 / Board reference
     * @param currentPiece 当前方块 / Current piece
     * @param nextPiece 下一个方块（预览）/ Next piece (preview)
     * @param score 当前得分 / Current score
     * @param level 当前等级 / Current level
     */
    void draw(const Board& board, const Piece& currentPiece,
              const Piece& nextPiece, int score, int level);

    /**
     * @brief 检查渲染器是否打开 / Checks if renderer is open
     * @return 是否打开 / Whether open
     */
    bool isOpen() const { return isOpen_; }

    /**
     * @brief 处理输入事件 / Processes input events
     */
    void processEvents();

    /**
     * @brief 检查是否请求关闭 / Checks if close was requested
     */
    bool shouldClose() const { return shouldClose_; }

    bool getMoveLeft() const { return moveLeft_; }
    bool getMoveRight() const { return moveRight_; }
    bool getMoveDown() const { return moveDown_; }
    bool getRotate() const { return rotate_; }
    bool getHardDrop() const { return hardDrop_; }

    /**
     * @brief 重置输入状态 / Resets input state
     */
    void resetInputs();

private:
    bool isOpen_;                  /**< 渲染器是否打开 / Whether renderer is open */
    bool shouldClose_;            /**< 是否请求关闭 / Whether close was requested */
    bool moveLeft_, moveRight_;    /**< 左右移动输入 / Left/right movement input */
    bool moveDown_;               /**< 软降输入 / Soft drop input */
    bool rotate_;                 /**< 旋转输入 / Rotate input */
    bool hardDrop_;               /**< 硬降输入 / Hard drop input */
    int inputChar_;               /**< 当前输入字符 / Current input character */
};
