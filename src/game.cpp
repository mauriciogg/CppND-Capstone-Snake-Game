#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : grid_width_(grid_width), grid_height_(grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  player_snake_ = std::make_shared<PlayerSnake>(grid_width, grid_height);
  ai_snake_ = std::make_shared<AISnake>(grid_width, grid_height);
  game_state_ = std::make_shared<GameState>(grid_width, grid_height);
  pathfinding_thread_ = std::make_unique<PathfindingThread>(game_state_);
  
  PlaceFood();
  pathfinding_thread_->Start();
}

Game::~Game() {
  if (pathfinding_thread_) {
    pathfinding_thread_->Stop();
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running && game_state_->game_running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.InputEvent(running, *player_snake_);
    Update();
    renderer.Render(*player_snake_, *ai_snake_, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(player_score_, ai_score_, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by either snake before placing food.
    if (!player_snake_->SnakeCell(x, y) && !ai_snake_->SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      game_state_->UpdateFood(food);
      pathfinding_thread_->NotifyStateChanged();
      return;
    }
  }
}

void Game::Update() {
  if (!player_snake_->IsAlive()) {
    ResetGame();
    return;
  }

  player_snake_->Update();
  ai_snake_->Update();
  
  // Update game state
  game_state_->UpdatePlayerSnake(player_snake_);
  game_state_->UpdateAISnake(ai_snake_);
  
  HandleCollisions();

  // Check if player snake got food
  int player_x = static_cast<int>(player_snake_->GetHeadX());
  int player_y = static_cast<int>(player_snake_->GetHeadY());
  
  if (food.x == player_x && food.y == player_y) {
    player_score_++;
    PlaceFood();
    player_snake_->GrowBody();
    player_snake_->speed += 0.02;
  }
  
  // Check if AI snake got food
  int ai_x = static_cast<int>(ai_snake_->GetHeadX());
  int ai_y = static_cast<int>(ai_snake_->GetHeadY());
  
  if (food.x == ai_x && food.y == ai_y) {
    ai_score_++;
    PlaceFood();
    ai_snake_->GrowBody();
    ai_snake_->speed += 0.02;  
  }
}

int Game::GetPlayerScore() const { return player_score_; }
int Game::GetAIScore() const { return ai_score_; }
int Game::GetPlayerSize() const { return player_snake_->GetSize(); }
int Game::GetAISize() const { return ai_snake_->GetSize(); }

bool Game::CheckSnakeCollision(const SnakeBase* snake1, const SnakeBase* snake2) const {
  if (!snake1 || !snake2) return false;
  
  int snake1_x = static_cast<int>(snake1->GetHeadX());
  int snake1_y = static_cast<int>(snake1->GetHeadY());
  
  return snake2->SnakeCell(snake1_x, snake1_y);
}

void Game::HandleCollisions() {
  // Check if player snake collided with AI snake
  if (CheckSnakeCollision(player_snake_.get(), ai_snake_.get())) {
    ResetGame();
    return;
  }
  
  // Check if AI snake collided with player snake
  if (CheckSnakeCollision(ai_snake_.get(), player_snake_.get())) {
    ResetGame();
    return;
  }
}

void Game::ResetGame() {
  // Print final scores before reset
  std::cout << "=== GAME OVER ===\n";
  std::cout << "Final Scores - Player: " << player_score_ << " | AI: " << ai_score_ << "\n";
  std::cout << "Snake Sizes - Player: " << player_snake_->GetSize() << " | AI: " << ai_snake_->GetSize() << "\n";
  
  if (player_score_ > ai_score_) {
    std::cout << "Player wins this round!\n";
  } else if (ai_score_ > player_score_) {
    std::cout << "AI wins this round!\n";
  } else {
    std::cout << "It's a tie!\n";
  }
  
  std::cout << "Restarting game...\n\n";
  
  // Reset scores
  player_score_ = 0;
  ai_score_ = 0;
  
  // Reset snakes
  player_snake_ = std::make_shared<PlayerSnake>(grid_width_, grid_height_);
  ai_snake_ = std::make_shared<AISnake>(grid_width_, grid_height_);
  
  // Update game state
  game_state_->UpdatePlayerSnake(player_snake_);
  game_state_->UpdateAISnake(ai_snake_);
  
  // Place new food
  PlaceFood();
  
  // Notify pathfinding thread
  pathfinding_thread_->NotifyStateChanged();
}
