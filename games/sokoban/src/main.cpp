/**
 * =============================================================================
 * @file main.cpp
 * @brief Sokoban game entry point / 推箱子游戏入口
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "level.h"
#include "renderer.h"

int main() {
    Renderer renderer;
    Level level;

    // 加载第一关 / Load first level
    level.loadLevel(0);

    while (!renderer.shouldClose()) {
        renderer.draw(level);

        char input = renderer.processInput();

        if (input == 'q') {
            break;
        } else if (input == 'r') {
            level.reset();
        } else if (input == 'n' && level.checkWin()) {
            // 进入下一关 / Go to next level
            int nextLevel = level.getCurrentLevel() + 1;
            if (nextLevel < level.getTotalLevels()) {
                level.loadLevel(nextLevel);
            }
        } else if (input != 0) {
            // 处理移动 / Handle movement
            int dx = 0, dy = 0;
            switch (input) {
                case 'u': dy = -1; break;
                case 'd': dy = 1; break;
                case 'l': dx = -1; break;
                case 'r': dx = 1; break;
            }
            level.movePlayer(dx, dy);
        }
    }

    return 0;
}