#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
 public:
  void InputEvent(bool &running, Snake &snake) const;
};

#endif
