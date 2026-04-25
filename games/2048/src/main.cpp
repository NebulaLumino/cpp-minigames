/**
 * =============================================================================
 * @file main.cpp
 * @brief 2048 game entry point / 2048游戏入口
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "board.h"
#include "renderer.h"

int main() {
    Renderer renderer;
    GameBoard board;

    while (!renderer.shouldClose()) {
        renderer.draw(board);

        char input = renderer.processInput();

        if (input == 'q') {
            break;
        } else if (input == 'r') {
            board.reset();
        } else if (input != 0) {
            bool moved = false;
            switch (input) {
                case 'u': moved = board.moveUp(); break;
                case 'd': moved = board.moveDown(); break;
                case 'l': moved = board.moveLeft(); break;
                case 'r': moved = board.moveRight(); break;
            }

            // 如果游戏结束且用户想继续（赢了之后按N）暂不处理
            // For now, just let them keep playing after winning
        }
    }

    return 0;
}