#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "game_state.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetPlayerScore() const;
  int GetPlayerSize() const;
  int GetAIScore() const;
  int GetAISize() const;
  
  void ToggleAI(); // Enable/disable AI snake

 private:
  std::shared_ptr<GameState> game_state_;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int player_score{0};
  int ai_score{0};
  std::size_t grid_width_;
  std::size_t grid_height_;

  void PlaceFood();
  void Update();
  bool CheckCollisions();
  bool CheckSnakeCollision(std::shared_ptr<SnakeBase> snake1, std::shared_ptr<SnakeBase> snake2);
};

#endif