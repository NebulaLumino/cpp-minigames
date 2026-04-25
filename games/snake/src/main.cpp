/**
 * =============================================================================
 * @file main.cpp
 * @brief Snake game entry point / 贪吃蛇游戏入口点
 * =============================================================================
 *
 * 经典的贪吃蛇游戏，使用ncurses库实现。
 * Classic Snake game using ncurses library.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 *
 * @控制说明 Controls:
 * - ← → ↑ ↓ : 移动方向 / Move direction
 * - Q : 退出 / Quit
 */

#include "game.h"

int main() {
    SnakeGame game;
    game.run();
    return 0;
}
