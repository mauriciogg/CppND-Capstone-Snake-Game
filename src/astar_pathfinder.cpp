#include "astar_pathfinder.h"
#include <cmath>
#include <algorithm>

AStarPathfinder::AStarPathfinder(int grid_width, int grid_height)
    : grid_width_(grid_width), grid_height_(grid_height) {}

std::vector<SDL_Point> AStarPathfinder::FindPath(const SDL_Point& start, 
                                                 const SDL_Point& goal, 
                                                 const std::vector<const SnakeBase*>& obstacles) {
  std::priority_queue<Node> open_set;
  std::unordered_set<std::pair<int, int>, NodeHash> closed_set;
  std::unordered_set<std::pair<int, int>, NodeHash> open_set_coords;
  
  auto start_node = std::make_shared<Node>(start.x, start.y, 0, 
                                           CalculateHeuristic(start.x, start.y, goal.x, goal.y));
  open_set.push(*start_node);
  open_set_coords.insert({start.x, start.y});
  
  while (!open_set.empty()) {
    Node current = open_set.top();
    open_set.pop();
    
    auto current_coords = std::make_pair(current.x, current.y);
    open_set_coords.erase(current_coords);
    closed_set.insert(current_coords);
    
    if (current.x == goal.x && current.y == goal.y) {
      return ReconstructPath(std::make_shared<Node>(current));
    }
    
    for (const auto& neighbor_coords : GetNeighbors(current.x, current.y)) {
      int nx = neighbor_coords.first;
      int ny = neighbor_coords.second;
      
      if (closed_set.find({nx, ny}) != closed_set.end()) {
        continue;
      }
      
      if (!IsValidPosition(nx, ny, obstacles)) {
        continue;
      }
      
      float tentative_g = current.g_cost + 1.0f;
      float h_cost = CalculateHeuristic(nx, ny, goal.x, goal.y);
      
      if (open_set_coords.find({nx, ny}) == open_set_coords.end()) {
        auto neighbor = std::make_shared<Node>(nx, ny, tentative_g, h_cost, 
                                               std::make_shared<Node>(current));
        open_set.push(*neighbor);
        open_set_coords.insert({nx, ny});
      }
    }
  }
  
  return std::vector<SDL_Point>();
}

float AStarPathfinder::CalculateHeuristic(int x1, int y1, int x2, int y2) const {
  return std::abs(x1 - x2) + std::abs(y1 - y2);
}

bool AStarPathfinder::IsValidPosition(int x, int y, const std::vector<const SnakeBase*>& obstacles) const {
  if (x < 0 || x >= grid_width_ || y < 0 || y >= grid_height_) {
    return false;
  }
  
  for (const auto* snake : obstacles) {
    if (snake && snake->SnakeCell(x, y)) {
      return false;
    }
  }
  
  return true;
}

std::vector<SDL_Point> AStarPathfinder::ReconstructPath(std::shared_ptr<Node> goal_node) const {
  std::vector<SDL_Point> path;
  std::shared_ptr<Node> current = goal_node;
  
  while (current != nullptr) {
    path.push_back({current->x, current->y});
    current = current->parent;
  }
  
  std::reverse(path.begin(), path.end());
  return path;
}

std::vector<std::pair<int, int>> AStarPathfinder::GetNeighbors(int x, int y) const {
  std::vector<std::pair<int, int>> neighbors;
  
  const std::vector<std::pair<int, int>> directions = {
    {0, -1}, {0, 1}, {-1, 0}, {1, 0}
  };
  
  for (const auto& dir : directions) {
    int nx = (x + dir.first + grid_width_) % grid_width_;
    int ny = (y + dir.second + grid_height_) % grid_height_;
    neighbors.push_back({nx, ny});
  }
  
  return neighbors;
}