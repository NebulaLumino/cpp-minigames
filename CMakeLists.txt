#/**
# * @file CMakeLists.txt
# * @brief 俄罗斯方块构建配置 / Tetris Build Configuration
# *
# * 使用CMake构建系统，依赖ncurses库。
# * Uses CMake build system with ncurses dependency.
# *
# * @构建说明 Build Instructions:
# *   mkdir -p build && cd build && cmake .. && make
# *   ./tetris
# *
# * @author NebulaLumino
# * @date 2026
# */

cmake_minimum_required(VERSION 3.16)
project(Tetris VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源代码文件 / Source files
set(SOURCES
    src/main.cpp
    src/board.cpp
    src/piece.cpp
    src/collision.cpp
    src/renderer.cpp
    src/game.cpp
)

add_executable(tetris ${SOURCES})
target_include_directories(tetris PRIVATE src)

# ncurses 库配置 / ncurses library configuration
find_package(Curses)
if(Curses_FOUND)
    target_include_directories(tetris PRIVATE ${CURSES_INCLUDE_DIR})
    target_link_libraries(tetris ${CURSES_LIBRARIES})
endif()
