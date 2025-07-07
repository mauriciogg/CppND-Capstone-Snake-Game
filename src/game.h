#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "player_snake.h"
#include "ai_snake.h"
#include "game_state.h"
#include "pathfinding_thread.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetPlayerScore() const;
  int GetAIScore() const;
  int GetPlayerSize() const;
  int GetAISize() const;

 private:
  std::shared_ptr<PlayerSnake> player_snake_;
  std::shared_ptr<AISnake> ai_snake_;
  std::shared_ptr<GameState> game_state_;
  std::unique_ptr<PathfindingThread> pathfinding_thread_;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int grid_width_;
  int grid_height_;
  int player_score_{0};
  int ai_score_{0};

  void PlaceFood();
  void Update();
  bool CheckSnakeCollision(const SnakeBase* snake1, const SnakeBase* snake2) const;
  void HandleCollisions();
  void ResetGame();
};

#endif
