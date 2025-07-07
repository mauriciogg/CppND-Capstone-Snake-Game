#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <mutex>
#include <atomic>
#include <memory>
#include "SDL.h"
#include "snake_base.h"
#include "player_snake.h"
#include "ai_snake.h"

class GameState {
 public:
  GameState(int grid_width, int grid_height);
  
  void UpdateFood(const SDL_Point& food_position);
  void UpdatePlayerSnake(std::shared_ptr<PlayerSnake> player_snake);
  void UpdateAISnake(std::shared_ptr<AISnake> ai_snake);
  
  SDL_Point GetFood() const;
  std::shared_ptr<PlayerSnake> GetPlayerSnake() const;
  std::shared_ptr<AISnake> GetAISnake() const;
  
  std::vector<const SnakeBase*> GetObstacles() const;
  
  std::atomic<bool> game_running{true};
  std::atomic<int> player_score{0};
  std::atomic<int> ai_score{0};
  
 private:
  mutable std::mutex state_mutex_;
  SDL_Point food_position_;
  std::shared_ptr<PlayerSnake> player_snake_;
  std::shared_ptr<AISnake> ai_snake_;
  int grid_width_;
  int grid_height_;
};

#endif