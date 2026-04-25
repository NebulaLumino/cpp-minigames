# Sokoban / 推箱子

> A classic warehouse puzzle game built with C++ and ncurses.
> 一个经典的仓库推箱子益智游戏，使用 C++ 和 ncurses 构建。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

## Overview / 概览

A classic puzzle game where you push boxes onto target locations. Navigate through increasingly complex warehouse layouts to complete each level.

一款经典益智游戏，将箱子推到目标位置。通过越来越复杂的仓库布局完成每个关卡。

---

## Features / 功能特性

- **Multiple Levels / 多关卡**: 5 progressively harder puzzles
- **Push Mechanics / 推动机制**: Push boxes, avoid getting stuck
- **Color-coded Display / 彩色显示**: Different colors for boxes and targets
- **Move Counter / 步数统计**: Track your efficiency

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
./sokoban

# Run tests / 运行测试
make sokoban_test && ./sokoban_test
```

---

## Controls / 操作说明

| Key | Action |
|-----|--------|
| Arrow Keys | Move player / 移动玩家 |
| N | Next level (after winning) / 下一关（获胜后） |
| R | Restart current level / 重新开始当前关卡 |
| Q | Quit / 退出 |

---

## Game Rules / 游戏规则

1. Push all boxes (yellow `[`) onto target locations (green `.`)
2. Boxes can only be pushed, not pulled
3. You can only push one box at a time
4. A box cannot be pushed into a wall or another box
5. Complete each level by placing all boxes on targets

1. 将所有箱子（黄色`[`）推到目标位置（绿色`.`）
2. 箱子只能推，不能拉
3. 一次只能推一个箱子
4. 箱子不能推入墙或另一个箱子
5. 将所有箱子放在目标上即完成关卡

---

## Symbols / 符号说明

| Symbol | Meaning |
|--------|---------|
| `#` | Wall / 墙 |
| `@` | Player / 玩家 |
| `$` | Box / 箱子 |
| `.` | Target / 目标 |
| `*` | Box on target / 箱子在目标上 |
| `+` | Player on target / 玩家在目标上 |

---

## Architecture / 架构

```
games/sokoban/
├── src/
│   ├── main.cpp      # Entry point / 入口点
│   ├── level.h       # Level class declaration / 关卡类声明
│   ├── level.cpp     # Level logic / 关卡逻辑
│   ├── renderer.h    # Renderer declaration / 渲染器声明
│   └── renderer.cpp  # ncurses rendering / ncurses渲染
├── tests/
│   └── LevelTest.cpp # Unit tests / 单元测试
└── CMakeLists.txt    # Build configuration / 构建配置
```

### Class Structure / 类结构

**Level**: Manages puzzle state
- `grid_`: 2D grid of cells (wall, box, target, etc.)
- `playerX_`, `playerY_`: Player position
- `pushBox()`: Handle box pushing logic
- `checkWin()`: Check if all boxes on targets

**Renderer**: ncurses-based display
- `draw()`: Renders puzzle and UI
- `processInput()`: Handles keyboard input

---

## Testing / 测试

Tests use [Catch2](https://github.com/catchorg/Catch2) v3.4.0:

```bash
cd build && cmake .. && make sokoban_test && ctest --output-on-failure
```

---

## Project Structure / 项目结构

This game is part of the [cpp-minigames](../) mono-repo. See the root README for full project list.

本游戏是 [cpp-minigames](../) 单仓库项目的一部分。查看根目录README获取完整项目列表。

---

## License / 许可证

MIT License - See [../../LICENSE](../../LICENSE) for details.