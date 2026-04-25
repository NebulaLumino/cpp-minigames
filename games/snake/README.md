# Snake Game / 贪吃蛇

> A classic terminal-based snake game built with C++ and ncurses.
> 一个经典的终端贪吃蛇游戏，使用 C++ 和 ncurses 构建。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

## Overview / 概览

A classic arcade-style snake game where players control a growing snake to eat food while avoiding walls and self-collision. Features multiple difficulty levels and color-coded display.

一款经典街机风格的贪吃蛇游戏，玩家控制一条不断生长的蛇来吃食物，同时避免撞墙和撞到自己。具有多个难度等级和彩色显示。

---

## Features / 功能特性

- **Colorful Display / 彩色显示**: Green snake head, cyan body, red food
- **Multiple Levels / 多级难度**: Speed increases as score grows
- **Smooth Controls / 流畅控制**: Arrow key movement, P to pause, Q to quit
- **Score System / 计分系统**: +10 points per food, level up every 50 points

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
./snake

# Run tests / 运行测试
make snake_test && ./snake_test
```

---

## Controls / 操作说明

| Key | Action |
|-----|--------|
| Arrow Keys | Move snake / 移动蛇 |
| P | Pause/Resume / 暂停/继续 |
| Q | Quit / 退出 |
| SPACE | Restart after game over / 游戏结束后重新开始 |

---

## Game Rules / 游戏规则

1. Use arrow keys to control the snake's direction
2. Eat the red food (oo) to grow and score points
3. Avoid hitting walls (boundaries) or yourself
4. Speed increases every 50 points (level up)
5. Game ends when snake collides with wall or itself

1. 使用方向键控制蛇的移动方向
2. 吃红色食物(oo)来增长长度和获得分数
3. 避免撞墙（边界）或撞到自己
4. 每得50分速度加快（升级）
5. 当蛇撞墙或撞到自己时游戏结束

---

## Architecture / 架构

```
games/snake/
├── src/
│   ├── main.cpp      # Entry point / 入口点
│   ├── game.h        # Game class declaration / 游戏类声明
│   ├── game.cpp      # Game logic / 游戏逻辑
│   ├── renderer.h    # Renderer declaration / 渲染器声明
│   └── renderer.cpp  # ncurses rendering / ncurses渲染
├── tests/
│   └── GameTest.cpp  # Unit tests / 单元测试
└── CMakeLists.txt    # Build configuration / 构建配置
```

### Class Structure / 类结构

**SnakeGame**: Main game class managing state
- `snake_`: Vector of {x, y} coordinates
- `foodX_`, `foodY_`: Food position
- `direction_`: Current movement direction
- `score_`, `level_`: Game progression

**Renderer**: ncurses-based display
- `draw()`: Renders game board and UI
- `processEvents()`: Handles keyboard input

---

## Testing / 测试

Tests use [Catch2](https://github.com/catchorg/Catch2) v3.4.0:

```bash
cd build && cmake .. && make snake_test && ctest --output-on-failure
```

---

## Project Structure / 项目结构

This game is part of the [cpp-minigames](../) mono-repo. See the root README for full project list.

本游戏是 [cpp-minigames](../) 单仓库项目的一部分。查看根目录README获取完整项目列表。

---

## License / 许可证

MIT License - See [../../LICENSE](../../LICENSE) for details.