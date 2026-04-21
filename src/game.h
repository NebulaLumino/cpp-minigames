/**
 * =============================================================================
 * @file game.h
 * @brief 游戏主逻辑类 / Game Main Logic Class
 * =============================================================================
 *
 * 管理游戏状态、主循环、输入处理和计分系统。
 * Manages game state, main loop, input handling, and scoring system.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include "board.h"
#include "piece.h"
#include "collision.h"
#include "renderer.h"

/**
 * =============================================================================
 * @enum GameState
 * @brief 游戏状态枚举 / Game State Enumeration
 * =============================================================================
 */
enum class GameState {
    PLAYING,    /**< 游戏进行中 / Game in progress */
    PAUSED,     /**< 游戏暂停 / Game paused */
    GAME_OVER   /**< 游戏结束 / Game over */
};

/**
 * =============================================================================
 * @class Game
 * @brief 游戏主类 / Main Game Class
 * =============================================================================
 *
 * 负责：
 * - 游戏主循环和状态管理
 * - 方块生成和放置
 * - 重力系统和锁定延迟机制
 * - 输入处理和碰撞检测
 * - 计分和等级系统
 *
 * Responsible for:
 * - Main game loop and state management
 * - Piece spawning and placement
 * - Gravity system and lock delay mechanism
 * - Input handling and collision detection
 * - Scoring and level system
 */
class Game {
public:
    /**
     * @brief 构造函数，初始化新游戏
     * @brief Constructor, initializes new game
     */
    Game();

    /**
     * @brief 运行游戏主循环
     * @brief Runs the main game loop
     */
    void run();

private:
    /**
     * @brief 根据时间更新游戏状态
     * @brief Updates game state based on time
     *
     * @param dt 帧时间（秒）/ Frame time in seconds
     */
    void update(float dt);

    /**
     * @brief 处理玩家输入
     * @brief Handles player input
     */
    void handleInput();

    /**
     * @brief 生成新方块
     * @brief Spawns a new piece
     */
    void spawnPiece();

    /**
     * @brief 放置当前方块到棋盘
     * @brief Places current piece on the board
     *
     * @return 是否成功放置 / Whether placement succeeded
     */
    bool placePiece();

    /**
     * @brief 根据消除行数计算得分
     * @brief Calculates score based on lines cleared
     *
     * @param lines 消除的行数 / Number of lines cleared
     * @return 获得的分数 / Score earned
     */
    int calculateScore(int lines) const;

    /**
     * @brief 根据总得分计算等级
     * @brief Calculates level based on total score
     *
     * @param score 总得分 / Total score
     * @return 当前等级 / Current level
     */
    int calculateLevel(int score) const;

    Board board_;                 /**< 游戏棋盘 / Game board */
    Piece currentPiece_;          /**< 当前下落的方块 / Current falling piece */
    Piece nextPiece_;             /**< 下一个方块（预览）/ Next piece (preview) */
    Renderer renderer_;           /**< 渲染器 / Renderer */
    GameState state_;            /**< 当前游戏状态 / Current game state */
    int score_;                   /**< 当前得分 / Current score */
    int level_;                   /**< 当前等级 / Current level */
    float dropTimer_;             /**< 下落计时器（秒）/ Drop timer in seconds */
    float dropInterval_;          /**< 下落间隔（秒）/ Drop interval in seconds */
    float lockTimer_;             /**< 锁定计时器（秒）/ Lock timer in seconds */
    float lockDelay_;             /**< 锁定延迟（秒）/ Lock delay in seconds */
    bool isLocking_;              /**< 是否处于锁定延迟状态 / Whether in lock delay state */
};
