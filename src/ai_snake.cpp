#include "ai_snake.h"
#include <cmath>
#include <iostream>

AISnake::AISnake(int grid_width, int grid_height)
    : SnakeBase(grid_width, grid_height),
      pathfinder_(std::make_unique<AStarPathfinder>(grid_width, grid_height)) {
    // Start AI snake at a different position from player
    head_x = grid_width / 4;
    head_y = grid_height / 4;
}

void AISnake::HandleDirectionChange() {
    // Update pathfinding every few frames to reduce computational load
    pathfinding_counter_++;
    if (pathfinding_counter_ >= 10) { // Update every 10 frames
        UpdatePathfinding();
        pathfinding_counter_ = 0;
    }
    
    // Follow the current path
    if (!current_path_.empty() && path_index_ < current_path_.size()) {
        Direction new_direction = GetDirectionToNextWaypoint();
        
        // Only change direction if it's valid (not opposite to current direction)
        if (size == 1 || 
            (new_direction != Direction::kUp && direction != Direction::kDown) ||
            (new_direction != Direction::kDown && direction != Direction::kUp) ||
            (new_direction != Direction::kLeft && direction != Direction::kRight) ||
            (new_direction != Direction::kRight && direction != Direction::kLeft)) {
            direction = new_direction;
        }
        
        // Check if we've reached the current waypoint
        SDL_Point current_pos = GetCurrentPosition();
        if (path_index_ < current_path_.size() &&
            current_pos.x == current_path_[path_index_].x &&
            current_pos.y == current_path_[path_index_].y) {
            path_index_++;
        }
    }
}

void AISnake::SetTarget(const SDL_Point& food_pos, const std::vector<SDL_Point>& player_body) {
    target_food_ = food_pos;
    player_body_ = player_body;
}

void AISnake::UpdatePathfinding() {
    if (target_food_.x == -1 || target_food_.y == -1) {
        return; // No target set
    }
    
    // Check if current path is still valid
    if (!IsPathValid()) {
        // Recalculate path
        SDL_Point start = GetCurrentPosition();
        current_path_ = pathfinder_->FindPath(start, target_food_, player_body_, body);
        path_index_ = 0;
        
        if (current_path_.empty()) {
            // No path found, try to move randomly to avoid getting stuck
            // For now, just continue in current direction
            std::cout << "AI Snake: No path to food found!" << std::endl;
        }
    }
}

SnakeBase::Direction AISnake::GetDirectionToNextWaypoint() const {
    if (current_path_.empty() || path_index_ >= current_path_.size()) {
        return direction; // Keep current direction if no path
    }
    
    SDL_Point current_pos = GetCurrentPosition();
    SDL_Point next_waypoint = current_path_[path_index_];
    
    // Calculate direction to next waypoint
    int dx = next_waypoint.x - current_pos.x;
    int dy = next_waypoint.y - current_pos.y;
    
    // Handle wrapping
    if (dx > grid_width / 2) dx -= grid_width;
    if (dx < -grid_width / 2) dx += grid_width;
    if (dy > grid_height / 2) dy -= grid_height;
    if (dy < -grid_height / 2) dy += grid_height;
    
    // Determine direction
    if (std::abs(dx) > std::abs(dy)) {
        return (dx > 0) ? Direction::kRight : Direction::kLeft;
    } else {
        return (dy > 0) ? Direction::kDown : Direction::kUp;
    }
}

bool AISnake::IsPathValid() const {
    if (current_path_.empty() || path_index_ >= current_path_.size()) {
        return false;
    }
    
    // Check if any part of the remaining path is now blocked
    for (size_t i = path_index_; i < current_path_.size(); ++i) {
        const SDL_Point& waypoint = current_path_[i];
        
        // Check against player body
        for (const auto& segment : player_body_) {
            if (waypoint.x == segment.x && waypoint.y == segment.y) {
                return false;
            }
        }
        
        // Check against our own body (excluding head)
        for (const auto& segment : body) {
            if (waypoint.x == segment.x && waypoint.y == segment.y) {
                return false;
            }
        }
    }
    
    return true;
}

SDL_Point AISnake::GetCurrentPosition() const {
    return {static_cast<int>(head_x), static_cast<int>(head_y)};
} 