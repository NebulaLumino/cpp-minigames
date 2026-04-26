# Go AI Engine Library
# 围棋AI引擎库
#
# This is a header-only library that can be integrated via CMake:
# add_subdirectory(libraries/go-engine)
# target_link_libraries(your_target PRIVATE go-engine)
#
# Build tests: mkdir -p build && cd build && cmake .. && make && ./test

cmake_minimum_required(VERSION 3.16)
project(GoEngine VERSION 1.0 LANGUAGES CXX)

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
# Header-only library / 头文件库
# ============================================================================
add_library(go-engine INTERFACE)
target_include_directories(go-engine INTERFACE include)

# ============================================================================
# Tests / 测试
# ============================================================================
add_executable(go_engine_test
    tests/test.cpp
)
target_link_libraries(go_engine_test PRIVATE Catch2::Catch2WithMain go-engine)

add_test(NAME GoEngineTest COMMAND go_engine_test)