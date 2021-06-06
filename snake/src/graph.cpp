#include "graph.h" /* NOLINT */

bool is_horizontal_line(const Point &a, const Point &b) { return a.y == b.y; }

bool is_vertical_line(const Point &a, const Point &b) { return a.x == b.x; }
