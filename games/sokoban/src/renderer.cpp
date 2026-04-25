/**
 * =============================================================================
 * @file renderer.cpp
 * @brief Sokoban renderer implementation / 推箱子渲染器实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "renderer.h"

Renderer::Renderer()
    : shouldClose_(false) {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_YELLOW, COLOR_BLACK);   // 箱子 / Box
        init_pair(2, COLOR_GREEN, COLOR_BLACK);    // 目标 / Target
        init_pair(3, COLOR_CYAN, COLOR_BLACK);     // 箱子在目标上 / Box on target
        init_pair(4, COLOR_MAGENTA, COLOR_BLACK);   // 玩家 / Player
        init_pair(5, COLOR_WHITE, COLOR_BLACK);     // 墙 / Wall
    }
}

Renderer::~Renderer() {
    endwin();
}

void Renderer::draw(const Level& level) {
    clear();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // 标题 / Title
    mvprintw(0, (maxX - 12) / 2, "=== SOKOBAN ===");

    // 计算棋盘起始位置 / Calculate board start position
    int boardWidth = level.getWidth() * 2;
    int boardHeight = level.getHeight();
    int startX = (maxX - boardWidth) / 2;
    int startY = 2;

    if (startX < 2) startX = 2;

    // 绘制关卡 / Draw level
    for (int y = 0; y < level.getHeight(); y++) {
        for (int x = 0; x < level.getWidth(); x++) {
            Cell cell = level.getCell(x, y);
            int drawX = startX + x * 2;
            int drawY = startY + y;

            char ch = ' ';
            int colorPair = 0;

            switch (cell) {
                case WALL:
                    ch = '#';
                    colorPair = COLOR_PAIR(5);
                    break;
                case BOX:
                    ch = '[';
                    colorPair = COLOR_PAIR(1);
                    break;
                case TARGET:
                    ch = '.';
                    colorPair = COLOR_PAIR(2);
                    break;
                case BOX_ON_TARGET:
                    ch = '*';
                    colorPair = COLOR_PAIR(3);
                    break;
                case PLAYER:
                    ch = '@';
                    colorPair = COLOR_PAIR(4);
                    break;
                case PLAYER_ON_TARGET:
                    ch = '@';
                    colorPair = COLOR_PAIR(4);
                    break;
                default:
                    ch = ' ';
                    break;
            }

            if (colorPair) {
                attron(colorPair);
            }
            mvprintw(drawY, drawX, "%c ", ch);
            if (colorPair) {
                attroff(colorPair);
            }
        }
    }

    // 右侧信息 / Side info
    int infoX = startX + boardWidth + 4;
    mvprintw(startY, infoX, "Level: %d/%d", level.getCurrentLevel() + 1, level.getTotalLevels());
    mvprintw(startY + 2, infoX, "Boxes: %d/%d", level.getBoxesOnTarget(), level.getTotalBoxes());

    // 游戏胜利 / Game win
    if (level.checkWin()) {
        attron(COLOR_PAIR(2));
        mvprintw(startY + level.getHeight() + 2, startX, "*** LEVEL COMPLETE! ***");
        attroff(COLOR_PAIR(2));
        mvprintw(startY + level.getHeight() + 4, startX, "Press N for next level, R to restart");
    }

    // 控制说明 / Controls
    mvprintw(maxY - 3, 0, "Arrow keys: Move | N: Next level | R: Restart | Q: Quit");

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