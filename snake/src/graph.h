#ifndef SNAKE_SRC_GRAPH_H_
#define SNAKE_SRC_GRAPH_H_

#include <ncurses.h>

struct Point {
  int x;
  int y;
};

bool is_horizontal_line(const Point &a, const Point &b);

bool is_vertical_line(const Point &a, const Point &b);

class Rect {
 public:
  Rect(int x, int y, int width, int height)
      : x(x), y(y), width(width), height(height) {}

  void draw(bool is_real) {
    int ch = is_real ? '#' : '.';
    move(0, 0);
    hline(ch, width);
    move(height, 0); /* NOLINT */
    hline(ch, width);
    move(0, 0); /* NOLINT */
    vline(ch, height);
    move(0, width); /* NOLINT */
    vline(ch, height);
  }

  int left() const { return x; }

  int top() const { return y; }

  int right() const { return x + width - 1; }

  int bottom() const { return y + height - 1; }

  int get_width() const { return width; }

  int get_height() const { return height; }

 private:
  int x;
  int y;
  int width;
  int height;
};

#endif  // SNAKE_SRC_GRAPH_H_
