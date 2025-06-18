#ifndef ASTAR_PATHFINDER_H
#define ASTAR_PATHFINDER_H

#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map> 
#include <memory>
#include <functional>
#include "SDL.h"
#include "snake_base.h"

// Custom hash function for SDL_Point
struct SDL_PointHash {
    std::size_t operator()(const SDL_Point& p) const {
        return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
    }
};

// Custom equality operator for SDL_Point
struct SDL_PointEqual {
    bool operator()(const SDL_Point& a, const SDL_Point& b) const {
        return a.x == b.x && a.y == b.y;
    }
};

class AStarPathfinder {
public:
    struct Node {
        SDL_Point position;
        float g_score{0.0f};  // Cost from start
        float h_score{0.0f};  // Heuristic cost to goal
        float f_score{0.0f};  // Total cost
        std::shared_ptr<Node> parent{nullptr};
        
        Node(SDL_Point pos) : position(pos) {}
    };
    
    // Comparator for priority queue (min-heap based on f_score)
    struct NodeComparator {
        bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
            return a->f_score > b->f_score;  // Greater than for min-heap
        }
    };

    AStarPathfinder(int grid_width, int grid_height);
    
    // Find path from start to goal, avoiding obstacles
    std::vector<SDL_Point> FindPath(
        const SDL_Point& start,
        const SDL_Point& goal,
        const std::vector<SDL_Point>& player_body,
        const std::vector<SDL_Point>& ai_body);

private:
    int grid_width_;
    int grid_height_;
    
    // Priority queue for open set
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator> open_set_;
    
    // Maps for efficient lookup
    std::unordered_map<SDL_Point, std::shared_ptr<Node>, SDL_PointHash, SDL_PointEqual> all_nodes_;
    std::unordered_set<SDL_Point, SDL_PointHash, SDL_PointEqual> closed_set_;
    std::unordered_set<SDL_Point, SDL_PointHash, SDL_PointEqual> obstacles_;
    
    // Helper methods
    float CalculateHeuristic(const SDL_Point& a, const SDL_Point& b) const;
    std::vector<SDL_Point> GetNeighbors(const SDL_Point& point) const;
    std::vector<SDL_Point> ReconstructPath(std::shared_ptr<Node> goal_node) const;
    bool IsValidPosition(const SDL_Point& point) const;
    void SetupObstacles(const std::vector<SDL_Point>& player_body, const std::vector<SDL_Point>& ai_body);
    void ClearState();
};

#endif 