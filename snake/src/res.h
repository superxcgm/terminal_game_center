#ifndef SNAKE_RES_H
#define SNAKE_RES_H

#include <string>
#include <vector>

class Resource {
 public:
  Resource(const std::string& path);
  int line_count() { return data.size(); }
  std::string& get_line(int i) { return data[i]; }

  void draw(int init_x, int init_y);

 private:
  std::vector<std::string> data;
};
#endif