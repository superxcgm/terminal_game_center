#ifndef SNAKE_SRC_RES_H_
#define SNAKE_SRC_RES_H_

#include <string>
#include <vector>

class Resource {
 public:
  explicit Resource(const std::string& path);
  int line_count() { return static_cast<int>(data.size()); }
  std::string& get_line(int i) { return data[i]; }

  void draw(int init_x, int init_y);

 private:
  std::vector<std::string> data;
};
#endif  // SNAKE_SRC_RES_H_
