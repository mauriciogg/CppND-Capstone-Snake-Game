# Implementation Plan: AI Snake with A* Pathfinding

## Overview
Add a computer-controlled snake that uses A* search algorithm to compete with the human player for food. The implementation will leverage modern C++ features including concurrency, smart pointers, and RAII principles.

## Phase 1: Core Infrastructure

### 1.1 Base Snake Abstraction
- Create a base `SnakeBase` class to share common functionality between `PlayerSnake` and `AISnake`
- Refactor existing `Snake` class to inherit from `SnakeBase`
- Use virtual methods for behavior differences (movement, direction changes)

### 1.2 AI Snake Implementation
- Create `AISnake` class that inherits from `SnakeBase`
- Implement A* pathfinding algorithm in a separate `AStarPathfinder` class
- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) for memory management

### 1.3 Pathfinding System
- Implement `Node` struct for A* algorithm with proper RAII
- Create `AStarPathfinder` class with:
  - Heuristic function (Manhattan distance)
  - Path reconstruction
  - Obstacle avoidance (snake bodies, walls)
  - Dynamic replanning when path becomes invalid

## Phase 2: Concurrency Integration

### 2.1 Threaded AI Processing
- Run A* pathfinding in separate thread to avoid blocking main game loop
- Use `std::thread` and `std::async` for concurrent execution
- Implement thread-safe communication using `std::mutex` and `std::condition_variable`

### 2.2 Safe State Sharing
- Create thread-safe `GameState` class to share information between threads
- Use `std::atomic` for simple shared variables
- Implement proper synchronization for complex data structures

### 2.3 Resource Management
- Use RAII principles for thread management
- Implement proper cleanup in destructors
- Use `std::lock_guard` and `std::unique_lock` for automatic lock management

## Phase 3: Game Logic Updates

### 3.1 Collision Detection
- Implement snake-to-snake collision detection
- Add collision handling for AI snake vs player snake
- Game over logic when player collides with AI snake

### 3.2 Multi-Snake Game Management
- Update `Game` class to manage both snakes
- Implement fair food placement logic
- Add scoring system for competitive gameplay

### 3.3 Rendering Updates
- Modify `Renderer` class to draw both snakes with different colors
- Add visual indicators for AI snake path (optional debug mode)
- Update window title to show both scores

## Phase 4: Performance and Polish

### 4.1 Optimization
- Optimize A* algorithm with appropriate data structures (`std::priority_queue`)

### 4.2 Configuration
- Allow enabling/disabling AI snake

### 4.3 Error Handling
- Implement robust error handling for threading operations
- Add graceful degradation if AI thread fails
- Proper exception handling throughout

## Key Modern C++ Features Used

1. **Smart Pointers**: `std::unique_ptr` for pathfinding nodes, `std::shared_ptr` for shared game state
2. **RAII**: Automatic resource management for threads, mutexes, and SDL resources
3. **Concurrency**: `std::thread`, `std::async`, `std::mutex`, `std::condition_variable`, `std::atomic`
4. **STL Containers**: `std::priority_queue` for A*, `std::vector` for paths, `std::unordered_set` for visited nodes
5. **Move Semantics**: Efficient path transfers between threads
6. **Lambda Functions**: For custom comparators and thread callbacks
7. **Template Programming**: Generic pathfinding that can work with different node types

## Implementation Order

1. **Foundation**: Base classes and interfaces
2. **Core Algorithm**: A* pathfinding implementation
3. **Integration**: Single-threaded AI integration
4. **Concurrency**: Multi-threaded implementation
5. **Game Logic**: Collision detection and game rules
6. **Rendering**: Visual updates
7. **Polish**: Performance optimization and configuration

## Success Criteria

- AI snake successfully navigates to food using A* pathfinding
- Smooth gameplay with no frame drops due to AI processing
- Proper collision detection between snakes
- Memory-safe implementation with no leaks
- Thread-safe operation with proper synchronization
- Graceful handling of edge cases and errors

## Files to be Created/Modified

### New Files:
- `src/snake_base.h/cpp` - Base snake functionality
- `src/ai_snake.h/cpp` - AI-controlled snake
- `src/player_snake.h/cpp` - Human-controlled snake  
- `src/astar_pathfinder.h/cpp` - A* pathfinding algorithm
- `src/game_state.h/cpp` - Thread-safe game state
- `src/pathfinding_thread.h/cpp` - AI processing thread management

### Modified Files:
- `src/game.h/cpp` - Multi-snake game management
- `src/renderer.h/cpp` - Multi-snake rendering
- `src/controller.h/cpp` - Input handling updates
- `src/main.cpp` - Initialization updates 