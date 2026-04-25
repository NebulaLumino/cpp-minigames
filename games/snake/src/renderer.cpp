/**
 * =============================================================================
 * @file renderer.cpp
 * @brief Snake game renderer implementation / 贪吃蛇游戏渲染器实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "renderer.h"

const char* SNAKE_HEAD = "[]";
const char* SNAKE_BODY = "[]";
const char* FOOD = "oo";
const char* BORDER_CORNER = "+";
const char* BORDER_H = "--";
const char* BORDER_V = "|";

Renderer::Renderer()
    : isOpen_(true)
    , shouldClose_(false)
    , moveLeft_(false), moveRight_(false)
    , moveUp_(false), moveDown_(false)
    , pause_(false), hardDrop_(false)
    , inputChar_(0)
    , maxX_(0), maxY_(0) {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);    // 蛇头 / Snake head
        init_pair(2, COLOR_CYAN, COLOR_BLACK);     // 蛇身 / Snake body
        init_pair(3, COLOR_RED, COLOR_BLACK);      // 食物 / Food
    }

    nodelay(stdscr, TRUE);
}

Renderer::~Renderer() {
    endwin();
}

void Renderer::draw(const std::vector<std::pair<int,int>>& snake, int foodX, int foodY, int score, int level) {
    clear();

    getmaxyx(stdscr, maxY_, maxX_);

    const int boardW = 22;
    const int boardH = 22;
    int startX = (maxX_ - boardW * 2) / 2;
    int startY = (maxY_ - boardH) / 2;

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    // 标题 / Title
    mvprintw(startY, (maxX_ - 13) / 2, "=== SNAKE ===");

    int bx = startX;
    int by = startY + 2;

    // 绘制边框 / Draw border
    move(by, bx);
    addstr(BORDER_CORNER);
    for (int i = 0; i < boardW; i++) addstr(BORDER_H);
    addstr(BORDER_CORNER);

    for (int y = 0; y < boardH; y++) {
        move(by + 1 + y, bx);
        addstr(BORDER_V);

        for (int x = 0; x < boardW; x++) {
            bool isSnakeHead = (snake[0].first == x && snake[0].second == y);
            bool isSnakeBody = false;
            bool isFood = (foodX == x && foodY == y);

            if (!isSnakeHead) {
                for (size_t i = 1; i < snake.size(); i++) {
                    if (snake[i].first == x && snake[i].second == y) {
                        isSnakeBody = true;
                        break;
                    }
                }
            }

            if (isSnakeHead) {
                attron(COLOR_PAIR(1));
                addstr(SNAKE_HEAD);
                attroff(COLOR_PAIR(1));
            } else if (isSnakeBody) {
                attron(COLOR_PAIR(2));
                addstr(SNAKE_BODY);
                attroff(COLOR_PAIR(2));
            } else if (isFood) {
                attron(COLOR_PAIR(3));
                addstr(FOOD);
                attroff(COLOR_PAIR(3));
            } else {
                addstr("  ");
            }
        }

        addstr(BORDER_V);

        // 右侧信息 / Side info
        if (y == 0) mvprintw(by + 1 + y, bx + boardW * 2 + 4, "SNAKE");
        if (y == 2) mvprintw(by + 1 + y, bx + boardW * 2 + 4, "Score: %d", score);
        if (y == 4) mvprintw(by + 1 + y, bx + boardW * 2 + 4, "Level: %d", level);
        if (y == 7) mvprintw(by + 1 + y, bx + boardW * 2 + 4, "-------");
        if (y == 9) mvprintw(by + 1 + y, bx + boardW * 2 + 4, "P   Pause");
        if (y == 11) mvprintw(by + 1 + y, bx + boardW * 2 + 4, "Q   Quit");
    }

    // 底部边框 / Bottom border
    move(by + boardH + 1, bx);
    addstr(BORDER_CORNER);
    for (int i = 0; i < boardW; i++) addstr(BORDER_H);
    addstr(BORDER_CORNER);

    refresh();
}

void Renderer::processEvents() {
    moveLeft_ = moveRight_ = moveUp_ = moveDown_ = hardDrop_ = pause_ = false;
    shouldClose_ = false;

    int ch = getch();
    if (ch != ERR) {
        inputChar_ = ch;
        switch (ch) {
            case KEY_LEFT:  moveLeft_ = true; break;
            case KEY_RIGHT: moveRight_ = true; break;
            case KEY_UP:    moveUp_ = true; break;
            case KEY_DOWN:  moveDown_ = true; break;
            case 'p':
            case 'P':       pause_ = true; break;
            case 'q':
            case 'Q':
            case 27:        shouldClose_ = true; break;
        }
    }
}

void Renderer::resetInputs() {
    moveLeft_ = moveRight_ = moveUp_ = moveDown_ = hardDrop_ = pause_ = false;
}

void Renderer::drawGameOver(int x, int y) {
    int savedX, savedY;
    getyx(stdscr, savedY, savedX);

    attron(A_REVERSE);
    mvprintw(y, x, "  GAME OVER  ");
    attroff(A_REVERSE);

    mvprintw(y + 2, x - 3, "Press SPACE to restart");

    move(savedY, savedX);
    refresh();
}
