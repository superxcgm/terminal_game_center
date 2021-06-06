#ifndef SNAKE_SRC_MENU_H_
#define SNAKE_SRC_MENU_H_

#include "./config.h"
#include "./graph.h"
#include "./res.h"

class Menu {
 public:
  explicit Menu(const Rect& rect);

  Config draw_main();

 private:
  void draw_border(char ch);

  void draw_control_menu(int flag, int base, const Config& config);

  Resource res_snake;
  Resource res_control_menu;

  Rect rect;
};

#endif  // SNAKE_SRC_MENU_H_
