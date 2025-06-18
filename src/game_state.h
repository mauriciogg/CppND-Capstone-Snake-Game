#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <memory>
#include <mutex>
#include <atomic>
#include "SDL.h"
#include "player_snake.h"
#include "ai_snake.h"

class GameState {
 public:
  GameState();
  ~GameState() = default;

  // Thread-safe access methods
  void SetFoodPosition(const SDL_Point& food_pos);
  SDL_Point GetFoodPosition() const;
  
  void SetGameRunning(bool running);
  bool IsGameRunning() const;
  
  // Snake access methods
  std::shared_ptr<PlayerSnake> GetPlayerSnake();
  std::shared_ptr<AISnake> GetAISnake();
  
  // AI control
  void SetAIEnabled(bool enabled);
  bool IsAIEnabled() const;
  
  // Update AI target with current game state
  void UpdateAITarget();

 private:
  std::shared_ptr<PlayerSnake> player_snake_;
  std::shared_ptr<AISnake> ai_snake_;
  
  SDL_Point food_position_{0, 0};
  std::atomic<bool> game_running_{true};
  std::atomic<bool> ai_enabled_{true};
  
  mutable std::mutex state_mutex_;
  
  // Grid dimensions (will be set during initialization)
  int grid_width_{32};
  int grid_height_{32};
  
  friend class Game; // Allow Game class to set grid dimensions
};

#endif 