# CPPND: Enhanced Snake Game with AI Opponent

This is an enhanced version of the Snake game with an AI opponent that uses A* pathfinding algorithm. The project demonstrates advanced C++ programming concepts including concurrency, smart pointers, RAII, and algorithmic pathfinding.

<img src="snake_game.gif"/>

## New Features Added

### 🤖 AI Snake Opponent
- **Intelligent Movement**: AI snake uses A* pathfinding algorithm to navigate to food optimally
- **Balanced Difficulty**: AI has reduced speed and occasional suboptimal moves for fair gameplay
- **Visual Distinction**: Player snake is blue, AI snake is red

### 🎮 Enhanced Gameplay
- **Competitive Scoring**: Both player and AI scores are tracked and displayed in the window title
- **Collision Reset**: Game resets on collision instead of terminating, allowing continuous play
- **Round-based Play**: Each collision ends a round with score comparison and restart

### 🧵 Concurrent Processing
- **Multi-threaded AI**: A* pathfinding runs in a separate thread to maintain smooth gameplay
- **Thread-safe Communication**: Uses mutexes and condition variables for safe data sharing
- **Real-time Updates**: AI continuously recalculates optimal paths during gameplay

### 💻 Modern C++ Features
- **Smart Pointers**: Automatic memory management with `std::unique_ptr` and `std::shared_ptr`
- **RAII**: Resource management for threads, mutexes, and SDL resources
- **STL Containers**: Priority queues for A*, vectors for paths, unordered sets for visited nodes
- **Inheritance Hierarchy**: Clean OOP design with base snake class and specialized implementations

## Expected Program Behavior

### Game Mechanics
- **Player Control**: Use arrow keys to control the blue snake
- **AI Behavior**: Red AI snake automatically navigates toward food using optimal pathfinding
- **Food Collection**: Both snakes compete for the same food item
- **Growth**: Snakes grow longer and slightly faster when eating food
- **Collision Handling**: Any collision (snake-to-snake or self-collision) resets the game

### Console Output
When the game starts:
```
Game has terminated successfully!
Player Score: X
Player Size: Y
AI Score: Z
AI Size: W
```

During gameplay, each collision triggers:
```
=== GAME OVER ===
Final Scores - Player: X | AI: Y
Snake Sizes - Player: A | AI: B
[Player wins this round! / AI wins this round! / It's a tie!]
Restarting game...
```

### Visual Elements
- **Window Title**: Shows real-time scores and FPS: "Player: X AI: Y FPS: Z"
- **Player Snake**: Blue colored segments
- **AI Snake**: Red colored segments  
- **Food**: Yellow/gold colored
- **Background**: Dark gray

### Performance
- **Smooth Gameplay**: 60 FPS target with concurrent AI processing
- **Responsive Controls**: Player input handled in main thread
- **Balanced AI**: Strategic delays and randomness prevent AI from being unbeatable

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
