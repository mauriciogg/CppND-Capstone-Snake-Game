#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "ai_snake.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const &player_snake, AISnake const &ai_snake, SDL_Point const &food);
  void UpdateWindowTitle(int player_score, int ai_score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  
  void RenderSnake(const SnakeBase &snake, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;
};

#endif