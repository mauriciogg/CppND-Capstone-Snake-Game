#include "snake_base.h"
#include <cmath>
#include <iostream>

SnakeBase::SnakeBase(int grid_width, int grid_height)
    : grid_width(grid_width),
      grid_height(grid_height),
      head_x(grid_width / 2),
      head_y(grid_height / 2) {}

void SnakeBase::ChangeDirection(Direction input, Direction opposite) {
  if (direction != opposite || size == 1) direction = input;
}

void SnakeBase::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void SnakeBase::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  body.push_back(prev_head_cell);

  if (!growing) {
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void SnakeBase::GrowBody() { 
  growing = true; 
}

bool SnakeBase::SnakeCell(int x, int y) const {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}
