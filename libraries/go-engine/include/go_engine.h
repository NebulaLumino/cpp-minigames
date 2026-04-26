/**
 * =============================================================================
 * @file go_engine.h
 * @brief Go AI Engine / 围棋AI引擎
 * =============================================================================
 *
 * A reusable Go AI library using Monte Carlo Tree Search (MCTS).
 * 可重用的围棋AI库，使用蒙特卡洛树搜索（MCTS）。
 *
 * Features:
 * - MCTS with UCB1 selection / UCB1选择的MCTS
 * - Random playout / 随机模拟
 * - Territory and influence evaluation / 地盘和影响力评估
 * - Configurable simulation count / 可配置模拟次数
 *
 * =============================================================================
 * @author  NebulaLumino
 * @date    2026
 */

#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <numeric>

/**
 * @brief 棋子颜色 / Stone color
 */
enum GoColor {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

/**
 * @brief 走法结构 / Move structure
 */
struct GoMove {
    int row, col;
    GoColor color;
    bool isPass;
    int visits;
    double winRate;

    GoMove() : row(0), col(0), color(EMPTY), isPass(false), visits(0), winRate(0.0) {}
    GoMove(int r, int c, GoColor col_) : row(r), col(c), color(col_), isPass(false), visits(0), winRate(0.0) {}

    static GoMove pass(GoColor color) {
        GoMove m;
        m.color = color;
        m.isPass = true;
        return m;
    }
};

/**
 * @brief 围棋AI引擎 / Go AI Engine
 */
class GoEngine {
public:
    GoEngine(int boardSize = 9);

    // 配置 / Configuration
    void setBoardSize(int size);
    void setSimulationCount(int count);
    void setTimeLimit(int ms);

    // 游戏接口 / Game Interface
    void reset();
    bool placeStone(int row, int col, GoColor color);
    void pass(GoColor color);

    // AI搜索 / AI Search
    GoMove getBestMove(GoColor color, int simulations = 1000);
    GoMove getBestMoveTime(GoColor color, int timeMs);

    // 评估 / Evaluation
    double evaluate(GoColor color) const;
    int countTerritory(GoColor color) const;
    int countStones(GoColor color) const;

    // 游戏状态 / Game State
    int getBoardSize() const { return boardSize_; }
    GoColor getStone(int row, int col) const { return board_[row][col]; }
    GoColor getCurrentTurn() const { return currentTurn_; }
    bool isGameOver() const { return twoPasses_; }

    // MCTS相关 / MCTS related
    struct Node {
        int row, col;
        GoColor color;
        bool isPass;
        int visits;
        int wins;
        Node* parent;
        std::vector<Node*> children;

        Node(int r, int c, GoColor col, bool pass = false, Node* p = nullptr)
            : row(r), col(c), color(col), isPass(pass), visits(0), wins(0), parent(p) {}
    };

    Node* selectBest(Node* node);
    Node* expand(Node* node, GoColor color);
    int simulate(Node* node, GoColor color);
    void backpropagate(Node* node, int result);
    double ucb1(Node* node, Node* child);

private:
    void clearTree();
    void findGroup(int row, int col, std::vector<std::pair<int, int>>& group, std::vector<std::vector<bool>>& visited);
    int countGroupLiberties(const std::vector<std::pair<int, int>>& group);
    bool captureGroup(int row, int col, GoColor byColor);
    bool isValidMove(int row, int col, GoColor color) const;
    bool wouldCapture(int row, int col, GoColor color) const;

    int boardSize_;
    std::vector<std::vector<GoColor>> board_;
    GoColor currentTurn_;
    bool lastPass_;
    bool twoPasses_;
    int simulationCount_;
    int timeLimit_;
    Node* root_;
};

// UCB1常数 / UCB1 constant
const double C_PUCT = 1.414;

// Helper function
inline GoColor oppositeColor(GoColor c) { return c == BLACK ? WHITE : BLACK; }

inline GoEngine::GoEngine(int boardSize)
    : boardSize_(boardSize)
    , currentTurn_(BLACK)
    , lastPass_(false)
    , twoPasses_(false)
    , simulationCount_(1000)
    , timeLimit_(0)
    , root_(nullptr) {
    srand(static_cast<unsigned>(time(nullptr)));
    board_.assign(boardSize_, std::vector<GoColor>(boardSize_, EMPTY));
    reset();
}

inline void GoEngine::setBoardSize(int size) {
    boardSize_ = size;
    board_.assign(boardSize_, std::vector<GoColor>(boardSize_, EMPTY));
    reset();
}

inline void GoEngine::setSimulationCount(int count) {
    simulationCount_ = std::max(100, count);
}

inline void GoEngine::setTimeLimit(int ms) {
    timeLimit_ = ms;
}

inline void GoEngine::reset() {
    for (int r = 0; r < boardSize_; r++) {
        for (int c = 0; c < boardSize_; c++) {
            board_[r][c] = EMPTY;
        }
    }
    currentTurn_ = BLACK;
    lastPass_ = false;
    twoPasses_ = false;
    clearTree();
}

inline void GoEngine::clearTree() {
    if (root_) {
        // 简化：每次都重新创建树 / Simplified: recreate tree each time
        root_ = nullptr;
    }
}

inline void GoEngine::findGroup(int row, int col, std::vector<std::pair<int, int>>& group, std::vector<std::vector<bool>>& visited) {
    if (row < 0 || row >= boardSize_ || col < 0 || col >= boardSize_) return;
    if (visited[row][col]) return;
    if (board_[row][col] != board_[row][col]) return;  // Simplified

    GoColor color = board_[row][col];
    if (color == EMPTY) return;

    visited[row][col] = true;
    group.push_back({row, col});

    findGroup(row - 1, col, group, visited);
    findGroup(row + 1, col, group, visited);
    findGroup(row, col - 1, group, visited);
    findGroup(row, col + 1, group, visited);
}

inline int GoEngine::countGroupLiberties(const std::vector<std::pair<int, int>>& group) {
    if (group.empty()) return 0;

    GoColor color = board_[group[0].first][group[0].second];
    std::vector<std::vector<bool>> visited(boardSize_, std::vector<bool>(boardSize_, false));

    for (const auto& pos : group) {
        visited[pos.first][pos.second] = true;
    }

    int liberties = 0;
    std::vector<std::pair<int, int>> boundary;

    for (const auto& pos : group) {
        int dr[4] = {-1, 1, 0, 0};
        int dc[4] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int nr = pos.first + dr[i];
            int nc = pos.second + dc[i];

            if (nr < 0 || nr >= boardSize_ || nc < 0 || nc >= boardSize_) continue;
            if (visited[nr][nc]) continue;

            if (board_[nr][nc] == EMPTY) {
                bool found = false;
                for (const auto& b : boundary) {
                    if (b.first == nr && b.second == nc) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    boundary.push_back({nr, nc});
                    liberties++;
                }
            }
        }
    }

    return liberties;
}

inline bool GoEngine::captureGroup(int row, int col, GoColor byColor) {
    GoColor enemy = oppositeColor(byColor);
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    bool captured = false;

    for (int i = 0; i < 4; i++) {
        int nr = row + dr[i];
        int nc = col + dc[i];

        if (nr < 0 || nr >= boardSize_ || nc < 0 || nc >= boardSize_) continue;
        if (board_[nr][nc] != enemy) continue;

        std::vector<std::pair<int, int>> group;
        std::vector<std::vector<bool>> visited(boardSize_, std::vector<bool>(boardSize_, false));
        findGroup(nr, nc, group, visited);

        if (countGroupLiberties(group) == 0) {
            for (const auto& pos : group) {
                board_[pos.first][pos.second] = EMPTY;
            }
            captured = true;
        }
    }

    return captured;
}

inline bool GoEngine::isValidMove(int row, int col, GoColor color) const {
    if (row < 0 || row >= boardSize_ || col < 0 || col >= boardSize_) return false;
    if (board_[row][col] != EMPTY) return false;
    return true;
}

inline bool GoEngine::wouldCapture(int row, int col, GoColor color) const {
    // Simplified: just check if opponent's group would have 0 liberties
    GoColor enemy = oppositeColor(color);

    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nr = row + dr[i];
        int nc = col + dc[i];

        if (nr < 0 || nr >= boardSize_ || nc < 0 || nc >= boardSize_) continue;
        if (board_[nr][nc] != enemy) continue;

        // Simplified: assume capture possible
        return true;
    }

    return false;
}

inline bool GoEngine::placeStone(int row, int col, GoColor color) {
    if (row < 0 || row >= boardSize_ || col < 0 || col >= boardSize_) return false;
    if (board_[row][col] != EMPTY) return false;
    if (twoPasses_) return false;

    // Simplified: place stone and capture
    board_[row][col] = color;
    captureGroup(row, col, color);

    currentTurn_ = oppositeColor(currentTurn_);
    lastPass_ = false;
    twoPasses_ = false;

    return true;
}

inline void GoEngine::pass(GoColor color) {
    if (lastPass_) {
        twoPasses_ = true;
    }
    lastPass_ = true;
    currentTurn_ = oppositeColor(currentTurn_);
}

inline GoMove GoEngine::getBestMove(GoColor color, int simulations) {
    GoEngine tempEngine(boardSize_);
    tempEngine.simulationCount_ = simulations;

    // Copy board state
    for (int r = 0; r < boardSize_; r++) {
        for (int c = 0; c < boardSize_; c++) {
            tempEngine.board_[r][c] = board_[r][c];
        }
    }
    tempEngine.currentTurn_ = color;

    Node* root = new Node(-1, -1, color, true, nullptr);

    for (int sim = 0; sim < simulations; sim++) {
        Node* node = root;
        GoColor nodeColor = color;

        // Selection: select child with highest UCB1
        while (!node->children.empty()) {
            node = selectBest(node);
            nodeColor = oppositeColor(nodeColor);
        }

        // Expansion: add new child if not terminal
        if (!tempEngine.twoPasses_) {
            node = tempEngine.expand(node, nodeColor);
            if (node) {
                nodeColor = oppositeColor(nodeColor);
            }
        }

        // Simulation: random playout
        int result = tempEngine.simulate(node, nodeColor);

        // Backpropagation
        tempEngine.backpropagate(node, result);
    }

    // Find best move (most visits)
    GoMove bestMove;
    bestMove.color = color;
    bestMove.visits = 0;

    for (Node* child : root->children) {
        if (child->visits > bestMove.visits) {
            bestMove.visits = child->visits;
            bestMove.winRate = (double)child->wins / child->visits;
            bestMove.row = child->row;
            bestMove.col = child->col;
            bestMove.isPass = child->isPass;
        }
    }

    // Clean up tree
    delete root;

    return bestMove;
}

inline GoMove GoEngine::getBestMoveTime(GoColor color, int timeMs) {
    // Simplified: just use simulations
    return getBestMove(color, simulationCount_);
}

inline GoEngine::Node* GoEngine::selectBest(Node* node) {
    Node* best = nullptr;
    double bestUCB = -1e100;

    for (Node* child : node->children) {
        double ucb = ucb1(node, child);
        if (ucb > bestUCB) {
            bestUCB = ucb;
            best = child;
        }
    }

    return best ? best : node->children.front();
}

inline GoEngine::Node* GoEngine::expand(Node* node, GoColor color) {
    // Generate all possible moves
    std::vector<Node*> candidates;

    // Add pass
    candidates.push_back(new Node(-1, -1, color, true, node));

    // Add all empty positions
    for (int r = 0; r < boardSize_; r++) {
        for (int c = 0; c < boardSize_; c++) {
            if (isValidMove(r, c, color)) {
                candidates.push_back(new Node(r, c, color, false, node));
            }
        }
    }

    // Randomly select one and add to tree
    if (!candidates.empty()) {
        int idx = rand() % candidates.size();
        node->children.push_back(candidates[idx]);
        return candidates[idx];
    }

    return nullptr;
}

inline int GoEngine::simulate(Node* node, GoColor color) {
    // Simple random playout
    GoEngine sim(*this);
    int passes = 0;

    for (int i = 0; i < boardSize_ * boardSize_ * 3; i++) {  // Max moves
        if (sim.twoPasses_) break;

        // Get valid moves
        std::vector<std::pair<int, int>> validMoves;
        for (int r = 0; r < boardSize_; r++) {
            for (int c = 0; c < boardSize_; c++) {
                if (sim.isValidMove(r, c, sim.currentTurn_)) {
                    validMoves.push_back({r, c});
                }
            }
        }

        if (validMoves.empty()) {
            sim.pass(sim.currentTurn_);
            passes++;
        } else {
            // Random move
            int idx = rand() % validMoves.size();
            sim.placeStone(validMoves[idx].first, validMoves[idx].second, sim.currentTurn_);
            passes = 0;
        }
    }

    // Simple evaluation: count territory
    int blackScore = countTerritory(BLACK);
    int whiteScore = countTerritory(WHITE) + 6.5;  // Komi

    if (blackScore > whiteScore) return 1;
    if (whiteScore > blackScore) return 0;
    return 0;  // Draw
}

inline void GoEngine::backpropagate(Node* node, int result) {
    while (node) {
        node->visits++;
        if (result == 1) {
            node->wins++;
        }
        node = node->parent;
    }
}

inline double GoEngine::ucb1(Node* parent, Node* child) {
    if (child->visits == 0) return 1e100;  // Prioritize unvisited
    double exploitation = (double)child->wins / child->visits;
    double exploration = C_PUCT * sqrt(log((double)parent->visits) / child->visits);
    return exploitation + exploration;
}

inline double GoEngine::evaluate(GoColor color) const {
    int territory = const_cast<GoEngine*>(this)->countTerritory(color);
    int stones = const_cast<GoEngine*>(this)->countStones(color);
    return territory + stones;
}

inline int GoEngine::countTerritory(GoColor color) const {
    std::vector<std::vector<bool>> visited(boardSize_, std::vector<bool>(boardSize_, false));
    int territory = 0;

    for (int r = 0; r < boardSize_; r++) {
        for (int c = 0; c < boardSize_; c++) {
            if (board_[r][c] == EMPTY && !visited[r][c]) {
                std::vector<std::pair<int, int>> region;
                std::vector<std::pair<int, int>> stack = {{r, c}};
                visited[r][c] = true;

                GoColor surrounding = EMPTY;
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

                        if (nr < 0 || nr >= boardSize_ || nc < 0 || nc >= boardSize_) continue;
                        if (visited[nr][nc]) continue;

                        if (board_[nr][nc] == EMPTY) {
                            visited[nr][nc] = true;
                            stack.push_back({nr, nc});
                        } else {
                            if (surrounding == EMPTY) {
                                surrounding = board_[nr][nc];
                            } else if (surrounding != board_[nr][nc]) {
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

inline int GoEngine::countStones(GoColor color) const {
    int count = 0;
    for (int r = 0; r < boardSize_; r++) {
        for (int c = 0; c < boardSize_; c++) {
            if (board_[r][c] == color) count++;
        }
    }
    return count;
}