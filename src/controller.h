#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "player_snake.h"

class Controller {
 public:
  void InputEvent(bool &running, PlayerSnake &snake) const;
};

#endif
