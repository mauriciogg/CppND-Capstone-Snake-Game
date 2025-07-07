#include "game_state.h"
#include <mutex>

GameState::GameState(int grid_width, int grid_height)
    : grid_width_(grid_width), grid_height_(grid_height), food_position_{0, 0} {}

void GameState::UpdateFood(const SDL_Point& food_position) {
  std::lock_guard<std::mutex> lock(state_mutex_);
  food_position_ = food_position;
}

void GameState::UpdatePlayerSnake(std::shared_ptr<PlayerSnake> player_snake) {
  std::lock_guard<std::mutex> lock(state_mutex_);
  player_snake_ = player_snake;
}

void GameState::UpdateAISnake(std::shared_ptr<AISnake> ai_snake) {
  std::lock_guard<std::mutex> lock(state_mutex_);
  ai_snake_ = ai_snake;
}

SDL_Point GameState::GetFood() const {
  std::lock_guard<std::mutex> lock(state_mutex_);
  return food_position_;
}

std::shared_ptr<PlayerSnake> GameState::GetPlayerSnake() const {
  std::lock_guard<std::mutex> lock(state_mutex_);
  return player_snake_;
}

std::shared_ptr<AISnake> GameState::GetAISnake() const {
  std::lock_guard<std::mutex> lock(state_mutex_);
  return ai_snake_;
}

std::vector<const SnakeBase*> GameState::GetObstacles() const {
  std::lock_guard<std::mutex> lock(state_mutex_);
  std::vector<const SnakeBase*> obstacles;
  
  if (player_snake_) {
    obstacles.push_back(player_snake_.get());
  }
  if (ai_snake_) {
    obstacles.push_back(ai_snake_.get());
  }
  
  return obstacles;
}