#ifndef PATHFINDING_THREAD_H
#define PATHFINDING_THREAD_H

#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <memory>
#include "game_state.h"

class PathfindingThread {
 public:
  PathfindingThread(std::shared_ptr<GameState> game_state);
  ~PathfindingThread();
  
  void Start();
  void Stop();
  void NotifyStateChanged();
  
 private:
  std::shared_ptr<GameState> game_state_;
  std::unique_ptr<std::thread> worker_thread_;
  std::condition_variable cv_;
  std::mutex cv_mutex_;
  std::atomic<bool> should_stop_{false};
  std::atomic<bool> state_changed_{false};
  
  void WorkerLoop();
  void UpdateAIPath();
};

#endif