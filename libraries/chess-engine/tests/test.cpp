/**
 * =============================================================================
 * @file test.cpp
 * @brief Chess Engine tests / 国际象棋引擎测试
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include <catch2/catch_test_macros.hpp>
#include "chess_engine.h"

TEST_CASE("ChessEngine: Initial position", "[ChessEngine]") {
    ChessEngine engine;
    engine.reset();

    REQUIRE(engine.getSideToMove() == WHITE);
    REQUIRE(engine.isGameOver() == false);
}

TEST_CASE("ChessEngine: Generate moves", "[ChessEngine]") {
    ChessEngine engine;
    engine.reset();

    auto whiteMoves = engine.generateAllMoves(WHITE);
    REQUIRE(whiteMoves.size() > 0);

    // 白方走后轮到黑方 / After white moves, black's turn
    engine.makeMove(whiteMoves[0]);
    REQUIRE(engine.getSideToMove() == BLACK);

    auto blackMoves = engine.generateAllMoves(BLACK);
    REQUIRE(blackMoves.size() > 0);
}

TEST_CASE("ChessEngine: Make move", "[ChessEngine]") {
    ChessEngine engine;
    engine.reset();

    auto moves = engine.generateAllMoves(WHITE);

    // 找到e4走法 / Find e4 move
    ChessMove e4;
    for (const auto& m : moves) {
        if (m.fromRow == 6 && m.fromCol == 4 && m.toRow == 4 && m.toCol == 4) {
            e4 = m;
            break;
        }
    }

    if (e4.fromRow != 0 || e4.fromCol != 0) {  // 如果找到了 / If found
        bool result = engine.makeMove(e4);
        REQUIRE(result == true);
        REQUIRE(engine.getSideToMove() == BLACK);
    }
}

TEST_CASE("ChessEngine: Evaluation", "[ChessEngine]") {
    ChessEngine engine;
    engine.reset();

    int eval = engine.evaluate();
    // 初始位置评估应该接近0 / Initial position evaluation should be close to 0
    bool evalInRange = (eval > -100) && (eval < 100);
    REQUIRE(evalInRange);
}

TEST_CASE("ChessEngine: Legal move detection", "[ChessEngine]") {
    ChessEngine engine;
    engine.reset();

    auto moves = engine.generateAllMoves(WHITE);

    // 所有生成的走法都应该是合法的 / All generated moves should be legal
    for (const auto& m : moves) {
        REQUIRE(engine.isLegalMove(m) == true);
    }
}

TEST_CASE("ChessEngine: Check detection", "[ChessEngine]") {
    ChessEngine engine;
    engine.reset();

    // 初始位置白方没有被将军 / White not in check initially
    REQUIRE(engine.isInCheck(WHITE) == false);
    REQUIRE(engine.isInCheck(BLACK) == false);
}

TEST_CASE("ChessEngine: Best move search", "[ChessEngine]") {
    ChessEngine engine;
    engine.reset();

    // 搜索最佳走法 / Search for best move
    ChessMove bestMove = engine.getBestMove(2);

    // 应该有返回走法 / Should return a move
    REQUIRE(bestMove.fromRow >= 0);
    REQUIRE(bestMove.fromRow < 8);
}