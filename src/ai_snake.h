#ifndef AI_SNAKE_H
#define AI_SNAKE_H

#include "snake_base.h"
#include "astar_pathfinder.h"
#include <memory>
#include <vector>
#include <random>

class AISnake : public SnakeBase {
 public:
  AISnake(int grid_width, int grid_height);
  
  void Update() override;
  void SetTarget(const SDL_Point& target);
  void SetObstacles(const std::vector<const SnakeBase*>& obstacles);
  
 private:
  std::unique_ptr<AStarPathfinder> pathfinder_;
  std::vector<SDL_Point> current_path_;
  SDL_Point target_;
  std::vector<const SnakeBase*> obstacles_;
  int path_index_;
  int update_counter_;
  int movement_delay_counter_;
  mutable std::mt19937 rng_;
  mutable std::uniform_int_distribution<int> fairness_dist_;
  
  void UpdatePath();
  void FollowPath();
  Direction GetDirectionToPoint(const SDL_Point& point) const;
  bool ShouldRecalculatePath() const;
  bool ShouldMoveThisFrame() const;
  bool ShouldMakeMistake() const;
};

#endif
