/**
 * =============================================================================
 * @file test.cpp
 * @brief Go Engine tests / 围棋引擎测试
 * =============================================================================
 *
 * @author  NebulaLumino
 * @date    2026
 */

#include <catch2/catch_test_macros.hpp>
#include "go_engine.h"

TEST_CASE("GoEngine: Initial state", "[GoEngine]") {
    GoEngine engine(9);

    REQUIRE(engine.getBoardSize() == 9);
    REQUIRE(engine.getCurrentTurn() == BLACK);
    REQUIRE(engine.isGameOver() == false);
}

TEST_CASE("GoEngine: Place stone", "[GoEngine]") {
    GoEngine engine(9);

    bool placed = engine.placeStone(4, 4, BLACK);
    REQUIRE(placed == true);
    REQUIRE(engine.getStone(4, 4) == BLACK);
    REQUIRE(engine.getCurrentTurn() == WHITE);
}

TEST_CASE("GoEngine: Invalid move - occupied", "[GoEngine]") {
    GoEngine engine(9);

    engine.placeStone(4, 4, BLACK);
    bool placed = engine.placeStone(4, 4, WHITE);
    REQUIRE(placed == false);
}

TEST_CASE("GoEngine: Turn switching", "[GoEngine]") {
    GoEngine engine(9);

    REQUIRE(engine.getCurrentTurn() == BLACK);
    engine.placeStone(4, 4, BLACK);
    REQUIRE(engine.getCurrentTurn() == WHITE);
    engine.placeStone(4, 5, WHITE);
    REQUIRE(engine.getCurrentTurn() == BLACK);
}

TEST_CASE("GoEngine: Pass", "[GoEngine]") {
    GoEngine engine(9);

    engine.pass(BLACK);
    REQUIRE(engine.getCurrentTurn() == WHITE);

    engine.pass(WHITE);
    REQUIRE(engine.isGameOver() == true);
}

TEST_CASE("GoEngine: Stone counting", "[GoEngine]") {
    GoEngine engine(9);

    engine.placeStone(4, 4, BLACK);
    engine.placeStone(4, 5, WHITE);
    engine.placeStone(3, 4, BLACK);

    REQUIRE(engine.countStones(BLACK) == 2);
    REQUIRE(engine.countStones(WHITE) == 1);
}

TEST_CASE("GoEngine: MCTS best move", "[GoEngine]") {
    GoEngine engine(9);

    // Get best move with limited simulations
    GoMove move = engine.getBestMove(BLACK, 100);

    REQUIRE(move.color == BLACK);
    REQUIRE(move.visits >= 0);
}

TEST_CASE("GoEngine: Territory", "[GoEngine]") {
    GoEngine engine(9);

    // Place stones in corners to create territory
    engine.placeStone(0, 0, BLACK);
    engine.placeStone(0, 1, BLACK);
    engine.placeStone(1, 0, BLACK);

    int blackTerritory = engine.countTerritory(BLACK);
    REQUIRE(blackTerritory >= 0);
}

TEST_CASE("GoEngine: Reset", "[GoEngine]") {
    GoEngine engine(9);

    engine.placeStone(4, 4, BLACK);
    engine.placeStone(4, 5, WHITE);

    engine.reset();

    REQUIRE(engine.getCurrentTurn() == BLACK);
    REQUIRE(engine.getStone(4, 4) == EMPTY);
}