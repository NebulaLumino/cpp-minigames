/**
 * =============================================================================
 * @file main.cpp
 * @brief Chess game entry point / 国际象棋游戏入口
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
    ChessBoard board;

    std::cout << "=== CHESS ===" << std::endl;
    std::cout << "Enter moves in algebraic notation (e.g., e2e4)" << std::endl;
    std::cout << "Type 'q' to quit, 'r' to restart" << std::endl;
    std::cout << std::endl;

    while (!renderer.shouldClose()) {
        board.printBoard();

        if (board.isCheckmate()) {
            std::cout << "CHECKMATE! ";
            std::cout << (board.getWinner() == WHITE ? "White" : "Black") << " wins!" << std::endl;
            break;
        } else if (board.isStalemate()) {
            std::cout << "STALEMATE! Game is a draw." << std::endl;
            break;
        } else if (board.isCheck()) {
            std::cout << "CHECK!" << std::endl;
        }

        std::cout << (board.getCurrentTurn() == WHITE ? "White" : "Black") << "'s turn: ";
        std::cout.flush();

        std::string moveStr;
        renderer.processInput(moveStr);

        if (renderer.shouldClose()) {
            break;
        }

        if (moveStr == "q" || moveStr == "Q") {
            break;
        }

        if (moveStr == "r" || moveStr == "R") {
            board.reset();
            continue;
        }

        if (moveStr.length() >= 4) {
            if (board.makeMove(moveStr)) {
                std::cout << "Move: " << moveStr << std::endl;
            } else {
                std::cout << "Invalid move!" << std::endl;
            }
        }

        std::cout << std::endl;
    }

    std::cout << "Thanks for playing!" << std::endl;
    return 0;
}