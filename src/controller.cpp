#include "controller.h"
#include <iostream>
#include "SDL.h"

void Controller::HandleInput(bool &running, PlayerSnake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          snake.ChangeDirection(PlayerSnake::Direction::kUp,
                          PlayerSnake::Direction::kDown);
          break;

        case SDLK_DOWN:
          snake.ChangeDirection(PlayerSnake::Direction::kDown,
                          PlayerSnake::Direction::kUp);
          break;

        case SDLK_LEFT:
          snake.ChangeDirection(PlayerSnake::Direction::kLeft,
                          PlayerSnake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          snake.ChangeDirection(PlayerSnake::Direction::kRight,
                          PlayerSnake::Direction::kLeft);
          break;
      }
    }
  }
}
