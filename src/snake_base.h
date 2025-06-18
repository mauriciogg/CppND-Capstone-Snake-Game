#ifndef SNAKE_BASE_H
#define SNAKE_BASE_H

#include <vector>
#include <memory>
#include "SDL.h"

class SnakeBase {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  SnakeBase(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  virtual ~SnakeBase() = default;

  // Common functionality
  virtual void Update();
  void GrowBody();
  bool SnakeCell(int x, int y) const;
  
  // Virtual methods for different behaviors
  virtual void HandleDirectionChange() = 0;
  
  // Public members
  Direction direction = Direction::kUp;
  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 protected:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif 