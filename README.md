# CPPND: Enhanced Snake Game with AI Opponent

This is an enhanced version of the Snake game with an AI opponent that uses A* pathfinding algorithm. 

<img src="snake_game.gif"/>

## New Features Added

### AI Snake Player 

An extra snake was added (AISnake class). Implemented in (src/ai_snke.<cpp.h>. The snake competes with the human player snake for the food. The snake uses the
A* pathfinding algorithm to find the food, but I added random mistakes to the movements and slowdowns to
the AI snake movements to not make the game terribly diffivult. The values for fairness where chosen arbitrarily by playing
against the AI snake with different values and choosing ones that seemed balanced. The logic for fairness
is implemented in the methods `ShouldMoveThisFrame` and `ShouldMakeMistake`. The human snake is blue, AI snake is red.
When the two snakes collide the final scores are logged to the console and then reset. The program does not terminate.

## Running the project

### Build and run the game:

```
    mkdir build && cd build
    cmake ..
    ./SnakeGame    
```
### Game Mechanics

There are two snakes. An AI snake and the human controlled snake.
Use arrow keys to control the blue snake and move it towrds the food. Colliding with the AI snake will result in game over
and the scores will reset. Both snakes compete for food and score the most points.

During gameplay, each collision triggers:
```
=== GAME OVER ===
Final Scores - Player: X | AI: Y
Snake Sizes - Player: A | AI: B
[Player wins this round! / AI wins this round! / It's a tie!]
Restarting game...
```

## Code Structure Overview

### Snake Class Hierarchy

```
SnakeBase (Abstract Base Class)
├── PlayerSnake (Human-controlled snake)
└── AISnake (AI-controlled snake)
```

- **`SnakeBase`** (`src/snake_base.h/.cpp`): Abstract base class defining common snake behavior
  - Pure virtual `Update()` method for polymorphic behavior
  - Shared functionality: movement, body management, collision detection

- **`PlayerSnake`** (`src/player_snake.h/.cpp`): Inherits from SnakeBase
  - Implements `Update()` with user-controlled movement

- **`AISnake`** (`src/ai_snake.h/.cpp`): Inherits from SnakeBase
  - Implements `Update()` with AI pathfinding logic
  - Uses `AStarPathfinder` to find the food 

### Pathfinding Logig 

- **`AStarPathfinder`** (`src/astar_pathfinder.h/.cpp`): A* algorithm implementation
  - Uses a priority queue as the underlying container for efficiency 
  - Returns optimal path as vector of SDL_Point coordinates

- **`PathfindingThread`** (`src/pathfinding_thread.h/.cpp`): Concurrent processing
  - Runs A* calculations in separate thread to avoid blocking the main thread 

### Game State Management

- **`GameState`** (`src/game_state.h/.cpp`): Thread-safe state container
  - Manages shared data between main game loop and pathfinding thread
  - Uses mutexes (`std::lock_guard`) for thread safety
  - Stores food position, snake references, and game status

### Threading Architecture

```
**Main Thread**
├── Game Loop 
├── Rendering
├── Input Handling
└── State Updates


**Pathfinding Thread**
├── A* Calculations
├── Path Updates
└── Obstacle Detection

```



## Rubric Points Addressed

Below are the specific rubric points addressed with corresponding code locations:

### Loops, Functions, and I/O

1. **The project demonstrates an understanding of C++ functions and control structures**
   - `src/astar_pathfinder.cpp` implementation of A* pathfinding algorithm with loops and conditionals
   - `src/ai_snake.cpp` AI update logic with control structures

2. **The project code is clearly organized into functions**
   - The projec is organized in classes with well defined and encapsulated behavior.
   - Snake specific logic is encapsulated in `src/player_snake.h` and `src/ai_snake.h` but common logic is placed in `src/base_snake.h`
   - A* star algorithm logic and implementation is defined as its own class. AI snake uses the A* class as oposed of implementing it.
   - The state of the game is tracked in its own class by GameState (`src/game_state.h`)
   
### Object Oriented Programming

1. **Classes use appropriate access specifiers for class members**
   - `src/snake_base.h` lines 9-43
   - `src/ai_snake.h` lines 10-35
   - `src/game_state.h` lines 11-32

2. **Class constructors utilize member initialization lists**
   - `src/snake_base.cpp` lines 4-6
   - `src/ai_snake.cpp` lines 5-16
   - `src/game_state.cpp` lines 4-5

3. **Classes abstract implementation details from their interfaces**
   - `src/snake_base.h` lines 15-16: Pure virtual () method abstracts snake behavior and is implemented by AISnake and PlayerSnake.
   - `src/pathfinding_thread.h` lines 23-26: Thread management details hidden. 

5. **Classes follow an appropriate inheritance hierarchy with virtual and override functions**
   - `src/snake_base.h` line 13: Virtual destructor for proper inheritance
   - `src/snake_base.h` line 15: Pure virtual Update() method
   - `src/ai_snake.h` line 13: Override Update() method in derived class
   - `src/player_snake.h` PlayerSnake inherits from SnakeBase
   - `src/ai_snake.h` AISnake inherits from SnakeBase

### Memory Management

1. **The project makes use of references in function declarations**
   - `src/ai_snake.h` line 15: `const std::vector<const SnakeBase*>& obstacles`
   - `src/renderer.h` line 14: `Snake const &player_snake, AISnake const &ai_snake`
   - `src/snake_base.cpp` line 33: `SDL_Point &current_cell, SDL_Point &prev_cell`

2. **The project uses destructors appropriately**
   - `src/snake_base.h` line 13: Virtual destructor for proper cleanup
   - `src/pathfinding_thread.cpp` lines 8-10: Destructor properly stops thread

3. **The project uses scope/RAII appropriately**
   - `src/game_state.cpp` lines 8, 13, 18, 23: std::lock_guard for automatic mutex management
   - `src/pathfinding_thread.cpp` lines 20-23: RAII thread management

5. **The project uses move semantics to move data, instead of copying it**
   - `src/ai_snake.cpp` lines 7-13: Move semantics in constructor initialization
   - `src/astar_pathfinder.cpp` line 47: Move semantics in path reconstruction

6. **The project uses smart pointers instead of raw pointers**
   - `src/ai_snake.h` line 19: `std::unique_ptr<AStarPathfinder> pathfinder_`
   - `src/game.h` lines 21-24: `std::shared_ptr` for snakes and game state
   - `src/pathfinding_thread.h` line 17: `std::unique_ptr<std::thread> worker_thread_`

### Concurrency

1. **The project uses multithreading**
   - `src/pathfinding_thread.h` line 4: Thread header inclusion
   - `src/pathfinding_thread.cpp` lines 15-17: Thread creation and management
   - `src/game.cpp` line 17: PathfindingThread integration

2. **A promise and future is used in the project**
   - `src/pathfinding_thread.cpp` lines 25-30: Condition variable for thread communication
   - `src/game_state.h` lines 27-29: Atomic variables for thread-safe state sharing

3. **A mutex or lock is used in the project**
   - `src/game_state.h` line 32: `std::mutex state_mutex_`
   - `src/game_state.cpp` lines 8, 13, 18, 23: std::lock_guard usage
   - `src/pathfinding_thread.h` line 20: `std::mutex cv_mutex_`

4. **A condition variable is used in the project**
   - `src/pathfinding_thread.h` line 19: `std::condition_variable cv_`
   - `src/pathfinding_thread.cpp` lines 25-28: Condition variable wait and notify
   - `src/pathfinding_thread.cpp` line 14: NotifyStateChanged() triggers condition variable

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
