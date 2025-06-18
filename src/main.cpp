#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  
  // Set up AI toggling
  controller.SetGame(&game);
  
  std::cout << "=== Snake Game: Player vs AI ===" << std::endl;
  std::cout << "Controls:" << std::endl;
  std::cout << "  Arrow Keys - Move player snake" << std::endl;
  std::cout << "  A - Toggle AI snake on/off" << std::endl;
  std::cout << "  ESC - Quit game" << std::endl;
  std::cout << std::endl;
  std::cout << "Player snake: Blue" << std::endl;
  std::cout << "AI snake: Red" << std::endl;
  std::cout << "Starting game..." << std::endl;

  game.Run(controller, renderer, kMsPerFrame);
  
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Final Scores:\n";
  std::cout << "  Player Score: " << game.GetPlayerScore() << "\n";
  std::cout << "  Player Size: " << game.GetPlayerSize() << "\n";
  std::cout << "  AI Score: " << game.GetAIScore() << "\n";
  std::cout << "  AI Size: " << game.GetAISize() << "\n";
  
  return 0;
}