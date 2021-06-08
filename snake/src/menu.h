#ifndef SNAKE_SRC_MENU_H_
#define SNAKE_SRC_MENU_H_

#include "./config.h"
#include "./graph.h"
#include "./res.h"

class Menu {
 public:
  explicit Menu(const Rect& rect);

  Config DrawMain();

 private:
  void DrawBorder(char ch);

  void DrawControlMenu(int flag, int base, const Config& config);

  Resource res_snake_;
  Resource res_control_menu_;

  Rect rect_;
};

#endif  // SNAKE_SRC_MENU_H_
