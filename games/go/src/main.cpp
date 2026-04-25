/**
 * =============================================================================
 * @file main.cpp
 * @brief Go game entry point / 围棋游戏入口
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "board.h"
#include "renderer.h"
#include <iostream>

int main() {
    Renderer renderer;
    GoBoard board(9);

    std::cout << "=== GO (9x9) ===" << std::endl;
    std::cout << "Black plays first (X)" << std::endl;
    std::cout << "Enter coordinates like 'd4' or use arrow keys + Enter" << std::endl;
    std::cout << "Type 'p' to pass, 'q' to quit" << std::endl;
    std::cout << std::endl;

    while (!renderer.shouldClose()) {
        board.printBoard();

        if (board.isGameOver()) {
            int blackScore = board.countStones(BLACK) + board.countTerritory(BLACK);
            int whiteScore = board.countStones(WHITE) + board.countTerritory(WHITE) + 6.5;
            std::cout << "\nGame Over!" << std::endl;
            std::cout << "Black: " << blackScore << ", White: " << whiteScore << std::endl;
            if (blackScore > whiteScore) {
                std::cout << "Black wins!" << std::endl;
            } else {
                std::cout << "White wins!" << std::endl;
            }
            break;
        }

        int row, col;
        bool pass;
        if (renderer.processInput(row, col, pass)) {
            if (pass) {
                board.pass();
                std::cout << (board.getCurrentTurn() == BLACK ? "Black" : "White") << " passed." << std::endl;
            } else {
                if (board.placeStone(row, col, board.getCurrentTurn())) {
                    std::cout << "Stone placed at " << static_cast<char>('a' + col) << board.getBoardSize() - row << std::endl;
                } else {
                    std::cout << "Invalid move!" << std::endl;
                }
            }
        }
    }

    std::cout << "Thanks for playing!" << std::endl;
    return 0;
}