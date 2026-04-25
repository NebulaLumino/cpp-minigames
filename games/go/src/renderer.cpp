/**
 * =============================================================================
 * @file renderer.cpp
 * @brief Go renderer implementation / 围棋渲染器实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "renderer.h"

Renderer::Renderer()
    : shouldClose_(false)
    , cursorRow_(4)
    , cursorCol_(4) {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_BLACK);   // Black stone
        init_pair(2, COLOR_WHITE, COLOR_BLACK);   // White stone
        init_pair(3, COLOR_CYAN, COLOR_BLACK);    // Cursor
        init_pair(4, COLOR_GREEN, COLOR_BLACK);   // Valid move highlight
    }
}

Renderer::~Renderer() {
    endwin();
}

void Renderer::draw(const GoBoard& board) {
    clear();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // 标题 / Title
    mvprintw(0, (maxX - 8) / 2, "=== GO ===");

    // 计算棋盘起始位置 / Calculate board start position
    int boardSize = board.getBoardSize();
    int startX = (maxX - boardSize * 2) / 2;
    int startY = 2;

    if (startX < 2) startX = 2;

    // 绘制坐标 / Draw coordinates
    mvprintw(startY - 1, startX + 1, "a b c d e f g h i");
    for (int r = 0; r < boardSize; r++) {
        mvprintw(startY + r * 2 + 1, startX - 2, "%d", boardSize - r);
    }

    // 绘制棋盘边框 / Draw board border
    mvprintw(startY, startX, "+");
    for (int c = 0; c < boardSize; c++) {
        printw("-+");
    }

    // 绘制棋盘和棋子 / Draw board and stones
    for (int r = 0; r < boardSize; r++) {
        mvprintw(startY + 1 + r * 2, startX, "|");

        for (int c = 0; c < boardSize; c++) {
            StoneColor stone = board.getStone(r, c);
            bool isCursor = (r == cursorRow_ && c == cursorCol_);

            if (isCursor && stone == EMPTY) {
                attron(A_REVERSE);
                printw(" .|");
                attroff(A_REVERSE);
            } else if (stone == BLACK) {
                attron(COLOR_PAIR(1) | A_BOLD);
                printw(" X|");
                attroff(COLOR_PAIR(1) | A_BOLD);
            } else if (stone == WHITE) {
                attron(COLOR_PAIR(2));
                printw(" O|");
                attroff(COLOR_PAIR(2));
            } else {
                printw("  |");
            }
        }

        mvprintw(startY + 1 + r * 2, startX + boardSize * 2 + 2, "|");
    }

    // 底部边框 / Bottom border
    mvprintw(startY + boardSize * 2 + 1, startX, "+");
    for (int c = 0; c < boardSize; c++) {
        printw("-+");
    }

    // 右侧信息 / Side info
    int infoX = startX + boardSize * 2 + 6;
    StoneColor turn = board.getCurrentTurn();
    mvprintw(startY, infoX, "Turn: %s", turn == BLACK ? "Black (X)" : "White (O)");
    mvprintw(startY + 2, infoX, "Captured:");
    mvprintw(startY + 3, infoX, "  Black: %d", board.getCapturedBlack());
    mvprintw(startY + 4, infoX, "  White: %d", board.getCapturedWhite());

    // 游戏结束 / Game over
    if (board.isGameOver()) {
        attron(A_REVERSE);
        mvprintw(startY + boardSize * 2 + 3, startX, "=== GAME OVER ===");
        attroff(A_REVERSE);

        // 简单计算胜负 / Simple winner calculation
        int blackScore = board.countStones(BLACK) + board.countTerritory(BLACK);
        int whiteScore = board.countStones(WHITE) + board.countTerritory(WHITE) + 6.5;  // 贴目
        mvprintw(startY + boardSize * 2 + 5, startX, "Black: %d, White: %d", blackScore, whiteScore);
        if (blackScore > whiteScore) {
            mvprintw(startY + boardSize * 2 + 7, startX, "Black wins!");
        } else {
            mvprintw(startY + boardSize * 2 + 7, startX, "White wins!");
        }
    }

    // 控制说明 / Controls
    mvprintw(maxY - 4, 0, "Arrow keys: Move cursor | Space/Enter: Place stone | P: Pass | Q: Quit");

    refresh();
}

bool Renderer::processInput(int& row, int& col, bool& pass) {
    pass = false;
    int ch = getch();

    switch (ch) {
        case KEY_UP:
            if (cursorRow_ > 0) cursorRow_--;
            break;
        case KEY_DOWN:
            if (cursorRow_ < 8) cursorRow_++;
            break;
        case KEY_LEFT:
            if (cursorCol_ > 0) cursorCol_--;
            break;
        case KEY_RIGHT:
            if (cursorCol_ < 8) cursorCol_++;
            break;
        case ' ':
        case '\n':
            row = cursorRow_;
            col = cursorCol_;
            return true;
        case 'p':
        case 'P':
            pass = true;
            return true;
        case 'q':
        case 'Q':
            shouldClose_ = true;
            return false;
        default:
            break;
    }

    return false;
}