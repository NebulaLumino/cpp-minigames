/**
 * =============================================================================
 * @file board.cpp
 * @brief Go board implementation / 围棋棋盘实现
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "board.h"
#include <iostream>
#include <sstream>
#include <algorithm>

GoBoard::GoBoard(int size)
    : size_(size)
    , currentTurn_(BLACK)
    , capturedBlack_(0)
    , capturedWhite_(0)
    , gameOver_(false)
    , lastPass_(false)
    , previousPass_(false) {

    board_.assign(size_, std::vector<Stone>(size_, Stone()));
    reset();
}

void GoBoard::reset() {
    for (int r = 0; r < size_; r++) {
        for (int c = 0; c < size_; c++) {
            board_[r][c] = Stone();
        }
    }
    currentTurn_ = BLACK;
    capturedBlack_ = 0;
    capturedWhite_ = 0;
    gameOver_ = false;
    lastPass_ = false;
    previousPass_ = false;
}

StoneColor GoBoard::getStone(int row, int col) const {
    if (row < 0 || row >= size_ || col < 0 || col >= size_) {
        return EMPTY;
    }
    return board_[row][col].color;
}

void GoBoard::findGroup(int row, int col, std::vector<std::pair<int, int>>& group, std::vector<std::vector<bool>>& visited) {
    if (row < 0 || row >= size_ || col < 0 || col >= size_) {
        return;
    }

    if (visited[row][col]) {
        return;
    }

    StoneColor color = board_[row][col].color;
    if (color == EMPTY) {
        return;
    }

    visited[row][col] = true;
    group.push_back({row, col});

    // 上下左右 / Up, Down, Left, Right
    findGroup(row - 1, col, group, visited);
    findGroup(row + 1, col, group, visited);
    findGroup(row, col - 1, group, visited);
    findGroup(row, col + 1, group, visited);
}

int GoBoard::countGroupLiberties(const std::vector<std::pair<int, int>>& group) {
    if (group.empty()) {
        return 0;
    }

    StoneColor color = board_[group[0].first][group[0].second].color;
    std::vector<std::vector<bool>> visited(size_, std::vector<bool>(size_, false));

    for (const auto& pos : group) {
        visited[pos.first][pos.second] = true;
    }

    int liberties = 0;
    std::vector<std::pair<int, int>> boundary;

    for (const auto& pos : group) {
        int r = pos.first;
        int c = pos.second;

        // 检查四个方向 / Check four directions
        int dr[4] = {-1, 1, 0, 0};
        int dc[4] ={0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr < 0 || nr >= size_ || nc < 0 || nc >= size_) {
                continue;
            }

            if (visited[nr][nc]) {
                continue;
            }

            if (board_[nr][nc].color == EMPTY) {
                bool isNew = true;
                for (const auto& b : boundary) {
                    if (b.first == nr && b.second == nc) {
                        isNew = false;
                        break;
                    }
                }
                if (isNew) {
                    boundary.push_back({nr, nc});
                    liberties++;
                }
            }
        }
    }

    return liberties;
}

bool GoBoard::captureGroup(int row, int col, StoneColor byColor) {
    StoneColor enemy = (byColor == BLACK) ? WHITE : BLACK;

    // 找到所有与(row, col)相邻的敌方组 / Find all adjacent enemy groups
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nr = row + dr[i];
        int nc = col + dc[i];

        if (nr < 0 || nr >= size_ || nc < 0 || nc >= size_) {
            continue;
        }

        if (board_[nr][nc].color != enemy) {
            continue;
        }

        // 检查这个组是否已被提走 / Check if group already captured
        std::vector<std::pair<int, int>> group;
        std::vector<std::vector<bool>> visited(size_, std::vector<bool>(size_, false));
        findGroup(nr, nc, group, visited);

        // 计算气数 / Count liberties
        int liberties = countGroupLiberties(group);

        if (liberties == 0) {
            // 提走这个组 / Capture this group
            for (const auto& pos : group) {
                if (board_[pos.first][pos.second].color == BLACK) {
                    capturedBlack_++;
                } else {
                    capturedWhite_++;
                }
                board_[pos.first][pos.second].color = EMPTY;
            }
            return true;
        }
    }

    return false;
}

bool GoBoard::wouldCapture(int row, int col, StoneColor color) {
    // 临时放置棋子 / Temporarily place stone
    board_[row][col].color = color;

    bool captured = captureGroup(row, col, color);

    // 移除临时棋子 / Remove temporary stone
    board_[row][col].color = EMPTY;

    return captured;
}

bool GoBoard::checkKo(int row, int col, StoneColor color) const {
    // 简化：只检查这个位置是否刚被提走 / Simplified: just check if this position was just captured
    // 对于完整的KO检测需要更复杂的逻辑
    // For full KO detection, more complex logic is needed

    StoneColor enemy = (color == BLACK) ? WHITE : BLACK;

    // 检查四个方向的敌人组 / Check enemy groups in four directions
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nr = row + dr[i];
        int nc = col + dc[i];

        if (nr < 0 || nr >= size_ || nc < 0 || nc >= size_) {
            continue;
        }

        if (board_[nr][nc].color != enemy) {
            continue;
        }

        // 检查这个组是否只有一气 / Check if group has only one liberty
        std::vector<std::pair<int, int>> group;
        std::vector<std::vector<bool>> visited(size_, std::vector<bool>(size_, false));

        // 临时标记 / Temporary marking
        Stone savedStone = board_[nr][nc];

        // 计算气数 - 简化版本 / Count liberties - simplified version
        int liberties = 0;
        std::vector<std::pair<int, int>> checked;

        for (int d2 = 0; d2 < 4; d2++) {
            int r2 = nr + dr[d2];
            int c2 = nc + dc[d2];

            if (r2 < 0 || r2 >= size_ || c2 < 0 || c2 >= size_) {
                continue;
            }

            if (board_[r2][c2].color == EMPTY) {
                bool found = false;
                for (const auto& p : checked) {
                    if (p.first == r2 && p.second == c2) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    checked.push_back({r2, c2});
                    liberties++;
                }
            }
        }

        if (liberties == 1) {
            // 可能形成KO / May form KO
            return true;
        }
    }

    return false;
}

int GoBoard::getLiberties(int row, int col) {
    if (row < 0 || row >= size_ || col < 0 || col >= size_) {
        return 0;
    }

    if (board_[row][col].color == EMPTY) {
        return 0;
    }

    std::vector<std::pair<int, int>> group;
    std::vector<std::vector<bool>> visited(size_, std::vector<bool>(size_, false));
    findGroup(row, col, group, visited);

    return countGroupLiberties(group);
}

bool GoBoard::isValidMove(int row, int col, StoneColor color) const {
    if (row < 0 || row >= size_ || col < 0 || col >= size_) {
        return false;
    }

    if (board_[row][col].color != EMPTY) {
        return false;
    }

    // 检查是否是自己吃自己 / Check if this is self-capture
    // 需要临时放置并检查 / Need to temporarily place and check

    return true;
}

std::vector<std::pair<int, int>> GoBoard::getValidMoves(StoneColor color) const {
    std::vector<std::pair<int, int>> validMoves;

    for (int r = 0; r < size_; r++) {
        for (int c = 0; c < size_; c++) {
            if (board_[r][c].color == EMPTY) {
                // 简化：所有空位都是有效的（真实实现需要更复杂的检查）
                // Simplified: all empty positions are valid (real implementation needs more complex checks)
                validMoves.push_back({r, c});
            }
        }
    }

    return validMoves;
}

bool GoBoard::placeStone(int row, int col, StoneColor color) {
    if (row < 0 || row >= size_ || col < 0 || col >= size_) {
        return false;
    }

    if (board_[row][col].color != EMPTY) {
        return false;
    }

    if (color != currentTurn_) {
        return false;
    }

    // 放置棋子 / Place stone
    board_[row][col].color = color;

    // 提走相邻的敌方组 / Capture adjacent enemy groups
    bool captured = captureGroup(row, col, color);

    // 检查自己是否被吃（无气）/ Check if we are now dead (no liberties)
    std::vector<std::pair<int, int>> ourGroup;
    std::vector<std::vector<bool>> visited(size_, std::vector<bool>(size_, false));
    findGroup(row, col, ourGroup, visited);

    if (countGroupLiberties(ourGroup) == 0 && !captured) {
        // 自己吃不了别人，自己也没气，不能落子 / Can't capture enemy, have no liberties, can't place
        board_[row][col].color = EMPTY;
        return false;
    }

    // 更新回合 / Switch turn
    currentTurn_ = (currentTurn_ == BLACK) ? WHITE : BLACK;
    lastPass_ = false;

    return true;
}

int GoBoard::countTerritory(StoneColor color) const {
    std::vector<std::vector<bool>> visited(size_, std::vector<bool>(size_, false));
    int territory = 0;

    for (int r = 0; r < size_; r++) {
        for (int c = 0; c < size_; c++) {
            if (board_[r][c].color == EMPTY && !visited[r][c]) {
                // 找到一块空地 / Find an empty region
                std::vector<std::pair<int, int>> region;
                std::vector<std::pair<int, int>> stack = {{r, c}};
                visited[r][c] = true;

                StoneColor surrounding = EMPTY;
                bool isTerritory = true;

                while (!stack.empty()) {
                    auto pos = stack.back();
                    stack.pop_back();
                    region.push_back(pos);

                    int dr[4] = {-1, 1, 0, 0};
                    int dc[4] = {0, 0, -1, 1};

                    for (int i = 0; i < 4; i++) {
                        int nr = pos.first + dr[i];
                        int nc = pos.second + dc[i];

                        if (nr < 0 || nr >= size_ || nc < 0 || nc >= size_) {
                            continue;
                        }

                        if (visited[nr][nc]) {
                            continue;
                        }

                        StoneColor stoneColor = board_[nr][nc].color;

                        if (stoneColor == EMPTY) {
                            visited[nr][nc] = true;
                            stack.push_back({nr, nc});
                        } else {
                            if (surrounding == EMPTY) {
                                surrounding = stoneColor;
                            } else if (surrounding != stoneColor) {
                                // 被两种颜色包围，不是任何一方的地 / Surrounded by both colors, not either player's territory
                                isTerritory = false;
                            }
                        }
                    }
                }

                if (isTerritory && surrounding == color) {
                    territory += region.size();
                }
            }
        }
    }

    return territory;
}

int GoBoard::countStones(StoneColor color) const {
    int count = 0;
    for (int r = 0; r < size_; r++) {
        for (int c = 0; c < size_; c++) {
            if (board_[r][c].color == color) {
                count++;
            }
        }
    }
    return count;
}

void GoBoard::pass() {
    previousPass_ = lastPass_;
    lastPass_ = true;

    currentTurn_ = (currentTurn_ == BLACK) ? WHITE : BLACK;

    if (bothPassed()) {
        gameOver_ = true;
    }
}

void GoBoard::printBoard() const {
    std::cout << "\n   ";
    for (int c = 0; c < size_; c++) {
        std::cout << static_cast<char>('a' + c) << " ";
    }
    std::cout << "\n  +";
    for (int c = 0; c < size_; c++) {
        std::cout << "--+";
    }
    std::cout << "\n";

    for (int r = 0; r < size_; r++) {
        std::cout << size_ - r << " |";
        for (int c = 0; c < size_; c++) {
            StoneColor stone = board_[r][c].color;
            if (stone == BLACK) {
                std::cout << " X|";
            } else if (stone == WHITE) {
                std::cout << " O|";
            } else {
                std::cout << "  |";
            }
        }
        std::cout << "\n  +";
        for (int c = 0; c < size_; c++) {
            std::cout << "--+";
        }
        std::cout << "\n";
    }

    std::cout << "\nCurrent turn: " << (currentTurn_ == BLACK ? "Black" : "White") << "\n";
    std::cout << "Captured - Black: " << capturedBlack_ << ", White: " << capturedWhite_ << "\n";
}

std::string GoBoard::toString() const {
    std::ostringstream oss;
    oss << "   ";
    for (int c = 0; c < size_; c++) {
        oss << static_cast<char>('a' + c) << " ";
    }
    oss << "\n  +";
    for (int c = 0; c < size_; c++) {
        oss << "--+";
    }
    oss << "\n";

    for (int r = 0; r < size_; r++) {
        oss << size_ - r << " |";
        for (int c = 0; c < size_; c++) {
            StoneColor stone = board_[r][c].color;
            if (stone == BLACK) {
                oss << " X|";
            } else if (stone == WHITE) {
                oss << " O|";
            } else {
                oss << "  |";
            }
        }
        oss << "\n  +";
        for (int c = 0; c < size_; c++) {
            oss << "--+";
        }
        oss << "\n";
    }

    return oss.str();
}