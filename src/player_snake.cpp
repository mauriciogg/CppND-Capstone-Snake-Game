#include "player_snake.h"

void PlayerSnake::HandleDirectionChange() {
  if (direction_changed) {
    direction = pending_direction;
    direction_changed = false;
  }
}

void PlayerSnake::ChangeDirection(Direction input, Direction opposite) {
  if (direction != opposite || size == 1) {
    pending_direction = input;
    direction_changed = true;
  }
} 