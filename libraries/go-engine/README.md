# Go AI Engine / 围棋AI引擎

> A reusable Go AI library using Monte Carlo Tree Search (MCTS).
> 可重用的围棋AI库，使用蒙特卡洛树搜索（MCTS）。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

## Overview / 概览

A standalone Go AI library that can be integrated into any Go application. Features include:

一款可集成到任何围棋应用中的独立AI库，功能包括：

- **Monte Carlo Tree Search (MCTS) / 蒙特卡洛树搜索**: UCB1-based selection
- **Random Playout / 随机模拟**: Fast game simulation
- **Territory Evaluation / 地盘评估**: Area counting for endgame
- **Configurable Simulation Count / 可配置模拟次数**: 100-10000+ simulations

---

## Features / 功能特性

### Core AI / 核心AI
- **MCTS Selection**: UCB1 formula for balancing exploration/exploitation
- **Random Playout**: Quick random games for win rate estimation
- **Backpropagation**: Update visit counts and win rates
- **Move Priors**: Unvisited moves get priority

### Go Rules / 围棋规则
- Stone placement and capture
- Pass detection (two passes end game)
- Territory counting
- Komi support (6.5 points)

---

## Integration / 集成使用

### CMake Integration / CMake集成

```cmake
# In your CMakeLists.txt / 在你的CMakeLists.txt中
add_subdirectory(libraries/go-engine)
target_link_libraries(your_go_game PRIVATE go-engine)
```

### Basic Usage / 基本用法

```cpp
#include "go_engine.h"

GoEngine engine(9);  // 9x9 board

// Place a stone
engine.placeStone(4, 4, BLACK);

// Get AI's best move
GoMove move = engine.getBestMove(WHITE, 1000);  // 1000 simulations
if (!move.isPass) {
    engine.placeStone(move.row, move.col, WHITE);
}

// Check game state
if (engine.isGameOver()) {
    // Game ended (two passes)
}
```

### Configuration / 配置

```cpp
engine.setBoardSize(19);        // 9x9, 13x13, or 19x19
engine.setSimulationCount(2000); // More = stronger but slower
engine.setTimeLimit(5000);      // Time limit in ms
```

---

## Architecture / 架构

```
libraries/go-engine/
├── include/
│   └── go_engine.h   # Header-only library / 头文件库
├── tests/
│   └── test.cpp     # Unit tests / 单元测试
├── CMakeLists.txt   # Build configuration / 构建配置
└── README.md        # This file / 本文件
```

### Key Classes / 关键类

**GoEngine**: Main AI class
- `getBestMove(color, simulations)`: Returns best move using MCTS
- `placeStone(row, col, color)`: Places a stone
- `countTerritory(color)`: Counts territory for evaluation
- `isGameOver()`: Check if game ended (two passes)

**GoMove**: Move representation
- `row, col`: Position (-1, -1) for pass
- `color`: BLACK or WHITE
- `isPass`: True if this is a pass
- `visits, winRate`: MCTS statistics

---

## MCTS Algorithm / MCTS算法

### 1. Selection / 选择
Starting from root, select child with highest UCB1:
```
UCB1 = winRate + C * sqrt(ln(parentVisits) / childVisits)
```

### 2. Expansion / 扩展
Add new child node for untried move.

### 3. Simulation / 模拟
Play random moves until game ends.

### 4. Backpropagation / 回传
Update visit counts and win counts up the tree.

---

## Evaluation / 评估函数

The engine evaluates positions using:

引擎使用以下方法评估局面：

- **Territory**: Empty intersections surrounded by one color
- **Stones**: Count of stones on board
- **Komi**: 6.5 points for White to compensate for Black's first-move advantage

Final score = territory + stones (+ komi for White)

---

## Testing / 测试

```bash
mkdir -p build && cd build
cmake ..
make
./go_engine_test
```

---

## Example: Two-Player AI Game / 示例：双人AI游戏

```cpp
GoEngine engine(9);
engine.setSimulationCount(1000);

while (!engine.isGameOver()) {
    GoMove move = engine.getBestMove(engine.getCurrentTurn(), 1000);
    if (move.isPass) {
        engine.pass(engine.getCurrentTurn());
    } else {
        engine.placeStone(move.row, move.col, engine.getCurrentTurn());
    }
}

int blackScore = engine.countTerritory(BLACK);
int whiteScore = engine.countTerritory(WHITE) + 6.5;
```

---

## License / 许可证

MIT License - See [../../LICENSE](../../LICENSE) for details.