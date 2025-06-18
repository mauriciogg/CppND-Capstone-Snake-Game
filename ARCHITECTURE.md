# High-Level Architecture: AI Snake Game

## System Overview

The enhanced Snake game features a multi-threaded architecture where a human-controlled snake competes against an AI-controlled snake using A* pathfinding. The system emphasizes modern C++ practices, thread safety, and efficient resource management.

## Component Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        Main Thread                          │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │    Game     │  │ Controller  │  │      Renderer       │  │
│  │   Manager   │  │             │  │                     │  │
│  └─────────────┘  └─────────────┘  └─────────────────────┘  │
│         │                                      │             │
│         │                                      │             │
│  ┌─────────────┐                    ┌─────────────────────┐  │
│  │ GameState   │                    │   Rendering Queue   │  │
│  │ (Threadsafe)│                    │                     │  │
│  └─────────────┘                    └─────────────────────┘  │
│         │                                                    │
└─────────│────────────────────────────────────────────────────┘
          │
          │ (Thread-safe communication)
          │
┌─────────│────────────────────────────────────────────────────┐
│         │                  AI Thread                         │
├─────────│────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────────┐  ┌─────────────────┐   │
│  │ AI Thread   │  │  A* Pathfinder  │  │   Path Cache    │   │
│  │  Manager    │  │                 │  │                 │   │
│  └─────────────┘  └─────────────────┘  └─────────────────┘   │
└─────────────────────────────────────────────────────────────┘
```

## Core Components

### 1. Snake Hierarchy

```cpp
SnakeBase (Abstract Base Class)
├── PlayerSnake (Human-controlled)
└── AISnake (Computer-controlled)
```

**SnakeBase** provides common functionality:
- Position management
- Body growth mechanics
- Collision detection
- Movement physics
- Virtual methods for direction changes

**PlayerSnake** handles:
- Input-based direction changes
- Human player scoring

**AISnake** handles:
- A* pathfinding integration
- Autonomous decision making
- Dynamic path replanning

### 2. Game State Management

```cpp
class GameState {
    // Thread-safe access to game data
    std::shared_ptr<PlayerSnake> player_snake;
    std::shared_ptr<AISnake> ai_snake;
    SDL_Point food_position;
    std::atomic<bool> game_running;
    std::mutex state_mutex;
    std::condition_variable state_condition;
};
```

**Responsibilities:**
- Centralized game state storage
- Thread-safe access patterns
- State synchronization between threads
- Event notification system

### 3. A* Pathfinding System

```cpp
class AStarPathfinder {
    struct Node {
        SDL_Point position;
        float g_score, h_score, f_score;
        std::shared_ptr<Node> parent;
    };
    
    std::priority_queue<std::shared_ptr<Node>> open_set;
    std::unordered_set<SDL_Point> closed_set;
};
```

**Features:**
- Efficient pathfinding using priority queue
- Dynamic obstacle avoidance
- Path caching and reuse
- Heuristic optimization

### 4. Threading Architecture

```cpp
class AIThreadManager {
    std::thread ai_thread;
    std::atomic<bool> should_terminate;
    std::shared_ptr<GameState> game_state;
    std::unique_ptr<AStarPathfinder> pathfinder;
};
```

**Threading Model:**
- Main thread: Game loop, rendering, input
- AI thread: Pathfinding, AI decision making
- Lock-free communication where possible
- Graceful thread termination

## Data Flow

### Main Game Loop
1. **Input Processing**: Controller handles player input
2. **State Update**: Game manager updates both snakes
3. **Collision Detection**: Check snake-snake and snake-food collisions
4. **Rendering**: Draw both snakes and food
5. **Frame Rate Control**: Maintain consistent game speed

### AI Processing Loop
1. **State Acquisition**: Get current game state (thread-safe)
2. **Path Planning**: Run A* algorithm to find path to food
3. **Path Validation**: Check if current path is still valid  
4. **Direction Update**: Set AI snake's direction
5. **State Communication**: Update AI decisions to main thread

## Memory Management Strategy

### Smart Pointer Usage
- `std::shared_ptr<GameState>` - Shared between threads
- `std::unique_ptr<AStarPathfinder>` - Owned by AI thread
- `std::shared_ptr<Node>` - A* algorithm nodes
- `std::unique_ptr<Snake>` - Snake object ownership

### RAII Implementation
- Automatic thread cleanup in destructors
- SDL resource management wrapper classes
- Lock guards for mutex management
- Exception-safe resource handling

## Synchronization Strategy

### Thread-Safe Patterns
```cpp
// Read access pattern
{
    std::shared_lock<std::shared_mutex> lock(state_mutex);
    // Read game state
}

// Write access pattern  
{
    std::unique_lock<std::shared_mutex> lock(state_mutex);
    // Modify game state
    state_condition.notify_all();
}

// Atomic operations for simple flags
std::atomic<bool> game_running{true};
std::atomic<int> ai_score{0};
```

### Lock Hierarchy
1. **Game State Lock** - Highest priority
2. **Rendering Lock** - Medium priority  
3. **Path Cache Lock** - Lowest priority

## Performance Considerations

### A* Optimization
- Bidirectional search for long paths
- Jump point search for grid optimization
- Path smoothing for natural movement
- Incremental path updates

### Threading Performance
- Minimize lock contention
- Use atomic operations for counters
- Lock-free queues for communication
- Thread-local storage for pathfinding

### Memory Efficiency
- Object pooling for A* nodes
- Path caching to avoid recomputation
- Smart pointer optimization
- Memory mapping for large grids

## Error Handling Strategy

### Thread Safety
- Exception handling in thread functions
- Graceful thread termination
- Resource cleanup on exceptions
- Deadlock prevention

### Game Logic
- Invalid path handling
- Food placement conflicts
- Snake collision edge cases
- Memory allocation failures

## Extensibility Design

### Plugin Architecture
- Abstract AI controller interface
- Configurable pathfinding algorithms
- Pluggable collision detection
- Customizable rendering backends

### Configuration System
- AI difficulty levels
- Pathfinding parameters
- Performance tuning options
- Debug visualization modes

## Testing Strategy

### Unit Tests
- A* algorithm correctness
- Snake movement logic
- Collision detection accuracy
- Thread synchronization

### Integration Tests
- Multi-threaded game simulation
- Performance benchmarking
- Memory leak detection
- Stress testing with multiple AIs

This architecture ensures a robust, performant, and maintainable implementation that showcases modern C++ capabilities while providing an engaging gaming experience. 