#include "graph.h" /* NOLINT */

bool IsHorizontalLine(const Point &a, const Point &b) { return a.y == b.y; }

bool IsVerticalLine(const Point &a, const Point &b) { return a.x == b.x; }
