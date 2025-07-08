#ifndef SNAKE_BASE_H
#define SNAKE_BASE_H

#include <vector>
#include <memory>
#include "SDL.h"

class SnakeBase {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  SnakeBase(int grid_width, int grid_height);
  virtual ~SnakeBase() = default;

  virtual void Update() = 0;
  virtual void ChangeDirection(Direction input, Direction opposite);

  void GrowBody();
  bool SnakeCell(int x, int y) const;
  bool IsAlive() const { return alive; }
  float GetHeadX() const { return head_x; }
  float GetHeadY() const { return head_y; }
  int GetSize() const { return size; }
  const std::vector<SDL_Point>& GetBody() const { return body; }
  
  Direction direction = Direction::kUp;
  float speed{0.1f};
  int size{1};
  bool alive{true};

 protected:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
  
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;
  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif
