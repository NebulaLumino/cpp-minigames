# 2048 / 2048益智游戏

> A classic sliding tile puzzle game built with C++ and ncurses.
> 一个经典的滑动拼图游戏，使用 C++ 和 ncurses 构建。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

## Overview / 概览

A addictive puzzle game where you slide numbered tiles to combine them and reach 2048. Simple controls, endless replayability.

一款令人上瘾的益智游戏，通过滑动数字方块将它们合并以达到2048。简单操作，无限重玩。

---

## Features / 功能特性

- **Color-coded Tiles / 彩色方块**: Each number has a unique color
- **Score System / 计分系统**: Track current and best scores
- **Smooth Controls / 流畅操作**: Arrow key sliding
- **Win Detection / 胜利检测**: Celebrate reaching 2048

---

## Building / 构建

```bash
# Create build directory / 创建构建目录
mkdir -p build && cd build

# Configure and build / 配置并构建
cmake ..

# Build the game / 构建游戏
make 2048_game

# Run the game / 运行游戏
./2048_game

# Run tests / 运行测试
make 2048_test && ./2048_test
```

---

## Controls / 操作说明

| Key | Action |
|-----|--------|
| Arrow Keys | Slide tiles / 滑动方块 |
| N | Continue after winning / 获胜后继续 |
| R | Restart game / 重新开始 |
| Q | Quit / 退出 |

---

## Game Rules / 游戏规则

1. Use arrow keys to slide all tiles in that direction
2. When two tiles with the same number touch, they merge into one
3. After each move, a new tile (2 or 4) appears randomly
4. Reach the 2048 tile to win!
5. If no moves are possible (board full, no adjacent matches), game over

1. 使用方向键将所有方块滑向该方向
2. 当两个相同数字的方块相遇时，它们合并成一个
3. 每次移动后，随机出现一个新的方块（2或4）
4. 达到2048方块即获胜！
5. 如果无法移动（棋盘满了且没有相邻匹配），游戏结束

---

## Architecture / 架构

```
games/2048/
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

**GameBoard**: Manages 2048 puzzle state
- `grid_`: 4x4 array of tile values
- `mergeRow()`: Merge tiles in a row
- `moveLeft/Right/Up/Down()`: Slide and merge operations

**Renderer**: ncurses-based display
- `draw()`: Renders board and UI
- `getColorForValue()`: Color mapping for tiles

---

## Testing / 测试

Tests use [Catch2](https://github.com/catchorg/Catch2) v3.4.0:

```bash
cd build && cmake .. && make 2048_test && ctest --output-on-failure
```

---

## Project Structure / 项目结构

This game is part of the [cpp-minigames](../) mono-repo. See the root README for full project list.

本游戏是 [cpp-minigames](../) 单仓库项目的一部分。查看根目录README获取完整项目列表。

---

## License / 许可证

MIT License - See [../../LICENSE](../../LICENSE) for details.