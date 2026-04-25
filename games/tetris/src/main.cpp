/**
 * =============================================================================
 * @file main.cpp
 * @brief 俄罗斯方块游戏入口点 / Tetris Game Entry Point
 * =============================================================================
 *
 * 使用ncurses库实现的终端俄罗斯方块游戏。
 * A terminal-based Tetris game using ncurses library.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 *
 * @编译说明 Build Requirements:
 * - C++17 或更高版本 / C++17 or later
 * - ncurses 库 / ncurses library
 *
 * @控制说明 Controls:
 * - ← → : 左右移动 / Move left/right
 * - ↑   : 旋转 / Rotate
 * - ↓   : 软降（加速下落）/ Soft drop (faster fall)
 * - 空格 : 硬降（一落到底）/ Hard drop (instant drop)
 * - P   : 暂停/继续 / Pause/Resume
 * - Q   : 退出 / Quit
 */

#include "game.h"

/**
 * @brief 程序入口点
 * @brief Program entry point
 * @return 退出码 / Exit code
 */
int main() {
    Game game;
    game.run();
    return 0;
}
