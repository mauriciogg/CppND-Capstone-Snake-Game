#ifndef PLAYER_SNAKE_H
#define PLAYER_SNAKE_H

#include "snake_base.h"

class PlayerSnake : public SnakeBase {
 public:
  PlayerSnake(int grid_width, int grid_height) 
      : SnakeBase(grid_width, grid_height) {}

  void HandleDirectionChange() override;
  void ChangeDirection(Direction input, Direction opposite);

 private:
  Direction pending_direction = direction;
  bool direction_changed = false;
};

#endif 