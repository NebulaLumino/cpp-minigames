# Minesweeper / 扫雷

> A classic terminal-based Minesweeper game built with C++ and ncurses.
> 一个经典的终端扫雷游戏，使用 C++ 和 ncurses 构建。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

## Overview / 概览

A logic puzzle game where players must uncover cells on a grid without triggering mines. Uses number clues to deduce safe paths and flags mine locations.

一款逻辑解谜游戏，玩家必须在不触发地雷的情况下揭开网格上的格子。利用数字线索推断安全路径并标记地雷位置。

---

## Features / 功能特性

- **Flood Fill Algorithm / 洪水填充算法**: Auto-reveal safe regions
- **Flag System / 标记系统**: Mark suspected mines with flags
- **Multiple Difficulty Levels / 多难度级别**: Customizable rows, cols, mines
- **First-click Safety / 首次点击安全**: First click never hits a mine

---

## Building / 构建

```bash
# Create build directory / 创建构建目录
mkdir -p build && cd build

# Configure and build / 配置并构建
cmake ..

# Build the game / 构建游戏
make

# Run the game / 运行游戏
./minesweeper

# With custom difficulty / 自定义难度
./minesweeper 16 16 40

# Run tests / 运行测试
make minesweeper_test && ./minesweeper_test
```

---

## Controls / 操作说明

| Key | Action |
|-----|--------|
| Arrow Keys | Move cursor / 移动光标 |
| Space/Enter | Reveal cell / 揭示格子 |
| F | Toggle flag / 切换标记 |
| R | Restart game / 重新开始 |
| Q | Quit / 退出 |

---

## Game Rules / 游戏规则

1. Left-click or press Space to reveal a cell
2. Numbers show how many mines are adjacent (1-8)
3. Use numbers to deduce safe cells
4. Flag suspected mines with F key
5. Clear all non-mine cells to win
6. Revealing a mine ends the game

1. 左键点击或按空格揭示一个格子
2. 数字显示周围有多少地雷(1-8)
3. 利用数字推断安全格子
4. 用F键标记怀疑有地雷的位置
5. 清除所有非地雷格子即获胜
6. 揭示到地雷游戏结束

---

## Architecture / 架构

```
games/minesweeper/
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

**Board**: Manages game state
- `board_`: 2D grid storing mine positions and clues
- `revealed_`: Tracks revealed cells
- `flagged_`: Tracks flagged cells
- `floodFill()`: Auto-reveal empty regions

**Renderer**: ncurses-based display
- `draw()`: Renders game board and UI
- `processInput()`: Handles keyboard input

---

## Testing / 测试

Tests use [Catch2](https://github.com/catchorg/Catch2) v3.4.0:

```bash
cd build && cmake .. && make minesweeper_test && ctest --output-on-failure
```

---

## Project Structure / 项目结构

This game is part of the [cpp-minigames](../) mono-repo. See the root README for full project list.

本游戏是 [cpp-minigames](../) 单仓库项目的一部分。查看根目录README获取完整项目列表。

---

## License / 许可证

MIT License - See [../../LICENSE](../../LICENSE) for details.