#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "player_snake.h"

class Game; // Forward declaration

class Controller {
 public:
  void HandleInput(bool &running, PlayerSnake &snake) const;
  void SetGame(Game* game); // For AI toggling

 private:
  void ChangeDirection(PlayerSnake &snake, PlayerSnake::Direction input,
                       PlayerSnake::Direction opposite) const;
  Game* game_{nullptr};
};

#endif