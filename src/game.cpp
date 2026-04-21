/**
 * =============================================================================
 * @file game.cpp
 * @brief 游戏主逻辑实现 / Game Main Logic Implementation
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include "game.h"
#include <thread>
#include <chrono>

Game::Game()
    : state_(GameState::PLAYING)
    , score_(0)
    , level_(1)
    , dropTimer_(0.0f)
    , dropInterval_(0.5f)
    , lockTimer_(0.0f)
    , lockDelay_(0.5f)
    , isLocking_(false) {
    board_.clear();
    currentPiece_ = PieceManager::createRandomPiece();
    nextPiece_ = PieceManager::createRandomPiece();
}

void Game::run() {
    while (renderer_.isOpen()) {
        renderer_.processEvents();

        if (renderer_.shouldClose()) break;

        // 处理暂停切换 / Handle pause toggle
        if (renderer_.getPause()) {
            if (state_ == GameState::PLAYING) {
                state_ = GameState::PAUSED;
            } else if (state_ == GameState::PAUSED) {
                state_ = GameState::PLAYING;
            }
        }

        // 游戏结束时显示画面并等待重启 / Show game over screen and wait for restart
        if (state_ == GameState::GAME_OVER) {
            // 创建一个空方块来隐藏它（避免glitch）
            // Create an empty piece to hide it (to avoid glitch)
            Piece emptyPiece;
            emptyPiece.type = currentPiece_.type;  // 保持颜色以便UI一致 / Keep color for UI consistency
            emptyPiece.x = -10;  // 移出屏幕 / Move off screen
            emptyPiece.y = -10;
            emptyPiece.rotation = 0;

            renderer_.draw(board_, emptyPiece, nextPiece_, score_, level_);

            // 在棋盘中央显示GAME OVER
            // Draw GAME OVER on top of board
            int centerX = (renderer_.getMaxX() - 11) / 2;
            int centerY = (renderer_.getMaxY() - 20) / 2 + 10;
            renderer_.drawGameOver(centerX, centerY);

            if (renderer_.getHardDrop()) {
                // 重置游戏 / Reset game
                board_.clear();
                score_ = 0;
                level_ = 1;
                dropInterval_ = 0.5f;
                dropTimer_ = 0.0f;
                lockTimer_ = 0.0f;
                isLocking_ = false;
                currentPiece_ = PieceManager::createRandomPiece();
                nextPiece_ = PieceManager::createRandomPiece();
                state_ = GameState::PLAYING;
            }
            renderer_.resetInputs();
            continue;
        }

        // 暂停时不处理输入和更新 / Skip input and update when paused
        if (state_ == GameState::PAUSED) {
            renderer_.draw(board_, currentPiece_, nextPiece_, score_, level_);
            renderer_.resetInputs();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            continue;
        }

        handleInput();
        update(0.016f);  // ~60 FPS
        renderer_.draw(board_, currentPiece_, nextPiece_, score_, level_);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Game::handleInput() {
    // 左右移动 / Left/Right movement
    if (renderer_.getMoveLeft() && Collision::canMove(board_, currentPiece_, -1, 0)) {
        currentPiece_.x--;
        if (isLocking_) lockTimer_ = 0.0f;  // 成功移动重置锁定计时器
    }
    if (renderer_.getMoveRight() && Collision::canMove(board_, currentPiece_, 1, 0)) {
        currentPiece_.x++;
        if (isLocking_) lockTimer_ = 0.0f;
    }

    // 加速下落（软降）/ Soft drop
    if (renderer_.getMoveDown() && Collision::canMove(board_, currentPiece_, 0, 1)) {
        currentPiece_.y++;
        score_ += 1;  // 软降每格+1分
        if (isLocking_) lockTimer_ = 0.0f;
    }

    // 旋转（含墙踢）/ Rotation with wall kick
    if (renderer_.getRotate() && Collision::canRotate(board_, currentPiece_)) {
        Piece original = currentPiece_;
        currentPiece_ = Collision::getRotatedPiece(board_, currentPiece_);

        // 检查旋转是否实际成功
        // Check if rotation actually succeeded
        bool positionChanged = (currentPiece_.x != original.x || currentPiece_.y != original.y);
        bool rotationChanged = (currentPiece_.rotation != original.rotation);

        // 如果旋转了但位置没变且新位置无效，恢复原状
        // If rotated but position unchanged and new position invalid, restore
        if (rotationChanged && !positionChanged && Collision::check(board_, currentPiece_)) {
            currentPiece_ = original;
        }

        if (isLocking_) lockTimer_ = 0.0f;
    }

    // 硬降（一落到底）/ Hard drop (instant drop to bottom)
    if (renderer_.getHardDrop()) {
        while (Collision::canMove(board_, currentPiece_, 0, 1)) {
            currentPiece_.y++;
            score_ += 2;  // 硬降每格+2分
        }
        placePiece();
    }
}

void Game::update(float dt) {
    if (state_ != GameState::PLAYING) return;

    dropTimer_ += dt;

    // 检测是否可以继续下落 / Check if piece can continue moving down
    if (Collision::canMove(board_, currentPiece_, 0, 1)) {
        // 可以下落，重置锁定状态 / Can move down, reset lock state
        isLocking_ = false;
        lockTimer_ = 0.0f;

        // 按重力间隔下落 / Gravity drop
        if (dropTimer_ >= dropInterval_) {
            dropTimer_ -= dropInterval_;
            currentPiece_.y++;
        }
    } else {
        // 无法下落，启动锁定延迟 / Can't move down, start lock delay
        if (!isLocking_) {
            isLocking_ = true;
            lockTimer_ = 0.0f;
        }

        lockTimer_ += dt;
        if (lockTimer_ >= lockDelay_) {
            placePiece();
        }
    }

    renderer_.resetInputs();
}

void Game::spawnPiece() {
    currentPiece_ = nextPiece_;
    nextPiece_ = PieceManager::createRandomPiece();
    currentPiece_.x = 3;
    currentPiece_.y = 0;

    // 重置锁定状态 / Reset lock state
    isLocking_ = false;
    lockTimer_ = 0.0f;

    // 检测游戏结束 / Check game over
    if (Collision::check(board_, currentPiece_)) {
        state_ = GameState::GAME_OVER;
    }
}

bool Game::placePiece() {
    auto shape = currentPiece_.getShape();
    board_.lockPiece(static_cast<int>(currentPiece_.type), currentPiece_.rotation,
                    currentPiece_.x, currentPiece_.y, shape);

    // 消除行并计分 / Clear lines and score
    int lines = board_.clearLines();
    score_ += calculateScore(lines);
    level_ = calculateLevel(score_);
    dropInterval_ = std::max(0.1f, 0.5f - (level_ - 1) * 0.05f);

    // 生成新方块 / Spawn new piece
    spawnPiece();
    dropTimer_ = 0.0f;
    return true;
}

// 经典俄罗斯方块计分规则 / Classic Tetris scoring rules
int Game::calculateScore(int lines) const {
    static const int LINE_SCORES[] = {
        0,     // 0行 / 0 lines
        100,   // 1行 / 1 line  (Single)
        300,   // 2行 / 2 lines (Double)
        500,   // 3行 / 3 lines (Triple)
        800    // 4行 / 4 lines (Tetris)
    };
    return LINE_SCORES[std::min(lines, 4)];
}

// 等级计算：每1000分升一级，最高15级
// Level: +1 per 1000 points, max level 15
int Game::calculateLevel(int score) const {
    return std::min(15, score / 1000 + 1);
}
