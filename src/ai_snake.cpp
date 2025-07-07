#include "ai_snake.h"
#include <cmath>
#include <algorithm>

AISnake::AISnake(int grid_width, int grid_height)
    : SnakeBase(grid_width, grid_height),
      pathfinder_(std::make_unique<AStarPathfinder>(grid_width, grid_height)),
      target_{0, 0},
      path_index_(0),
      update_counter_(0),
      movement_delay_counter_(0),
      rng_(std::random_device{}()),
      fairness_dist_(1, 100) {
  head_x = grid_width / 4.0f;
  head_y = grid_height / 4.0f;
  speed = 0.1;
}

void AISnake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};
  
  update_counter_++;
  
  // Slow down the AI snake, otherwise its nearly impossible to win
  if (!ShouldMoveThisFrame()) {
    return;
  }
  
  if (ShouldRecalculatePath()) {
    UpdatePath();
  }
  
  // Occasionally make suboptimal moves for fairness
  if (!ShouldMakeMistake()) {
    FollowPath();
  }
  UpdateHead();
  
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};

  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void AISnake::SetTarget(const SDL_Point& target) {
  target_ = target;
}

void AISnake::SetObstacles(const std::vector<const SnakeBase*>& obstacles) {
  obstacles_ = obstacles;
}

void AISnake::UpdatePath() {
  SDL_Point current_pos{static_cast<int>(head_x), static_cast<int>(head_y)};
  current_path_ = pathfinder_->FindPath(current_pos, target_, obstacles_);
  path_index_ = 0;
}

void AISnake::FollowPath() {
  if (current_path_.empty() || path_index_ >= current_path_.size()) {
    return;
  }
  
  SDL_Point current_pos{static_cast<int>(head_x), static_cast<int>(head_y)};
  SDL_Point next_point = current_path_[path_index_];
  
  if (current_pos.x == next_point.x && current_pos.y == next_point.y) {
    path_index_++;
    if (path_index_ < current_path_.size()) {
      next_point = current_path_[path_index_];
    } else {
      return;
    }
  }
  
  Direction new_direction = GetDirectionToPoint(next_point);
  
  Direction opposite;
  switch (direction) {
    case Direction::kUp: opposite = Direction::kDown; break;
    case Direction::kDown: opposite = Direction::kUp; break;
    case Direction::kLeft: opposite = Direction::kRight; break;
    case Direction::kRight: opposite = Direction::kLeft; break;
  }
  
  ChangeDirection(new_direction, opposite);
}

SnakeBase::Direction AISnake::GetDirectionToPoint(const SDL_Point& point) const {
  int dx = point.x - static_cast<int>(head_x);
  int dy = point.y - static_cast<int>(head_y);
  
  if (dx > grid_width / 2) dx -= grid_width;
  if (dx < -grid_width / 2) dx += grid_width;
  if (dy > grid_height / 2) dy -= grid_height;
  if (dy < -grid_height / 2) dy += grid_height;
  
  if (std::abs(dx) > std::abs(dy)) {
    return (dx > 0) ? Direction::kRight : Direction::kLeft;
  } else {
    return (dy > 0) ? Direction::kDown : Direction::kUp;
  }
}

// The following methods are here to introduce some degree of fairness to the game
// The values are completely arbitrary and I chosed them based on me playing the
// game in order to find the right mix for a balanced difficulty.
  // A followup could be to make this configurable via a difficulty setting.

bool AISnake::ShouldRecalculatePath() const {
  return current_path_.empty() || 
         path_index_ >= current_path_.size() || 
         update_counter_ % 40  == 0;
}

bool AISnake::ShouldMoveThisFrame() const {
  // Move 70% of the time.
  return fairness_dist_(rng_) <= 75;
}

bool AISnake::ShouldMakeMistake() const {
  // 10% chance to skip optimal move for fairness
  return fairness_dist_(rng_) <= 10;
}
