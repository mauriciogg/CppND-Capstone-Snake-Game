#include "game_state.h"
#include <mutex>

GameState::GameState() {
    // Initialize with default grid dimensions
    // These will be updated by the Game class during initialization
}

void GameState::SetFoodPosition(const SDL_Point& food_pos) {
    std::lock_guard<std::mutex> lock(state_mutex_);
    food_position_ = food_pos;
}

SDL_Point GameState::GetFoodPosition() const {
    std::lock_guard<std::mutex> lock(state_mutex_);
    return food_position_;
}

void GameState::SetGameRunning(bool running) {
    game_running_.store(running);
}

bool GameState::IsGameRunning() const {
    return game_running_.load();
}

std::shared_ptr<PlayerSnake> GameState::GetPlayerSnake() {
    std::lock_guard<std::mutex> lock(state_mutex_);
    if (!player_snake_) {
        player_snake_ = std::make_shared<PlayerSnake>(grid_width_, grid_height_);
    }
    return player_snake_;
}

std::shared_ptr<AISnake> GameState::GetAISnake() {
    std::lock_guard<std::mutex> lock(state_mutex_);
    if (!ai_snake_) {
        ai_snake_ = std::make_shared<AISnake>(grid_width_, grid_height_);
    }
    return ai_snake_;
}

void GameState::SetAIEnabled(bool enabled) {
    ai_enabled_.store(enabled);
}

bool GameState::IsAIEnabled() const {
    return ai_enabled_.load();
}

void GameState::UpdateAITarget() {
    if (!IsAIEnabled()) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(state_mutex_);
    if (ai_snake_ && player_snake_) {
        ai_snake_->SetTarget(food_position_, player_snake_->body);
    }
} 