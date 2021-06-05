#ifndef SNAKE_MENU_H
#define SNAKE_MENU_H

#include "config.h"
#include "graph.h"
#include "res.h"

class Menu {
 public:
  Menu(const Rect& rect);

  Config draw_main();

 private:
  void draw_border(char ch);

  void draw_control_menu(int flag, int base, const Config& config);

  Resource res_snake;
  Resource res_control_menu;

  Rect rect;
};

#endif  // SNAKE_MENU_H
