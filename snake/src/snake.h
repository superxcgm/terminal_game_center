#ifndef SNAKE_SRC_SNAKE_H_
#define SNAKE_SRC_SNAKE_H_

#include <list>

#include "./graph.h"

#define WIN_LINES 24
#define WIN_COLS 80
#define VERSION "0.5"
#define INIT_X 40
#define INIT_Y 12
#define INIT_LEN 3
#define DIR_LEFT 1
#define DIR_RIGHT 2
#define DIR_UP 3
#define DIR_DOWN 4
#define SYMBOL_SNAKE_BODY 'o'
#define SYMBOL_SNAKE_HEAD '@'
#define SYMBOL_FRUIT '$'
#define SYMBOL_BLANK ' '

class Snake {
 public:
  Snake();

  void Draw();

  void Update(const Rect& rect);

  void AddHead(const Point& p);

  void ChangeDirection(int new_direction);

  void DuplicateTail();

  bool is_hit(const Point& p, bool ignore_head, const Rect& rect,
              bool is_real_wall);

  Point head() { return data_.front(); }

 private:
  int direction_;

  bool valid_change_direction(int new_direction);

  void print_snake();

  std::list<Point> data_;

  friend void RedrawSnack(int signum);
};

#endif  // SNAKE_SRC_SNAKE_H_
