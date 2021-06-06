#ifndef SNAKE_SRC_CONFIG_H_
#define SNAKE_SRC_CONFIG_H_

class Config {
 public:
  void decreaseLevel() {
    if (level > 1) {
      level--;
    }
  }

  void increaseLevel() {
    if (level < 9) {
      level++;
    }
  }

  bool is_real_wall() const { return real_border; }

  void set_real_border(bool real_border) { this->real_border = real_border; }

  int get_level() const { return level; }

 private:
  int level = 1;
  bool real_border = true;
};

#endif  // SNAKE_SRC_CONFIG_H_
