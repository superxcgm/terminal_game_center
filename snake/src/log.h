#ifndef SNAKE_SRC_LOG_H_
#define SNAKE_SRC_LOG_H_

#define DEBUG_ON

#include <cstdio>

class Log {
 public:
  static void Init();

  static void Debug(const char *fmt, ...);

 private:
  static FILE *fp_;
};

#endif  // SNAKE_SRC_LOG_H_
