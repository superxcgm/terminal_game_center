#ifndef SNAKE_SRC_GAME_H_
#define SNAKE_SRC_GAME_H_

#include <queue>
#include <random>

#include "./config.h"
#include "./menu.h"
#include "./res.h"
#include "./snake.h"

class Game {
 public:
  Game();

  void Init();

  void Run();

  void Destroy();

 private:
  Snake snake_;

  bool is_game_over_;

  std::queue<int> queue_dir_;

  Rect rect_;

  Point fruit_;

  Menu menu_;

  Config config_;

  Resource res_game_over_;

  std::default_random_engine random_engine_;

  bool is_hit_wall();

  void OnGame();

  void AddBorderBack();

  void GameOver();

  void DrawFruit();

  bool is_hit_body();

  bool is_hit_fruit();

  bool is_hit_snake(const Point &point);

  friend void RedrawSnack(int signum);
};

#endif  // SNAKE_SRC_GAME_H_
