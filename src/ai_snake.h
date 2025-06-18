#ifndef AI_SNAKE_H
#define AI_SNAKE_H

#include "snake_base.h"
#include "astar_pathfinder.h"
#include <memory>
#include <vector>

class AISnake : public SnakeBase {
 public:
  AISnake(int grid_width, int grid_height);

  void HandleDirectionChange() override;
  
  // Set the target food position and player snake body for pathfinding
  void SetTarget(const SDL_Point& food_pos, const std::vector<SDL_Point>& player_body);
  
  // Update pathfinding (can be called less frequently than the main update)
  void UpdatePathfinding();

 private:
  std::unique_ptr<AStarPathfinder> pathfinder_;
  std::vector<SDL_Point> current_path_;
  SDL_Point target_food_{-1, -1};
  std::vector<SDL_Point> player_body_;
  int path_index_{0};
  int pathfinding_counter_{0};
  
  // Helper methods
  SnakeBase::Direction GetDirectionToNextWaypoint() const;
  bool IsPathValid() const;
  SDL_Point GetCurrentPosition() const;
};

#endif 