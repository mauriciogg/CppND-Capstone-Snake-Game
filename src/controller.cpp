#include "controller.h"
#include "game.h"
#include <iostream>
#include "SDL.h"

void Controller::ChangeDirection(PlayerSnake &snake, PlayerSnake::Direction input,
                                 PlayerSnake::Direction opposite) const {
  snake.ChangeDirection(input, opposite);
}

void Controller::HandleInput(bool &running, PlayerSnake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, PlayerSnake::Direction::kUp,
                          PlayerSnake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, PlayerSnake::Direction::kDown,
                          PlayerSnake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, PlayerSnake::Direction::kLeft,
                          PlayerSnake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, PlayerSnake::Direction::kRight,
                          PlayerSnake::Direction::kLeft);
          break;
          
        case SDLK_a:
          // Toggle AI snake
          if (game_) {
            game_->ToggleAI();
          }
          break;
      }
    }
  }
}

void Controller::SetGame(Game* game) {
  game_ = game;
}