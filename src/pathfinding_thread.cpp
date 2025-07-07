#include "pathfinding_thread.h"
#include <chrono>

PathfindingThread::PathfindingThread(std::shared_ptr<GameState> game_state)
    : game_state_(game_state) {}

PathfindingThread::~PathfindingThread() {
  Stop();
}

void PathfindingThread::Start() {
  if (worker_thread_ && worker_thread_->joinable()) {
    return;
  }
  
  should_stop_ = false;
  worker_thread_ = std::make_unique<std::thread>(&PathfindingThread::WorkerLoop, this);
}

void PathfindingThread::Stop() {
  should_stop_ = true;
  cv_.notify_all();
  
  if (worker_thread_ && worker_thread_->joinable()) {
    worker_thread_->join();
  }
}

void PathfindingThread::NotifyStateChanged() {
  state_changed_ = true;
  cv_.notify_one();
}

void PathfindingThread::WorkerLoop() {
  while (!should_stop_) {
    std::unique_lock<std::mutex> lock(cv_mutex_);
    
    cv_.wait_for(lock, std::chrono::milliseconds(100), [this] {
      return should_stop_.load() || state_changed_.load();
    });
    
    if (should_stop_) {
      break;
    }
    
    if (state_changed_.exchange(false)) {
      UpdateAIPath();
    }
  }
}

void PathfindingThread::UpdateAIPath() {
  if (!game_state_) {
    return;
  }
  
  auto ai_snake = game_state_->GetAISnake();
  if (!ai_snake || !ai_snake->IsAlive()) {
    return;
  }
  
  SDL_Point food = game_state_->GetFood();
  std::vector<const SnakeBase*> obstacles = game_state_->GetObstacles();
  
  ai_snake->SetTarget(food);
  ai_snake->SetObstacles(obstacles);
}