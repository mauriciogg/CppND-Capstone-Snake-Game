cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src)

add_executable(SnakeGame 
    src/main.cpp 
    src/game.cpp 
    src/controller.cpp 
    src/renderer.cpp 
    src/snake_base.cpp
    src/player_snake.cpp
    src/ai_snake.cpp
    src/astar_pathfinder.cpp
    src/game_state.cpp
    src/pathfinding_thread.cpp
)
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(SnakeGame ${SDL2_LIBRARIES} pthread)
