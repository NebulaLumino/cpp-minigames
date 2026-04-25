/**
 * =============================================================================
 * @file renderer.cpp
 * @brief Minesweeper renderer implementation / 扫雷游戏渲染器实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "renderer.h"
#include "board.h"
#include <cstring>

Renderer::Renderer()
    : shouldClose_(false)
    , selectedRow_(0)
    , selectedCol_(0) {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);     // 地雷 / Mine
        init_pair(2, COLOR_GREEN, COLOR_BLACK);  // 数字 / Numbers
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);  // 旗子 / Flag
        init_pair(4, COLOR_CYAN, COLOR_BLACK);   // 隐藏格子 / Hidden
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK); // 选中 / Selected
    }
}

Renderer::~Renderer() {
    endwin();
}

void Renderer::draw(const Board& board) {
    clear();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // 标题 / Title
    mvprintw(0, (maxX - 12) / 2, "=== MINESWEEPER ===");

    // 计算棋盘起始位置 / Calculate board start position
    int boardWidth = board.getCols() * 4;
    int boardHeight = board.getRows();
    int startX = (maxX - boardWidth) / 2;
    int startY = 2;

    if (startX < 2) startX = 2;

    // 绘制边框 / Draw border
    mvprintw(startY, startX, "+");
    for (int c = 0; c < board.getCols(); c++) {
        printw("---+");
    }

    for (int r = 0; r < board.getRows(); r++) {
        mvprintw(startY + 1 + r, startX, "|");
        for (int c = 0; c < board.getCols(); c++) {
            bool isSelected = (r == selectedRow_ && c == selectedCol_);
            char cellChar = ' ';

            if (board.isRevealed(r, c)) {
                if (board.isMine(r, c)) {
                    cellChar = '*';
                } else {
                    int adj = board.getAdjacentMines(r, c);
                    cellChar = adj == 0 ? ' ' : ('0' + adj);
                }
            } else if (board.isFlagged(r, c)) {
                cellChar = 'F';
            } else {
                cellChar = '#';
            }

            attron(isSelected ? A_REVERSE : A_NORMAL);
            printw(" %c |", cellChar);
            attroff(isSelected ? A_REVERSE : A_NORMAL);
        }

        mvprintw(startY + 1 + r, startX + board.getCols() * 4 + 2, "|");
    }

    // 底部边框 / Bottom border
    mvprintw(startY + board.getRows() + 1, startX, "+");
    for (int c = 0; c < board.getCols(); c++) {
        printw("---+");
    }

    // 右侧信息 / Side info
    int infoX = startX + board.getCols() * 4 + 6;
    mvprintw(startY + 1, infoX, "Mines: %d", board.getTotalMines());
    mvprintw(startY + 3, infoX, "Flags: %d", board.getFlagCount());
    mvprintw(startY + 5, infoX, "Rows: %d", board.getRows());
    mvprintw(startY + 7, infoX, "Cols: %d", board.getCols());

    // 游戏状态 / Game state
    if (board.isGameOver()) {
        if (board.isWin()) {
            attron(COLOR_PAIR(2));
            mvprintw(startY + board.getRows() + 3, startX, "*** YOU WIN! ***");
            attroff(COLOR_PAIR(2));
        } else {
            attron(COLOR_PAIR(1));
            mvprintw(startY + board.getRows() + 3, startX, "*** GAME OVER ***");
            attroff(COLOR_PAIR(1));
        }
        mvprintw(startY + board.getRows() + 5, startX, "Press R to restart, Q to quit");
    }

    // 控制说明 / Controls
    mvprintw(maxY - 3, 0, "Arrow keys: Move | Space/Enter: Reveal | F: Flag | Q: Quit | R: Restart");

    refresh();
}

bool Renderer::processInput(int& row, int& col, char& action) {
    action = 0;
    int ch = getch();

    switch (ch) {
        case KEY_UP:
            if (selectedRow_ > 0) selectedRow_--;
            break;
        case KEY_DOWN:
            selectedRow_++;
            break;
        case KEY_LEFT:
            if (selectedCol_ > 0) selectedCol_--;
            break;
        case KEY_RIGHT:
            selectedCol_++;
            break;
        case ' ':
        case '\n':
            row = selectedRow_;
            col = selectedCol_;
            action = 'r';  // reveal
            break;
        case 'f':
        case 'F':
            row = selectedRow_;
            col = selectedCol_;
            action = 'f';  // flag
            break;
        case 'q':
        case 'Q':
            shouldClose_ = true;
            break;
        case 'r':
        case 'R':
            action = 'r';  // restart
            break;
        default:
            break;
    }

    row = selectedRow_;
    col = selectedCol_;
    return action != 0;
}

void Renderer::showGameOver(bool won) {
    mvprintw(LINES / 2, COLS / 2 - 10, won ? "YOU WIN!" : "GAME OVER!");
    refresh();
}

void Renderer::showMessage(const char* msg) {
    mvprintw(LINES / 2, COLS / 2 - strlen(msg) / 2, "%s", msg);
    refresh();
}