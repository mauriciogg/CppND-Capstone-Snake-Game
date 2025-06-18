#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game - Player vs AI", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(const SnakeBase& player_snake, const SnakeBase& ai_snake, const SDL_Point& food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render player snake (blue)
  RenderSnake(player_snake, 0x00, 0x7A, 0xCC, 0xFF);
  
  // Render AI snake (red) - only if it's different from player snake
  if (&player_snake != &ai_snake) {
    RenderSnake(ai_snake, 0xCC, 0x00, 0x7A, 0xFF);
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::RenderSnake(const SnakeBase& snake, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, r, g, b, a);
  for (const SDL_Point& point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head (slightly brighter)
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  
  if (snake.alive) {
    // Make head slightly brighter
    Uint8 head_r = std::min(255, static_cast<int>(r) + 50);
    Uint8 head_g = std::min(255, static_cast<int>(g) + 50);
    Uint8 head_b = std::min(255, static_cast<int>(b) + 50);
    SDL_SetRenderDrawColor(sdl_renderer, head_r, head_g, head_b, a);
  } else {
    // Dead snake head is dark red
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::UpdateWindowTitle(int player_score, int ai_score, int fps) {
  std::string title{"Snake Game - Player: " + std::to_string(player_score) + 
                   " | AI: " + std::to_string(ai_score) + 
                   " | FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
