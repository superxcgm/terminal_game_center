#ifndef SNAKE_SRC_CONFIG_H_
#define SNAKE_SRC_CONFIG_H_

class Config {
 public:
  void DecreaseLevel() {
    if (level_ > 1) {
      level_--;
    }
  }

  void IncreaseLevel() {
    if (level_ < 9) {
      level_++;
    }
  }

  bool is_real_wall() const { return real_border_; }

  void set_real_border(bool real_border) { this->real_border_ = real_border; }

  int get_level() const { return level_; }

 private:
  int level_ = 1;
  bool real_border_ = true;
};

#endif  // SNAKE_SRC_CONFIG_H_
