#ifndef SNAKE_SRC_GRAPH_H_
#define SNAKE_SRC_GRAPH_H_

#include <ncurses.h>

struct Point {
  int x;
  int y;
};

bool IsHorizontalLine(const Point &a, const Point &b);

bool IsVerticalLine(const Point &a, const Point &b);

class Rect {
 public:
  Rect(int x, int y, int width, int height)
      : x_(x), y_(y), width_(width), height_(height) {}

  void Draw(bool is_real) {
    int ch = is_real ? '#' : '.';
    move(0, 0);
    hline(ch, width_);
    move(height_, 0); /* NOLINT */
    hline(ch, width_);
    move(0, 0); /* NOLINT */
    vline(ch, height_);
    move(0, width_); /* NOLINT */
    vline(ch, height_);
  }

  int left() const { return x_; }

  int top() const { return y_; }

  int right() const { return x_ + width_ - 1; }

  int bottom() const { return y_ + height_ - 1; }

  int get_width() const { return width_; }

  int get_height() const { return height_; }

 private:
  int x_;
  int y_;
  int width_;
  int height_;
};

#endif  // SNAKE_SRC_GRAPH_H_
