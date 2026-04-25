# Chess / 国际象棋

> A classic two-player board game built with C++ and ncurses.
> 一个经典的双人棋盘游戏，使用 C++ 和 ncurses 构建。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

## Overview / 概览

A complete chess implementation with full rule support including castling, en passant, pawn promotion, check, checkmate, and stalemate detection.

一款完整的国际象棋实现，支持所有规则包括王车易位、吃过路兵、兵升变将军、将死和逼和检测。

---

## Features / 功能特性

- **Full Chess Rules / 完整象棋规则**: All standard moves and special moves
- **Castling / 王车易位**: Both kingside and queenside
- **En Passant / 吃过路兵**: Special pawn capture
- **Pawn Promotion / 兵升变**: Promote to Queen, Rook, Bishop, or Knight
- **Check/Checkmate/Stalemate / 将军/将死/逼和**: Complete game state detection
- **Move Notation / 移动记谱法**: Standard algebraic notation (e.g., "e2e4")

---

## Building / 构建

```bash
# Create build directory / 创建构建目录
mkdir -p build && cd build

# Configure and build / 配置并构建
cmake ..

# Build the game / 构建游戏
make chess

# Run the game / 运行游戏
./chess

# Run tests / 运行测试
make chess_test && ./chess_test
```

---

## Controls / 操作说明

| Key | Action |
|-----|--------|
| e2e4 | Move piece from e2 to e4 / 从e2移动到e4 |
| R | Restart game / 重新开始 |
| Q | Quit / 退出 |

---

## Game Rules / 游戏规则

### Basic Moves / 基本移动
- **King (王)**: Moves one square in any direction
- **Queen (后)**: Moves any number of squares horizontally, vertically, or diagonally
- **Rook (车)**: Moves any number of squares horizontally or vertically
- **Bishop (象)**: Moves any number of squares diagonally
- **Knight (马)**: Moves in an L-shape (2 squares + 1 square perpendicular)
- **Pawn (兵)**: Moves forward one square, captures diagonally

### Special Moves / 特殊移动
- **Castling (王车易位)**: King moves two squares toward rook, rook jumps over king
- **En Passant (吃过路兵)**: Pawn can capture enemy pawn that just moved two squares
- **Pawn Promotion (兵升变)**: Pawn reaching opposite end becomes Queen/Rook/Bishop/Knight

### Win Conditions / 胜利条件
- **Checkmate (将死)**: King is in check with no legal moves
- **Stalemate (逼和)**: No legal moves but king is not in check
- **Resignation (认输)**: Player resigns

---

## Architecture / 架构

```
games/chess/
├── src/
│   ├── main.cpp      # Entry point / 入口点
│   ├── board.h       # Board class declaration / 棋盘类声明
│   ├── board.cpp     # Board logic / 棋盘逻辑
│   ├── renderer.h    # Renderer declaration / 渲染器声明
│   └── renderer.cpp  # ncurses rendering / ncurses渲染
├── tests/
│   └── BoardTest.cpp # Unit tests / 单元测试
└── CMakeLists.txt    # Build configuration / 构建配置
```

### Class Structure / 类结构

**ChessBoard**: Manages chess game state
- `board_[8][8]`: 8x8 grid of pieces
- `getLegalMoves()`: Get all legal moves for a piece
- `makeMove()`: Execute a move in algebraic notation
- `isCheck()`, `isCheckmate()`, `isStalemate()`: Game state detection

**Piece Types**: KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN

**Renderer**: ncurses-based display
- `draw()`: Renders chess board and UI
- `processInput()`: Handles keyboard input

---

## Testing / 测试

Tests use [Catch2](https://github.com/catchorg/Catch2) v3.4.0:

```bash
cd build && cmake .. && make chess_test && ctest --output-on-failure
```

---

## Project Structure / 项目结构

This game is part of the [cpp-minigames](../) mono-repo. See the root README for full project list.

本游戏是 [cpp-minigames](../) 单仓库项目的一部分。查看根目录README获取完整项目列表。

---

## License / 许可证

MIT License - See [../../LICENSE](../../LICENSE) for details.