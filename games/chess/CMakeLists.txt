# Chess Build Configuration
# 国际象棋游戏构建配置
#
# Build: mkdir -p build && cd build && cmake .. && make && ./chess
# Test:  cd build && cmake .. && make chess_test && ctest --output-on-failure

cmake_minimum_required(VERSION 3.16)
project(Chess VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================
# Test dependencies: Catch2 (header-only via FetchContent)
# ============================================================================
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

# ============================================================================
# Curses library / Curses库
# ============================================================================
find_package(Curses REQUIRED)
set(CURSES_LIBRARY libcurses.tbd)
set(CURSES_INCLUDE_DIR /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include)

# ============================================================================
# Source files / 源代码文件
# ============================================================================
set(SOURCES
    src/main.cpp
    src/board.cpp
    src/renderer.cpp
)

add_executable(chess ${SOURCES})
target_include_directories(chess PRIVATE src ${CURSES_INCLUDE_DIR})
target_link_libraries(chess PRIVATE ${CURSES_LIBRARY})

# ============================================================================
# Tests / 测试
# ============================================================================
add_executable(chess_test
    src/board.cpp
    tests/BoardTest.cpp
)
target_include_directories(chess_test PRIVATE src ${CURSES_INCLUDE_DIR})
target_link_libraries(chess_test PRIVATE Catch2::Catch2WithMain)