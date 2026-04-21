# Tetris / 俄罗斯方块

> A terminal-based Tetris game built with C++17 and ncurses.
> 使用 C++17 和 ncurses 库编写的终端俄罗斯方块游戏。

## Features / 功能特性

| Feature | Description | 说明 |
|---------|-------------|------|
| SRS Rotation | Super Rotation System with wall kicks | 超级旋转系统，支持墙踢 |
| Ghost Piece | Shows where piece will land | 幽灵方块，显示落点位置 |
| Lock Delay | 500ms delay before piece locks | 500毫秒锁定延迟 |
| Level System | Speed increases every 1000 points | 每1000分加速一次 |
| Color Display | 7 colors for 7 piece types | 7种颜色对应7种方块 |
| Terminal Adaptive | Auto-centers based on terminal size | 自动适应终端大小居中 |

## Requirements / 系统要求

- **C++ Compiler**: C++17 or later (e.g., clang++, g++)
- **ncurses**: Terminal UI library

### macOS / Linux
```bash
# macOS
brew install ncurses

# Linux (Debian/Ubuntu)
sudo apt-get install libncurses5-dev libncursesw5-dev
```

## Build / 构建

```bash
# 1. 进入项目目录 / Navigate to project directory
cd tetris

# 2. 创建构建目录 / Create build directory
mkdir -p build && cd build

# 3. 配置CMake / Configure CMake
cmake ..

# 4. 编译 / Compile
make

# 5. 运行 / Run
./tetris
```

## Controls / 操作说明

| Key | Action | 说明 |
|-----|--------|------|
| `←` `→` | Move left/right | 左右移动 |
| `↑` | Rotate | 旋转 |
| `↓` | Soft drop (+1 point/cell) | 软降（加速下落，每格+1分）|
| `Space` | Hard drop (+2 points/cell) | 硬降（直接落底，每格+2分）|
| `P` | Pause/Resume | 暂停/继续 |
| `Q` | Quit | 退出 |

## Scoring / 计分规则

| Lines Cleared | Points | 说明 |
|--------------|--------|------|
| 1 | 100 | 单行 / Single |
| 2 | 300 | 双行 / Double |
| 3 | 500 | 三行 / Triple |
| 4 | 800 | 四行（俄罗斯方块）/ Tetris |

**Level**: 每1000分升一级，最高15级 / +1 level per 1000 points, max level 15

**Speed**: 每升一级下落间隔减少0.05秒，最快0.1秒 / -0.05s drop interval per level, min 0.1s

## Project Structure / 项目结构

```
tetris/
├── CMakeLists.txt          # 构建配置 / Build configuration
├── README.md               # 本文件 / This file
└── src/
    ├── main.cpp            # 入口点 / Entry point
    ├── board.h/cpp         # 棋盘数据结构 / Board data structure
    ├── piece.h/cpp         # 方块定义与旋转表 / Piece definitions & rotation tables
    ├── collision.h/cpp    # 碰撞检测与SRS墙踢 / Collision detection & SRS wall kicks
    ├── renderer.h/cpp     # ncurses渲染器 / ncurses renderer
    └── game.h/cpp          # 游戏主逻辑 / Main game logic
```

## Technical Details / 技术细节

### Piece Type Storage / 方块类型存储
- Grid stores `1-7` for piece types (not `0-6`)
- `0` = empty cell, `1` = I piece, `2` = O piece, etc.
- 网格存储 `1-7` 表示方块类型（不是 `0-6`）
- `0` = 空单元格，`1` = I 方块，`2` = O 方块，以此类推

### SRS Wall Kicks / SRS 墙踢系统
Uses standard Super Rotation System offset tables:
使用标准超级旋转系统偏移表：

```
J/L/S/T/Z: {0,0} → {0,-1} → {0,1} → {-1,0} → {1,0} → {-1,-1} → {1,-1} → {-1,1}
I:         {0,0} → {0,-2} → {0,2} → {-1,0} → {1,0} → {-2,0}  → {2,0}  → {-1,-1}
```

### Lock Delay / 锁定延迟
When piece cannot move down:
当方块无法继续下落时：

1. 启动500ms锁定计时器 / Start 500ms lock timer
2. 移动或旋转重置计时器 / Movement or rotation resets timer
3. 计时器归零后方块锁定 / Piece locks when timer reaches 0

## License / 许可证

MIT License
