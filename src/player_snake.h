#ifndef PLAYER_SNAKE_H
#define PLAYER_SNAKE_H

#include "snake_base.h"

class PlayerSnake : public SnakeBase {
 public:
  PlayerSnake(int grid_width, int grid_height);
  
  void Update() override;
};

#endif