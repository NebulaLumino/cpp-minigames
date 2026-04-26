# Chess AI Engine / 国际象棋AI引擎

> A reusable header-only chess AI library with minimax search and alpha-beta pruning.
> 可重用的头文件国际象棋AI库，包含极小极大搜索和Alpha-Beta剪枝。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

## Overview / 概览

A standalone chess AI library that can be integrated into any chess application. Features include:

一款可集成到任何国际象棋应用中的独立AI库，功能包括：

- **Legal Move Generation / 合法走法生成**: All standard chess moves including castling, en passant, promotion
- **Minimax with Alpha-Beta Pruning / 极小极大与Alpha-Beta剪枝**: Efficient depth-first search
- **Material Evaluation / 物质评估**: Piece values (Queen=900, Rook=500, Bishop=330, Knight=320, Pawn=100)
- **Positional Evaluation / 位置评估**: Piece-square tables for opening/endgame
- **Move Ordering / 走法排序**: History heuristics for better pruning

---

## Features / 功能特性

### Core AI / 核心AI
- Configurable search depth (1-8 ply)
- Quiescence search (optional)
- Alpha-beta pruning for efficiency
- Move ordering for better pruning

### Chess Rules / 象棋规则
- All standard moves (King, Queen, Rook, Bishop, Knight, Pawn)
- Castling (kingside and queenside)
- En passant capture
- Pawn promotion
- Check, checkmate, and stalemate detection

### Integration / 集成
- Header-only library (no compilation needed)
- Simple C++ interface
- FEN position loading/saving

---

## Integration / 集成使用

### CMake Integration / CMake集成

```cmake
# In your CMakeLists.txt / 在你的CMakeLists.txt中
add_subdirectory(libraries/chess-engine)
target_link_libraries(your_chess_game PRIVATE chess-engine)
```

### Basic Usage / 基本用法

```cpp
#include "chess_engine.h"

ChessEngine engine;
engine.reset();  // Set to initial position

// Get best move with specified depth
ChessMove bestMove = engine.getBestMove(4);

// Make a move
engine.makeMove(move);

// Check game state
if (engine.isCheckmate(WHITE)) {
    // Game over
}
```

### FEN Position Loading / FEN位置加载

```cpp
// Load a specific position from FEN notation
engine.loadFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
```

---

## Architecture / 架构

```
libraries/chess-engine/
├── include/
│   └── chess_engine.h   # Header-only library / 头文件库
├── tests/
│   └── test.cpp         # Unit tests / 单元测试
├── CMakeLists.txt       # Build configuration / 构建配置
└── README.md            # This file / 本文件
```

### Key Classes / 关键类

**ChessEngine**: Main AI class
- `getBestMove(depth)`: Returns best move using minimax
- `generateAllMoves(color)`: Generates all legal moves
- `evaluate()`: Returns position evaluation
- `makeMove(move)`: Executes a move

**ChessMove**: Move representation
- `fromRow, fromCol`: Source square
- `toRow, toCol`: Destination square
- `promotion`: Pawn promotion piece (if any)

**ChessBoardState**: Board state
- `board[8][8]`: Piece positions
- `sideToMove`: Current turn
- Castling rights, en passant square, etc.

---

## Evaluation / 评估函数

The engine uses a combination of:

引擎使用以下组合进行评估：

### Material Score / 物质分数
| Piece | Value |
|-------|-------|
| Queen | 900 |
| Rook | 500 |
| Bishop | 330 |
| Knight | 320 |
| Pawn | 100 |

### Positional Score / 位置分数
Uses piece-square tables for better play in opening and endgame phases.

使用棋子位置表来优化开局和残局阶段的走棋。

---

## Testing / 测试

```bash
mkdir -p build && cd build
cmake ..
make
./chess_engine_test
```

---

## Example: Two-Player AI Game / 示例：双人AI游戏

```cpp
ChessEngine engine;
engine.reset();

while (!engine.isGameOver()) {
    std::cout << "Thinking..." << std::endl;
    ChessMove move = engine.getBestMove(4);
    engine.makeMove(move);
    engine.printBoard();  // If you have this function
}
```

---

## License / 许可证

MIT License - See [../../LICENSE](../../LICENSE) for details.