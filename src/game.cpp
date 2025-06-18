#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : game_state_(std::make_shared<GameState>()),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      grid_width_(grid_width),
      grid_height_(grid_height) {
  
  // Set grid dimensions in game state
  game_state_->grid_width_ = static_cast<int>(grid_width);
  game_state_->grid_height_ = static_cast<int>(grid_height);
  
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  auto player_snake = game_state_->GetPlayerSnake();
  auto ai_snake = game_state_->GetAISnake();

  while (running && game_state_->IsGameRunning()) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, *player_snake);
    
    Update();
    
    // Check for collisions
    if (CheckCollisions()) {
      game_state_->SetGameRunning(false);
      std::cout << "Game Over! Collision detected." << std::endl;
    }
    
    // Render based on AI state
    if (game_state_->IsAIEnabled()) {
      renderer.Render(*player_snake, *ai_snake, food);
    } else {
      renderer.Render(*player_snake, *player_snake, food);
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(player_score, ai_score, frame_count);
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
  auto player_snake = game_state_->GetPlayerSnake();
  auto ai_snake = game_state_->GetAISnake();
  
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    
    // Check that the location is not occupied by either snake
    bool occupied = false;
    
    if (player_snake && player_snake->SnakeCell(x, y)) {
      occupied = true;
    }
    
    if (!occupied && game_state_->IsAIEnabled() && ai_snake && ai_snake->SnakeCell(x, y)) {
      occupied = true;
    }
    
    if (!occupied) {
      food.x = x;
      food.y = y;
      game_state_->SetFoodPosition(food);
      return;
    }
  }
}

void Game::Update() {
  auto player_snake = game_state_->GetPlayerSnake();
  auto ai_snake = game_state_->GetAISnake();
  
  if (!player_snake->alive) return;

  // Update player snake
  player_snake->Update();

  // Update AI snake if enabled
  if (game_state_->IsAIEnabled() && ai_snake && ai_snake->alive) {
    game_state_->UpdateAITarget();
    ai_snake->Update();
  }

  // Check if player snake ate food
  int player_x = static_cast<int>(player_snake->head_x);
  int player_y = static_cast<int>(player_snake->head_y);

  if (food.x == player_x && food.y == player_y) {
    player_score++;
    PlaceFood();
    player_snake->GrowBody();
    player_snake->speed += 0.02f;
  }

  // Check if AI snake ate food
  if (game_state_->IsAIEnabled() && ai_snake) {
    int ai_x = static_cast<int>(ai_snake->head_x);
    int ai_y = static_cast<int>(ai_snake->head_y);

    if (food.x == ai_x && food.y == ai_y) {
      ai_score++;
      PlaceFood();
      ai_snake->GrowBody();
      ai_snake->speed += 0.02f;
    }
  }
}

bool Game::CheckCollisions() {
  auto player_snake = game_state_->GetPlayerSnake();
  auto ai_snake = game_state_->GetAISnake();
  
  // Check if player snake collided with AI snake
  if (game_state_->IsAIEnabled() && ai_snake && player_snake->alive && ai_snake->alive) {
    return CheckSnakeCollision(player_snake, ai_snake);
  }
  
  return false;
}

bool Game::CheckSnakeCollision(std::shared_ptr<SnakeBase> snake1, std::shared_ptr<SnakeBase> snake2) {
  int snake1_x = static_cast<int>(snake1->head_x);
  int snake1_y = static_cast<int>(snake1->head_y);
  int snake2_x = static_cast<int>(snake2->head_x);
  int snake2_y = static_cast<int>(snake2->head_y);
  
  // Check head-to-head collision
  if (snake1_x == snake2_x && snake1_y == snake2_y) {
    return true;
  }
  
  // Check if snake1's head collides with snake2's body
  if (snake2->SnakeCell(snake1_x, snake1_y)) {
    snake1->alive = false;
    return true;
  }
  
  // Check if snake2's head collides with snake1's body
  if (snake1->SnakeCell(snake2_x, snake2_y)) {
    snake2->alive = false;
    return true;
  }
  
  return false;
}

void Game::ToggleAI() {
  game_state_->SetAIEnabled(!game_state_->IsAIEnabled());
  std::cout << "AI Snake " << (game_state_->IsAIEnabled() ? "Enabled" : "Disabled") << std::endl;
}

int Game::GetPlayerScore() const { return player_score; }
int Game::GetPlayerSize() const { 
  auto player_snake = game_state_->GetPlayerSnake();
  return player_snake ? player_snake->size : 1;
}
int Game::GetAIScore() const { return ai_score; }
int Game::GetAISize() const { 
  auto ai_snake = game_state_->GetAISnake();
  return ai_snake ? ai_snake->size : 1;
}