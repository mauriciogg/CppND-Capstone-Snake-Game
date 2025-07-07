#ifndef ASTAR_PATHFINDER_H
#define ASTAR_PATHFINDER_H

#include <vector>
#include <queue>
#include <unordered_set>
#include <memory>
#include "SDL.h"
#include "snake_base.h"

struct Node {
  int x, y;
  float g_cost;
  float h_cost;
  float f_cost;
  std::shared_ptr<Node> parent;
  
  Node(int x, int y, float g = 0, float h = 0, std::shared_ptr<Node> parent = nullptr)
      : x(x), y(y), g_cost(g), h_cost(h), f_cost(g + h), parent(parent) {}
      
  bool operator<(const Node& other) const {
    return f_cost > other.f_cost;
  }
};

struct NodeHash {
  std::size_t operator()(const std::pair<int, int>& p) const {
    return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
  }
};

class AStarPathfinder {
 public:
  AStarPathfinder(int grid_width, int grid_height);
  
  std::vector<SDL_Point> FindPath(const SDL_Point& start, const SDL_Point& goal, 
                                  const std::vector<const SnakeBase*>& obstacles);
  
 private:
  int grid_width_;
  int grid_height_;
  
  float CalculateHeuristic(int x1, int y1, int x2, int y2) const;
  bool IsValidPosition(int x, int y, const std::vector<const SnakeBase*>& obstacles) const;
  std::vector<SDL_Point> ReconstructPath(std::shared_ptr<Node> goal_node) const;
  std::vector<std::pair<int, int>> GetNeighbors(int x, int y) const;
};

#endif