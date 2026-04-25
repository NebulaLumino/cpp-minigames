# CPP-MINIGAMES / CPP小游戏合集

> A collection of terminal-based mini-games built with C++17 and ncurses.
> 使用 C++17 和 ncurses 编写的终端小游戏集合，适合初学者学习参考。

## Games / 游戏列表

| Game | Description | 说明 |
|------|-------------|------|
| [Tetris](./games/tetris/) | Classic block puzzle game | 经典俄罗斯方块 |
| [Snake](./games/snake/) | Classic snake arcade game | 经典贪吃蛇 |
| [Minesweeper](./games/minesweeper/) | Logic puzzle, find mines | 扫雷逻辑解谜 |
| [Sokoban](./games/sokoban/) | Warehouse puzzle, push boxes | 推箱子仓库解谜 |
| [2048](./games/2048/) | Sliding tile puzzle, reach 2048 | 滑动拼图，达到2048 |
| [Chess](./games/chess/) | Two-player strategy board game | 双人策略棋盘游戏 |

## Quick Start / 快速开始

```bash
# Clone the repository
git clone https://github.com/NebulaLumino/cpp-minigames.git
cd cpp-minigames

# Build a specific game
cd games/tetris/build
cmake ../..
make

# Run
./tetris
```

## Project Structure / 项目结构

```
cpp-minigames/
├── README.md              # 本文件 / This file
├── LICENSE                # MIT许可证 / MIT License
├── games/
│   └── tetris/            # 俄罗斯方块 / Tetris
│       ├── CMakeLists.txt
│       ├── src/           # 源代码 / Source code
│       └── tests/         # 单元测试 / Unit tests
```

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

## Contributing / 贡献

Contributions welcome! Feel free to add more mini-games to the collection.

See [CONTRIBUTING.md](./games/tetris/CONTRIBUTING.md) for details.

## License / 许可证

MIT License - see individual game directories for details.
