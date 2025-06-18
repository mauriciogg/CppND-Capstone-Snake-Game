#include "astar_pathfinder.h"
#include <cmath>
#include <algorithm>

AStarPathfinder::AStarPathfinder(int grid_width, int grid_height)
    : grid_width_(grid_width), grid_height_(grid_height) {
}

std::vector<SDL_Point> AStarPathfinder::FindPath(
    const SDL_Point& start,
    const SDL_Point& goal,
    const std::vector<SDL_Point>& player_body,
    const std::vector<SDL_Point>& ai_body) {
    
    // Clear previous state
    ClearState();
    
    // Setup obstacles (both snakes' bodies)
    SetupObstacles(player_body, ai_body);
    
    // Check if goal is reachable
    if (obstacles_.count(goal) > 0) {
        return {}; // Goal is blocked
    }
    
    // Create start node
    auto start_node = std::make_shared<Node>(start);
    start_node->g_score = 0.0f;
    start_node->h_score = CalculateHeuristic(start, goal);
    start_node->f_score = start_node->g_score + start_node->h_score;
    
    // Add to open set and node map
    open_set_.push(start_node);
    all_nodes_[start] = start_node;
    
    while (!open_set_.empty()) {
        // Get node with lowest f_score
        auto current = open_set_.top();
        open_set_.pop();
        
        // Skip if already processed (can happen with priority queue)
        if (closed_set_.count(current->position) > 0) {
            continue;
        }
        
        // Add to closed set
        closed_set_.insert(current->position);
        
        // Check if we reached the goal
        if (current->position.x == goal.x && current->position.y == goal.y) {
            return ReconstructPath(current);
        }
        
        // Explore neighbors
        auto neighbors = GetNeighbors(current->position);
        for (const auto& neighbor_pos : neighbors) {
            // Skip if already processed or is an obstacle
            if (closed_set_.count(neighbor_pos) > 0 || obstacles_.count(neighbor_pos) > 0) {
                continue;
            }
            
            float tentative_g_score = current->g_score + 1.0f; // Assuming unit cost
            
            // Check if we have a better path to this neighbor
            auto neighbor_it = all_nodes_.find(neighbor_pos);
            std::shared_ptr<Node> neighbor_node;
            
            if (neighbor_it == all_nodes_.end()) {
                // Create new node
                neighbor_node = std::make_shared<Node>(neighbor_pos);
                all_nodes_[neighbor_pos] = neighbor_node;
            } else {
                neighbor_node = neighbor_it->second;
                // Skip if we found a worse path
                if (tentative_g_score >= neighbor_node->g_score) {
                    continue;
                }
            }
            
            // Update neighbor node
            neighbor_node->parent = current;
            neighbor_node->g_score = tentative_g_score;
            neighbor_node->h_score = CalculateHeuristic(neighbor_pos, goal);
            neighbor_node->f_score = neighbor_node->g_score + neighbor_node->h_score;
            
            // Add to open set
            open_set_.push(neighbor_node);
        }
    }
    
    // No path found
    return {};
}

float AStarPathfinder::CalculateHeuristic(const SDL_Point& a, const SDL_Point& b) const {
    // Manhattan distance
    return static_cast<float>(std::abs(a.x - b.x) + std::abs(a.y - b.y));
}

std::vector<SDL_Point> AStarPathfinder::GetNeighbors(const SDL_Point& point) const {
    std::vector<SDL_Point> neighbors;
    
    // Four-directional movement
    std::vector<SDL_Point> directions = {
        {0, -1},  // Up
        {0, 1},   // Down
        {-1, 0},  // Left
        {1, 0}    // Right
    };
    
    for (const auto& dir : directions) {
        SDL_Point neighbor = {point.x + dir.x, point.y + dir.y};
        
        // Handle wrapping (same as snake movement)
        neighbor.x = (neighbor.x + grid_width_) % grid_width_;
        neighbor.y = (neighbor.y + grid_height_) % grid_height_;
        
        if (IsValidPosition(neighbor)) {
            neighbors.push_back(neighbor);
        }
    }
    
    return neighbors;
}

std::vector<SDL_Point> AStarPathfinder::ReconstructPath(std::shared_ptr<Node> goal_node) const {
    std::vector<SDL_Point> path;
    auto current = goal_node;
    
    while (current != nullptr) {
        path.push_back(current->position);
        current = current->parent;
    }
    
    // Reverse to get path from start to goal
    std::reverse(path.begin(), path.end());
    
    // Remove the start position (we're already there)
    if (!path.empty()) {
        path.erase(path.begin());
    }
    
    return path;
}

bool AStarPathfinder::IsValidPosition(const SDL_Point& point) const {
    return point.x >= 0 && point.x < grid_width_ && 
           point.y >= 0 && point.y < grid_height_;
}

void AStarPathfinder::SetupObstacles(const std::vector<SDL_Point>& player_body, const std::vector<SDL_Point>& ai_body) {
    obstacles_.clear();
    
    // Add player snake body as obstacles
    for (const auto& segment : player_body) {
        obstacles_.insert(segment);
    }
    
    // Add AI snake body as obstacles (excluding head since we're planning for it)
    for (const auto& segment : ai_body) {
        obstacles_.insert(segment);
    }
}

void AStarPathfinder::ClearState() {
    // Clear the priority queue
    while (!open_set_.empty()) {
        open_set_.pop();
    }
    
    all_nodes_.clear();
    closed_set_.clear();
    obstacles_.clear();
} 