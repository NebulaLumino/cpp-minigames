# Go / 围棋

> A classic strategy board game built with C++ and ncurses.
> 一个经典的策略棋盘游戏，使用 C++ 和 ncurses 构建。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

## Overview / 概览

A classic strategy game where two players take turns placing stones on a 19x19 (or 9x9/13x13) grid, aiming to surround territory and capture opponent stones.

一款经典策略游戏，两名玩家轮流在19×19（或9×9/13×13）棋盘上放置棋子，目标是围取地盘并提吃对方棋子。

---

## Features / 功能特性

- **Multiple Board Sizes / 多种棋盘大小**: 9x9, 13x13, 19x19
- **Territory Counting / 数目计算**: Automatic territory and stone counting
- **Capture Mechanics / 提子机制**: Remove opponent stones with no liberties
- **Pass Function / 放弃之手**: Pass when no good moves available
- **Kö Rule / 劫规则**: Basic kö (superko not enforced in this simple version)

---

## Building / 构建

```bash
# Create build directory / 创建构建目录
mkdir -p build && cd build

# Configure and build / 配置并构建
cmake ..

# Build the game / 构建游戏
make go_game

# Run the game / 运行游戏
./go_game

# Run tests / 运行测试
make go_test && ./go_test
```

---

## Controls / 操作说明

| Key | Action |
|-----|--------|
| Arrow Keys | Move cursor / 移动光标 |
| Space/Enter | Place stone / 放置棋子 |
| P | Pass / 放弃 |
| Q | Quit / 退出 |

---

## Game Rules / 游戏规则

### Basic Concepts / 基本概念
- **Black plays first** (棋盘上的X) / 黑方先行
- **Stones are placed** on intersections, not squares / 棋子放在交叉点上
- **Liberties** are empty points adjacent to a stone or group / 气是棋子或棋串相邻的空点
- **Capturing** occurs when a group has no liberties / 无气的棋串被提走

### Territory / 地盘
- **Territory** is empty area surrounded by one player's stones / 被一方棋子围住的空区域
- **Final score** = stones + territory (+ komi for white) / 最终得分 = 子数 + 地盘 (+ 贴目给白方)

### Win Condition / 胜利条件
- Game ends when both players pass consecutively
- Player with higher score wins (Black: stones+territory, White: stones+territory+6.5 komi)

---

## Scoring / 计分

- **Chinese scoring** (地盘): Count empty intersections surrounded + stones
- **Komi** (贴目): 6.5 points awarded to white to compensate for black's first-move advantage

---

## Architecture / 架构

```
games/go/
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

**GoBoard**: Manages Go game state
- `board_`: 2D grid of stones
- `placeStone()`: Place stone and handle captures
- `captureGroup()`: Remove opponent stones
- `countTerritory()`: Calculate territory

**Stone Colors**: BLACK (X), WHITE (O), EMPTY

**Renderer**: ncurses-based display
- `draw()`: Renders board and UI
- `processInput()`: Handles keyboard input

---

## Testing / 测试

Tests use [Catch2](https://github.com/catchorg/Catch2) v3.4.0:

```bash
cd build && cmake .. && make go_test && ctest --output-on-failure
```

---

## Project Structure / 项目结构

This game is part of the [cpp-minigames](../) mono-repo. See the root README for full project list.

本游戏是 [cpp-minigames](../) 单仓库项目的一部分。查看根目录README获取完整项目列表。

---

## License / 许可证

MIT License - See [../../LICENSE](../../LICENSE) for details.