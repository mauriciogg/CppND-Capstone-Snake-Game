#include "player_snake.h"
#include <cmath>
#include <iostream>

PlayerSnake::PlayerSnake(int grid_width, int grid_height)
    : SnakeBase(grid_width, grid_height) {}

void PlayerSnake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};

  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}