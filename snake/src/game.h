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

  void init();

  void run();

  void destroy();

 private:
  Snake the_snake;

  bool is_game_over;

  std::queue<int> queue_dir;

  Rect rect;

  Point fruit;

  Menu menu;

  Config config;

  Resource res_game_over;

  std::default_random_engine random_engine;

  bool is_hit_wall();

  void on_game();

  void add_border_back();

  void game_over();

  void draw_fruit();

  bool is_hit_body();

  bool is_hit_fruit();

  bool is_hit_snake(const Point &point);

  friend void redraw_snack(int signum);
};

#endif  // SNAKE_SRC_GAME_H_
