/**
 * =============================================================================
 * @file main.cpp
 * @brief Minesweeper game entry point / 扫雷游戏入口
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include <cstring>
#include "board.h"
#include "renderer.h"

void printUsage(const char* progName) {
    printf("Usage: %s [rows] [cols] [mines]\n", progName);
    printf("  Default: 9x9 grid with 10 mines\n");
    printf("  Example: %s 16 16 40 for expert mode\n", progName);
}

int main(int argc, char* argv[]) {
    int rows = 9;
    int cols = 9;
    int mines = 10;

    if (argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        printUsage(argv[0]);
        return 0;
    }

    if (argc >= 4) {
        rows = atoi(argv[1]);
        cols = atoi(argv[2]);
        mines = atoi(argv[3]);

        if (rows < 5 || rows > 30 || cols < 5 || cols > 30) {
            printf("Error: Rows and cols must be between 5 and 30\n");
            return 1;
        }

        if (mines < 1 || mines >= rows * cols) {
            printf("Error: Mines must be between 1 and %d\n", rows * cols - 1);
            return 1;
        }
    }

    Renderer renderer;
    Board board(rows, cols, mines);

    while (!renderer.shouldClose()) {
        renderer.draw(board);

        int row, col;
        char action;
        if (renderer.processInput(row, col, action)) {
            if (action == 'r') {
                // Restart
                board = Board(rows, cols, mines);
            } else if (action == 'f') {
                // Toggle flag
                board.toggleFlag(row, col);
            } else if (action == 'r' && board.isGameOver()) {
                // Reveal after game over for restart
                board.reveal(row, col);
            }
        }

        // Reveal on space/enter if not game over
        if (action == 'r' && !board.isGameOver()) {
            board.reveal(row, col);
        }
    }

    return 0;
}