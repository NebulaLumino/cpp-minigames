/**
 * =============================================================================
 * @file renderer.cpp
 * @brief 2048 renderer implementation / 2048渲染器实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "renderer.h"
#include <cstring>

Renderer::Renderer()
    : shouldClose_(false) {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);     // 2
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);    // 4
        init_pair(3, COLOR_GREEN, COLOR_BLACK);     // 8
        init_pair(4, COLOR_CYAN, COLOR_BLACK);       // 16
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);    // 32
        init_pair(6, COLOR_RED, COLOR_BLACK);        // 64
        init_pair(7, COLOR_BLUE, COLOR_BLACK);      // 128
        init_pair(8, COLOR_GREEN, COLOR_BLACK);      // 256
        init_pair(9, COLOR_CYAN, COLOR_BLACK);       // 512
        init_pair(10, COLOR_YELLOW, COLOR_BLACK);    // 1024
        init_pair(11, COLOR_RED, COLOR_BLACK);       // 2048
    }
}

Renderer::~Renderer() {
    endwin();
}

int Renderer::getColorForValue(int value) {
    switch (value) {
        case 2:    return COLOR_PAIR(1);
        case 4:    return COLOR_PAIR(2);
        case 8:    return COLOR_PAIR(3);
        case 16:   return COLOR_PAIR(4);
        case 32:   return COLOR_PAIR(5);
        case 64:   return COLOR_PAIR(6);
        case 128:  return COLOR_PAIR(7);
        case 256:  return COLOR_PAIR(8);
        case 512:  return COLOR_PAIR(9);
        case 1024: return COLOR_PAIR(10);
        case 2048: return COLOR_PAIR(11);
        default:   return A_NORMAL;
    }
}

void Renderer::draw(const GameBoard& board) {
    clear();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // 标题 / Title
    mvprintw(0, (maxX - 6) / 2, "2048");

    // 计算棋盘起始位置 / Calculate board start position
    int boardWidth = 4 * 6;  // 4 cells * 6 chars each
    int startX = (maxX - boardWidth) / 2;
    int startY = 2;

    if (startX < 2) startX = 2;

    // 绘制棋盘 / Draw board
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            int value = board.getCell(r, c);
            int drawX = startX + c * 6;
            int drawY = startY + r * 3;

            // 绘制单元格背景 / Draw cell background
            mvprintw(drawY, drawX, "+----+");

            // 绘制数字 / Draw number
            char buf[8];
            if (value == 0) {
                strcpy(buf, "     ");
            } else {
                snprintf(buf, sizeof(buf), "%5d", value);
            }

            int color = getColorForValue(value);
            attron(color);
            mvprintw(drawY + 1, drawX, "|%s|", buf);
            attroff(color);

            mvprintw(drawY + 2, drawX, "+----+");
        }
    }

    // 右侧信息 / Side info
    int infoX = startX + boardWidth + 4;
    mvprintw(startY, infoX, "Score: %d", board.getScore());
    mvprintw(startY + 2, infoX, "Best: %d", board.getBestScore());
    mvprintw(startY + 4, infoX, "Highest: %d", board.getHighestTile());

    // 游戏状态 / Game state
    if (board.hasWon()) {
        attron(COLOR_PAIR(3));
        mvprintw(startY + 14, startX, "*** YOU WIN! ***");
        attroff(COLOR_PAIR(3));
        mvprintw(startY + 16, startX, "Press N to continue or R to restart");
    } else if (board.isGameOver()) {
        attron(COLOR_PAIR(6));
        mvprintw(startY + 14, startX, "*** GAME OVER ***");
        attroff(COLOR_PAIR(6));
        mvprintw(startY + 16, startX, "Press R to restart");
    }

    // 控制说明 / Controls
    mvprintw(maxY - 3, 0, "Arrow keys: Slide tiles | N: Continue (if won) | R: Restart | Q: Quit");

    refresh();
}

char Renderer::processInput() {
    int ch = getch();

    switch (ch) {
        case KEY_UP:
            return 'u';
        case KEY_DOWN:
            return 'd';
        case KEY_LEFT:
            return 'l';
        case KEY_RIGHT:
            return 'r';
        case 'n':
        case 'N':
            return 'n';
        case 'r':
        case 'R':
            return 'r';
        case 'q':
        case 'Q':
            shouldClose_ = true;
            return 'q';
        default:
            return 0;
    }
}