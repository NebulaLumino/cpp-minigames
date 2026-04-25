/**
 * =============================================================================
 * @file renderer.cpp
 * @brief Chess renderer implementation / 国际象棋渲染器实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "renderer.h"
#include <cstring>

const char* WHITE_KING = " K ";
const char* WHITE_QUEEN = " Q ";
const char* WHITE_ROOK = " R ";
const char* WHITE_BISHOP = " B ";
const char* WHITE_KNIGHT = " N ";
const char* WHITE_PAWN = " P ";
const char* BLACK_KING = " k ";
const char* BLACK_QUEEN = " q ";
const char* BLACK_ROOK = " r ";
const char* BLACK_BISHOP = " b ";
const char* BLACK_KNIGHT = " n ";
const char* BLACK_PAWN = " p ";
const char* EMPTY_CELL = " . ";

Renderer::Renderer()
    : shouldClose_(false)
    , cursorRow_(6)
    , cursorCol_(4)
    , inputMode_(false) {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);   // 白方棋子 / White pieces
        init_pair(2, COLOR_RED, COLOR_BLACK);    // 黑方棋子 / Black pieces
        init_pair(3, COLOR_CYAN, COLOR_BLACK);   // 高亮选中 / Highlight
        init_pair(4, COLOR_YELLOW, COLOR_BLACK); // 可移动位置 / Valid moves
    }
}

Renderer::~Renderer() {
    endwin();
}

void Renderer::draw(const ChessBoard& board) {
    clear();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // 标题 / Title
    mvprintw(0, (maxX - 14) / 2, "=== CHESS ===");

    // 计算棋盘起始位置 / Calculate board start position
    int startX = (maxX - 8 * 4) / 2;
    int startY = 2;

    if (startX < 2) startX = 2;

    // 绘制棋盘 / Draw board
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece piece = board.getPiece(r, c);
            int drawX = startX + c * 4;
            int drawY = startY + r * 2;

            bool isSelected = (r == cursorRow_ && c == cursorCol_);

            // 棋盘格背景色 / Board cell background
            if (isSelected) {
                attron(A_REVERSE);
            }

            // 深色或浅色格 / Dark or light square
            bool isDark = (r + c) % 2 == 1;
            if (isDark) {
                attron(COLOR_PAIR(2));
            }

            mvprintw(drawY, drawX, "|%s", EMPTY_CELL);
            mvprintw(drawY + 1, drawX, "|   ");

            if (isDark) {
                attroff(COLOR_PAIR(2));
            }
            if (isSelected) {
                attroff(A_REVERSE);
            }

            // 绘制棋子 / Draw piece
            if (!piece.isEmpty()) {
                int colorPair = piece.isWhite() ? COLOR_PAIR(1) : COLOR_PAIR(2);
                attron(colorPair | A_BOLD);

                const char* pieceStr = EMPTY_CELL;
                switch (piece.type) {
                    case KING:   pieceStr = piece.isWhite() ? WHITE_KING : BLACK_KING; break;
                    case QUEEN:  pieceStr = piece.isWhite() ? WHITE_QUEEN : BLACK_QUEEN; break;
                    case ROOK:   pieceStr = piece.isWhite() ? WHITE_ROOK : BLACK_ROOK; break;
                    case BISHOP: pieceStr = piece.isWhite() ? WHITE_BISHOP : BLACK_BISHOP; break;
                    case KNIGHT: pieceStr = piece.isWhite() ? WHITE_KNIGHT : BLACK_KNIGHT; break;
                    case PAWN:   pieceStr = piece.isWhite() ? WHITE_PAWN : BLACK_PAWN; break;
                    default:     pieceStr = EMPTY_CELL; break;
                }

                mvprintw(drawY, drawX + 1, "%s", pieceStr);
                attroff(colorPair | A_BOLD);
            }

            mvprintw(drawY, drawX + 4, "|");
            mvprintw(drawY + 1, drawX + 4, "|");
        }
    }

    // 右侧信息 / Side info
    int infoX = startX + 8 * 4 + 4;
    PieceColor turn = board.getCurrentTurn();
    mvprintw(startY, infoX, "Turn: %s", turn == WHITE ? "White" : "Black");
    mvprintw(startY + 2, infoX, "White pieces:");
    mvprintw(startY + 4, infoX, "King, Queen");
    mvprintw(startY + 5, infoX, "Rook, Bishop");
    mvprintw(startY + 6, infoX, "Knight, Pawn");

    // 游戏状态 / Game state
    if (board.isCheckmate()) {
        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(startY + 16, startX, "*** CHECKMATE! ***");
        attroff(COLOR_PAIR(2) | A_BOLD);
        PieceColor winner = board.getWinner();
        mvprintw(startY + 18, startX, "%s wins!", winner == WHITE ? "White" : "Black");
    } else if (board.isStalemate()) {
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(startY + 16, startX, "*** STALEMATE ***");
        attroff(COLOR_PAIR(1) | A_BOLD);
        mvprintw(startY + 18, startX, "Game is a draw!");
    } else if (board.isCheck()) {
        attron(COLOR_PAIR(3));
        mvprintw(startY + 16, startX, "CHECK!");
        attroff(COLOR_PAIR(3));
    }

    // 控制说明 / Controls
    mvprintw(maxY - 5, 0, "Move: e2e4 or click pieces | Arrow keys: Navigate | Q: Quit");
    mvprintw(maxY - 4, 0, "Promotion: q/r/b/n for Queen/Rook/Bishop/Knight");
    mvprintw(maxY - 3, 0, "Note: Full mouse support not available, use keyboard");

    refresh();
}

bool Renderer::processInput(std::string& moveStr) {
    int ch = getch();

    switch (ch) {
        case KEY_UP:
            if (cursorRow_ > 0) cursorRow_--;
            break;
        case KEY_DOWN:
            if (cursorRow_ < 7) cursorRow_++;
            break;
        case KEY_LEFT:
            if (cursorCol_ > 0) cursorCol_--;
            break;
        case KEY_RIGHT:
            if (cursorCol_ < 7) cursorCol_++;
            break;
        case 'q':
        case 'Q':
            shouldClose_ = true;
            return false;
        case '\n':
        case ' ':
            // 切换输入模式（简化实现）/ Toggle input mode (simplified)
            inputMode_ = !inputMode_;
            break;
        default:
            if (inputMode_ && ch != ERR) {
                if (ch == '\n' || ch == ' ') {
                    inputMode_ = false;
                    return !inputBuffer_.empty();
                } else if (isprint(ch)) {
                    inputBuffer_ += static_cast<char>(ch);
                }
            }
            break;
    }

    // 构建移动字符串 / Build move string
    moveStr = inputBuffer_;
    return false;
}