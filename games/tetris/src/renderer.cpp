/**
 * =============================================================================
 * @file renderer.cpp
 * @brief 渲染器实现 / Renderer Implementation
 * =============================================================================
 *
 * 使用ASCII字符绘制游戏画面，支持颜色和终端自适应。
 * Uses ASCII characters to render the game with color support
 * and terminal adaptation.
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#include "renderer.h"
#include <curses.h>

// ============================================================================
// 绘制字符常量 / Drawing Character Constants
// ============================================================================
/** 实心方块（当前/已锁定）/ Filled block (current/locked) */
const char* BLOCK_FILLED = "[]";
/** 空单元格 / Empty cell */
const char* BLOCK_EMPTY = "  ";
/** 幽灵方块（落点预览）/ Ghost block (landing preview) */
const char* BLOCK_GHOST = "{}";
/** 垂直边框线 / Vertical border line */
const char* VERT_LINE = "|";
/** 水平边框线 / Horizontal border line */
const char* HORIZ_LINE = "--";
/** 角落字符 / Corner character */
const char* CORNER = "+";

Renderer::Renderer()
    : isOpen_(true)
    , shouldClose_(false)
    , moveLeft_(false), moveRight_(false)
    , moveDown_(false), rotate_(false), hardDrop_(false)
    , pause_(false)
    , inputChar_(0)
    , maxX_(0), maxY_(0) {

    // ncurses初始化 / ncurses initialization
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);    // 启用功能键 / Enable function keys
    curs_set(0);             // 隐藏光标 / Hide cursor

    // 颜色初始化 / Color initialization
    if (has_colors()) {
        start_color();
        // 颜色对1-7对应方块类型1-7
        // Color pairs 1-7 for piece types 1-7
        init_pair(1, COLOR_CYAN, COLOR_BLACK);     // I - 青色 / Cyan
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);     // O - 黄色 / Yellow
        init_pair(3, COLOR_MAGENTA, COLOR_BLACK);   // T - 紫色 / Magenta
        init_pair(4, COLOR_GREEN, COLOR_BLACK);     // S - 绿色 / Green
        init_pair(5, COLOR_RED, COLOR_BLACK);       // Z - 红色 / Red
        init_pair(6, COLOR_BLUE, COLOR_BLACK);     // J - 蓝色 / Blue
        init_pair(7, COLOR_WHITE, COLOR_BLACK);     // L - 白色 / White
    }

    nodelay(stdscr, TRUE);  // 非阻塞输入 / Non-blocking input
}

Renderer::~Renderer() {
    endwin();
}

void Renderer::draw(const Board& board, const Piece& currentPiece,
                   const Piece& nextPiece, int score, int level) {
    clear();

    // 获取终端尺寸 / Get terminal size
    getmaxyx(stdscr, maxY_, maxX_);
    int maxY = maxY_;
    int maxX = maxX_;

    // 计算棋盘尺寸（每格2字符宽）/ Calculate board dimensions (2 chars per cell)
    int boardWidth = BOARD_WIDTH * 2 + 2;   // +2 for borders
    int boardHeight = BOARD_HEIGHT + 2;       // +2 for top/bottom
    int sidePanelWidth = 12;                  // 侧边栏宽度 / Side panel width

    // 居中计算 / Center calculation
    int startX = (maxX - boardWidth - sidePanelWidth) / 2;
    int startY = (maxY - boardHeight) / 2;

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    // 绘制标题 / Draw title
    mvprintw(startY, (maxX - 13) / 2, "=== TETRIS ===");

    int bx = startX;           // 棋盘起始X / Board start X
    int by = startY + 2;       // 棋盘起始Y / Board start Y

    // ========== 绘制棋盘边框 / Draw board border ==========
    move(by, bx);
    addstr(CORNER);
    for (int x = 0; x < BOARD_WIDTH; x++) {
        addstr(HORIZ_LINE);
    }
    addstr(CORNER);

    // ========== 绘制棋盘行 / Draw board rows ==========
    auto grid = board.getGrid();

    // 计算幽灵方块位置 / Calculate ghost piece position
    int ghostY = Collision::getGhostY(board, currentPiece);

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        move(by + 1 + y, bx);
        addstr(VERT_LINE);

        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (grid[y][x] != 0) {
                // 已锁定的方块 / Locked piece
                attron(COLOR_PAIR(grid[y][x]));
                addstr(BLOCK_FILLED);
                attroff(COLOR_PAIR(grid[y][x]));
            } else {
                // 检查是否为当前方块或幽灵方块 / Check for current piece or ghost
                bool isCurrentPiece = false;
                bool isGhost = false;
                auto shape = currentPiece.getShape();

                for (int py = 0; py < 4; ++py) {
                    for (int px = 0; px < 4; ++px) {
                        if (shape[py][px] != 0) {
                            int pieceX = currentPiece.x + px;
                            int pieceY = currentPiece.y + py;
                            int ghostPieceX = currentPiece.x + px;
                            int ghostPieceY = ghostY + py;

                            if (pieceX == x && pieceY == y) {
                                isCurrentPiece = true;
                            }
                            // 幽灵只在空单元格显示 / Ghost only shows in empty cells
                            if (ghostPieceX == x && ghostPieceY == y && !isCurrentPiece) {
                                isGhost = true;
                            }
                        }
                    }
                }

                if (isCurrentPiece) {
                    attron(COLOR_PAIR(static_cast<int>(currentPiece.type) + 1));
                    addstr(BLOCK_FILLED);
                    attroff(COLOR_PAIR(static_cast<int>(currentPiece.type) + 1));
                } else if (isGhost) {
                    attron(COLOR_PAIR(static_cast<int>(currentPiece.type) + 1));
                    addstr(BLOCK_GHOST);
                    attroff(COLOR_PAIR(static_cast<int>(currentPiece.type) + 1));
                } else {
                    addstr(BLOCK_EMPTY);
                }
            }
        }

        addstr(VERT_LINE);

        // ========== 绘制侧边栏 / Draw side panel ==========
        int sideX = bx + BOARD_WIDTH * 2 + 4;

        if (y == 0) {
            mvprintw(by + 1 + y, sideX, "NEXT:");
        } else if (y == 1) {
            mvprintw(by + 1 + y, sideX, "+--------+");
        } else if (y >= 2 && y <= 5) {
            // 下一个方块预览 / Next piece preview
            auto nextShape = nextPiece.getShape();
            int ny = y - 2;
            move(by + 1 + y, sideX);
            addstr("|");
            for (int px = 0; px < 4; ++px) {
                if (nextShape[ny][px] != 0) {
                    attron(COLOR_PAIR(static_cast<int>(nextPiece.type) + 1));
                    addstr(BLOCK_FILLED);
                    attroff(COLOR_PAIR(static_cast<int>(nextPiece.type) + 1));
                } else {
                    addstr(BLOCK_EMPTY);
                }
            }
            addstr("|");
        } else if (y == 6) {
            mvprintw(by + 1 + y, sideX, "+--------+");
        } else if (y == 8) {
            mvprintw(by + 1 + y, sideX, "SCORE: %d", score);
        } else if (y == 10) {
            mvprintw(by + 1 + y, sideX, "LEVEL: %d", level);
        } else if (y == 13) {
            mvprintw(by + 1 + y, sideX, "---------");
        } else if (y == 15) {
            mvprintw(by + 1 + y, sideX, "< > Move");
        } else if (y == 16) {
            mvprintw(by + 1 + y, sideX, "^   Rotate");
        } else if (y == 17) {
            mvprintw(by + 1 + y, sideX, "v   Drop");
        } else if (y == 18) {
            mvprintw(by + 1 + y, sideX, "P   Pause");
        } else if (y == 19) {
            mvprintw(by + 1 + y, sideX, "Q   Quit");
        }
    }

    // 绘制底部边框 / Draw bottom border
    move(by + BOARD_HEIGHT + 1, bx);
    addstr(CORNER);
    for (int x = 0; x < BOARD_WIDTH; x++) {
        addstr(HORIZ_LINE);
    }
    addstr(CORNER);

    refresh();
}

void Renderer::processEvents() {
    moveLeft_ = moveRight_ = moveDown_ = rotate_ = hardDrop_ = pause_ = false;
    shouldClose_ = false;

    int ch = getch();
    if (ch != ERR) {
        inputChar_ = ch;
        switch (ch) {
            case KEY_LEFT:  moveLeft_ = true; break;
            case KEY_RIGHT: moveRight_ = true; break;
            case KEY_DOWN:  moveDown_ = true; break;
            case KEY_UP:    rotate_ = true; break;
            case ' ':       hardDrop_ = true; break;
            case 'p':
            case 'P':       pause_ = true; break;
            case 'q':
            case 'Q':
            case 27:        shouldClose_ = true; break;  // ESC键 / ESC key
        }
    }
}

void Renderer::resetInputs() {
    moveLeft_ = moveRight_ = moveDown_ = rotate_ = hardDrop_ = pause_ = false;
}

int Renderer::getMaxX() const {
    return maxX_;
}

int Renderer::getMaxY() const {
    return maxY_;
}

void Renderer::drawGameOver(int x, int y) {
    // 保存当前属性 / Save current attributes
    int savedX, savedY;
    getyx(stdscr, savedY, savedX);

    // 绘制半透明背景覆盖游戏区域
    // Draw semi-transparent background over game area
    attron(A_REVERSE);
    mvprintw(y, x, "   GAME OVER   ");
    attroff(A_REVERSE);

    mvprintw(y + 2, x - 3, "Press SPACE to restart");

    // 恢复光标位置 / Restore cursor position
    move(savedY, savedX);
    refresh();
}
